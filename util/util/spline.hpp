// Cubic spline

// Copyright snsinfu 2020-2021.
// Distributed under the Boost Software License, Version 1.0.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef INCLUDED_SNSINFU_SPLINE_HPP
#define INCLUDED_SNSINFU_SPLINE_HPP

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <vector>


namespace detail_cubic_spline
{
    /*
     * Solves a tridiagonal system of equations.
     *
     * The arguments lower, diag, upper and rhs define the equations and must
     * be of the same length (say n). The i-th element of each vector specifies
     * the i-th linear equation, i = 0 to n - 1 from the top to the bottom.
     * lower and upper must be padded at lower[0] and upper[n - 1].
     *
     * The vectors are modified in-place. The solution is returned to rhs.
     */
    inline void solve_tridiagonal_system(
        std::vector<double>& lower,
        std::vector<double>& diag,
        std::vector<double>& upper,
        std::vector<double>& rhs
    )
    {
        std::size_t const dim = rhs.size();

        assert(dim != 0);
        assert(lower.size() == dim);
        assert(diag.size() == dim);
        assert(upper.size() == dim);

        // Elimination step.
        for (std::size_t i = 0; i < dim - 1; i++) {
            if (std::fabs(diag[i]) >= std::fabs(lower[i + 1])) {
                // Normal tridiagonal algorithm.
                auto const w = lower[i + 1] / diag[i];
                diag[i + 1] -= w * upper[i];
                rhs[i + 1] -= w * rhs[i];
                lower[i + 1] = 0;
            } else {
                // Pivoting. Here, we interchange the i-th row and the (i+1)-th
                // row, then eliminate. Unlike the other branch, the lower
                // triangular element lower[i+1] will remain. This affects the
                // back substitution step below.
                auto const w = diag[i] / lower[i + 1];

                auto const u = diag[i + 1];
                diag[i] = lower[i + 1];
                diag[i + 1] = upper[i] - w * u;
                lower[i + 1] = upper[i + 1];
                upper[i + 1] *= -w;
                upper[i] = u;

                auto const r = rhs[i];
                rhs[i] = rhs[i + 1];
                rhs[i + 1] = r - w * rhs[i + 1];
            }
        }

        // Back-substitution step.
        rhs[dim - 1] /= diag[dim - 1];

        for (std::size_t i_rev = 2; i_rev <= dim; i_rev++) {
            auto const i = dim - i_rev;
            if (i == dim - 2) {
                rhs[i] -= upper[i] * rhs[i + 1];
            } else {
                rhs[i] -= upper[i] * rhs[i + 1] + lower[i + 1] * rhs[i + 2];
            }
            rhs[i] /= diag[i];
        }
    }
}


/*
 * Cubic spline functor for interpolating one-dimensional series of values.
 */
class cubic_spline
{
    // Order of the polynomial.
    static constexpr int order = 3;

    // A spline_data defines a single piece of spline function:
    //   f(t) = sum( a_k * (t - t_0)^k , 0 <= k <= 3 ).
    // t_0 is the knot and a_0,...,a_3 are the coefficients.
    struct spline_data
    {
        double knot;
        double coefficients[order + 1];
    };

public:
    /*
     * Specifies the boundary conditions used to determine the splines.
     */
    enum class boundary_conditions
    {
        natural,
        not_a_knot,
    };

    static constexpr auto natural = boundary_conditions::natural;
    static constexpr auto not_a_knot = boundary_conditions::not_a_knot;

    /*
     * Constructs a cubic spline function that passes through given knots.
     */
    cubic_spline(
        std::vector<double> const& t,
        std::vector<double> const& x,
        boundary_conditions bc = natural
    )
    {
        make_spline(t, x, bc);
        make_bins();
    }

    /*
     * Evaluates the cubic splines at `t`.
     */
    double operator()(double t) const
    {
        spline_data const& spline = find_spline(t);

        double value = spline.coefficients[order];
        for (int i = order - 1; i >= 0; i--) {
            value *= t - spline.knot;
            value += spline.coefficients[i];
        }

        return value;
    }

private:
    /*
     * Constructs spline segments `_splines` for given set of knot points.
     */
    void make_spline(
        std::vector<double> const& knots,
        std::vector<double> const& values,
        boundary_conditions bc
    )
    {
        if (knots.size() != values.size()) {
            throw std::invalid_argument("input lengths mismatch");
        }
        if (knots.size() <= 1) {
            throw std::invalid_argument("insufficient number of knots");
        }

        auto const n_knots = knots.size();
        auto const n_segments = n_knots - 1;

        std::vector<double> intervals(n_segments);
        std::vector<double> slopes(n_segments);

        for (std::size_t i = 0; i < n_segments; i++) {
            auto const dt = knots[i + 1] - knots[i];
            auto const dx = values[i + 1] - values[i];

            if (dt <= 0) {
                throw std::invalid_argument("knots must be strictly increasing");
            }

            intervals[i] = dt;
            slopes[i] = dx / dt;
        }

        // Let M[i] be the second derivative of the i-th spline at the i-th
        // knot, i = 0,...,n where n is the number of segments. The vector M is
        // given by a tridiagonal system of equations:
        //
        // [ D[0] U[0]                      ] [ M[0]   ]   [ Y[0]   ]
        // [ L[1] D[1] U[1]                 ] [ M[1]   ]   [ Y[1]   ]
        // [      L[2] D[2] U[2]            ] [ M[2]   ]   [ Y[2]   ]
        // [      ...  ...  ...             ] [   :    ] = [   :    ]
        // [           L[n-1] D[n-1] U[n-1] ] [ M[n-1] ]   [ Y[n-1] ]
        // [                  L[n]   D[n]   ] [ M[n]   ]   [ Y[n]   ]
        //
        // We define the coefficients L, D, U and Y below and solve the
        // equations for M.

        std::vector<double> L(n_segments + 1);
        std::vector<double> D(n_segments + 1);
        std::vector<double> U(n_segments + 1);
        std::vector<double> Y(n_segments + 1);

        if (n_segments == 1) {
            // Natural (which gives a straight line) is the only sensible choice
            // if there is only one spline.
            bc = boundary_conditions::natural;
        }

        switch (bc) {
        case boundary_conditions::natural:
            // These coefficients are derived by assuming that the boundary
            // splines are linear.
            D[0] = 1;
            D[n_segments] = 1;
            break;

        case boundary_conditions::not_a_knot:
            // These coefficients are derived by assuming that the boundary
            // splines are identical to their adjacent splines.
            {
                auto const h0 = intervals[0];
                auto const h1 = intervals[1];
                auto const s0 = slopes[0];
                auto const s1 = slopes[1];
                D[0] = h0 - h1;
                U[0] = 2 * h0 + h1;
                Y[0] = -6 * h0 / (h0 + h1) * (s0 - s1);
            }
            {
                auto const h0 = intervals[n_segments - 1];
                auto const h1 = intervals[n_segments - 2];
                auto const s0 = slopes[n_segments - 1];
                auto const s1 = slopes[n_segments - 2];
                D[n_segments] = h0 - h1;
                L[n_segments] = 2 * h0 + h1;
                Y[n_segments] = 6 * h0 / (h0 + h1) * (s0 - s1);
            }
            break;
        }

        // The remaining coefficients are derived from the spline conditions.
        for (std::size_t i = 1; i < n_segments; i++) {
            L[i] = intervals[i - 1];
            D[i] = 2 * (intervals[i - 1] + intervals[i]);
            U[i] = intervals[i];
            Y[i] = 6 * (slopes[i] - slopes[i - 1]);
        }

        // Solve the equations. Solution is returned to Y.
        detail_cubic_spline::solve_tridiagonal_system(L, D, U, Y);
        auto const& M = Y;

        // Derive the polynomial coefficients of each spline segment from the
        // second derivatives `M`.
        _splines.clear();
        _splines.reserve(n_segments);

        for (std::size_t i = 0; i < n_segments; i++) {
            spline_data spline;
            spline.knot = knots[i];
            spline.coefficients[0] = values[i];
            spline.coefficients[1] = slopes[i] - (M[i + 1] + 2 * M[i]) * intervals[i] / 6;
            spline.coefficients[2] = M[i] / 2;
            spline.coefficients[3] = (M[i + 1] - M[i]) / (6 * intervals[i]);
            _splines.push_back(spline);
        }

        _splines.shrink_to_fit();

        _lower_bound = knots.front();
        _upper_bound = knots.back();
    }

    /*
     * Builds a bin-based index `_bins` that is used to quickly find a spline
     * segment covering a query point.
     */
    void make_bins()
    {
        _bin_interval = (_upper_bound - _lower_bound) / double(_splines.size());

        // We need to map uniformly-spaced bins to spline segments that may
        // not be uniformly spaced. Here `index` identifies a spline segment.
        // We iterate over bins and synchronize spline segments accordingly.
        std::size_t index = 0;

        for (int bin = 0; ; bin++) {
            auto const bin_edge = _lower_bound + _bin_interval * bin;

            while (index + 1 < _splines.size() && _splines[index + 1].knot <= bin_edge) {
                index++;
            }

            // Now, the lower edge of the bin is covered by the spline segment
            // at `index`.
            _bins.push_back(index);

            if (index + 1 == _splines.size()) {
                break;
            }
        }

        _bins.shrink_to_fit();
    }

    /*
     * Returns the spline segment that covers given point `t`.
     */
    spline_data const& find_spline(double t) const
    {
        if (t <= _lower_bound) {
            return _splines.front();
        }
        if (t >= _upper_bound) {
            return _splines.back();
        }

        std::size_t bin = std::size_t((t - _lower_bound) / _bin_interval);
        if (bin >= _bins.size()) {
            bin = _bins.size() - 1;
        }
        std::size_t index = _bins[bin];

        assert(t >= _splines[index].knot);

        for (; index + 1 < _splines.size(); index++) {
            if (t < _splines[index + 1].knot) {
                break;
            }
        }

        return _splines[index];
    }

private:
    std::vector<spline_data> _splines;
    std::vector<std::size_t> _bins;
    double _lower_bound;
    double _upper_bound;
    double _bin_interval;
};

#endif
