//
// Created by criogenox 03/2024
//
#include <span>

#include "../util/input.cpp"
#include "spline.hpp"

using std::vector;
using std::span;
using std::back_insert_iterator;

class Sfunc {
protected:
    Sfunc() = default;

    vector<double> s;
    const Linspace lino{-6.8, 6.8, 0.01};
    vector<double> yi = lino.linspacec();

    void sfuncc(cubic_spline &spline) {
        transform(cbegin(yi), cend(yi), back_insert_iterator(s),
                  [&spline](const double &yi_i) {
                      return spline(yi_i);
                  });
    }
};

class Pre_Asymm : protected Sfunc {
protected:
    Pre_Asymm(const vector<double> &s0, const vector<double> &yii) : Sfunc(), s0(s0), yii(yii) {
    }

    long preasymc() {
        sfuncc(spline);
        const long idmins = distance(cbegin(s), std::ranges::
                                     min_element(cbegin(s), cend(s)));
        return idmins;
    }

private:
    const vector<double> &s0, &yii;
    cubic_spline spline = cubic_spline(yii, s0);
};

class Matrixa : protected Pre_Asymm {
protected:
    Matrixa(const vector<double> &s0, const vector<double> &yii)
        : Pre_Asymm(s0, yii) {
    }

    void matrixc() {
        const auto idmins = preasymc();
        const span<double> sin{begin(s), begin(s) + idmins};
        const span<double> sip{begin(s) + idmins, end(s)};
        int it{0};
        for (const auto si: sin) {
            vector<double> sipc;
            transform(cbegin(sip), cend(sip), back_insert_iterator(sipc),
                      [&si](const double sip_i) {
                          return std::abs(sip_i - si);
                      });
            const long idxsipc = distance(cbegin(sipc), min_element(cbegin(sipc),
                                                                    cend(sipc)));
            yemm.push_back(vector<double>{
                (yi[idmins + idxsipc] - yi[it]) / 2,
                yi[it],
                yi[idmins + idxsipc],
                static_cast<double>(it),
                static_cast<double>(idmins + idxsipc),
                si
            });
            it += 1;
        }
    }

    vector<vector<double> > yemm;
};

class Asym : protected Matrixa {
public:
    Asym(const vector<double> &s0, const vector<double> &yii)
        : Matrixa(s0, yii) {
    }

    void asymc() {
        matrixc();
    }

    const vector<vector<double> > &get_y() {
        return yemm;
    }

    const vector<double> &get_s() {
        return s;
    }
};
