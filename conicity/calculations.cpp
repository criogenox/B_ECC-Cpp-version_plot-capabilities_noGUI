//
// Created by criogenox 03/2024
//
#include <algorithm>
#include <complex>
#include <functional>
#include <numeric>

using std::back_insert_iterator;
using std::vector;
using std::function;
using std::numbers::pi;
using std::plus;

class Opc {
protected:
    function<double(double)> psif{};
    function<double(double)> xf{};
    function<double(double)> angf{};

    explicit Opc(const double &e, const double &r0, const double &i)
        : psif{
            [&e, &r0, &i](const double &s_temp) {
                return abs(sqrt((-2 / (e * r0)) * (s_temp - i)));
            }
        } {
    }

    explicit Opc(const double &dy)
        : xf{
            [&dy](const double &psi_temp) {
                return 2 * dy / (psi_temp * 1000);
            }
        } {
    }

    explicit Opc(const double &e, const double &r0)
        : angf{
            [&e, &r0](const double &lambda_temp) {
                return pow(pi / (lambda_temp * 1000), 2) * (2 * e * r0);
            }
        } {
    }
};

class Psi final : protected Opc {
public:
    Psi(const double &e, const double &r0, const double &i, const vector<double> &s)
        : Opc(e, r0, i),
          s(s) {
    }

    const vector<double> &psic(const long imin, const long imax) {
        transform(cbegin(s) + imin, cbegin(s) + imax,
                  back_insert_iterator(psi), move(psif));
        return psi;
    }

private:
    vector<double> psi;
    const vector<double> &s;
};

class Xplot final : protected Opc {
public:
    Xplot(const double &dy, const vector<double> &psi)
        : Opc(dy),
          psi(psi) {
    }

    const vector<double> &xplotc() {
        adjacent_difference(cbegin(psi), cend(psi), back_insert_iterator(psisum), plus<>{});
        transform(cbegin(psisum), cend(psisum), back_insert_iterator(x), move(xf));
        replace(begin(x), begin(x) + 1, x[0], 0.0);
        partial_sum(cbegin(x), cend(x), std::back_inserter(xplot));
        return xplot;
    }

private:
    vector<double> psisum, x, xplot;
    const vector<double> &psi;
};

class Ang final : protected Opc {
public:
    Ang(const double &e, const double &r0, const vector<double> &lambda)
        : Opc(e, r0),
          lambda(lambda) {
    }

    const vector<double> &angc() {
        transform(cbegin(lambda), cend(lambda),
                  back_inserter(ang), move(angf));
        return ang;
    }

private:
    vector<double> ang;
    const vector<double> &lambda;
};

class Calc final {
public:
    Calc(const std::vector<std::vector<double> > &yc, const std::vector<double> &s)
        : yc(yc),
          s(s) {
    }

    vector<double> calcc(const double &r0, const double &e, const double &dy) {
        for (const auto &j: yc) {
            const long imin = static_cast<int>(j[3]);
            const long imax = static_cast<int>(j[4]);

            Psi psio{e, r0, j[5], s};
            auto psi = psio.psic(imin, imax);

            Xplot xploto{dy, psi};
            auto xplot = xploto.xplotc();

            lambda.push_back(2 * xplot.back());
        }
        Ang ango{e, r0, lambda};
        const auto ang = ango.angc();
        return ang;
    }

private:
    vector<double> lambda;
    const vector<vector<double> > &yc;
    const vector<double> &s;
};
