//
// Created by criogenox 03/2024
//
#include <vector>

using std::vector;

class Matrix {
protected:
    Matrix(const vector<double> &s0, const vector<double> &yii): s0(s0), yii(yii) {
    }

    void sfunction(const int &cas, const unsigned long idxf, const unsigned long nl) {
        int it{0};
        for (const auto si: s0) {
            yemm.push_back(vector<double>{
                (yii[idxf - it] - yii[it]) / 2,
                yii[it],
                yii[idxf - it],
                static_cast<double>(it),
                static_cast<double>(idxf - it),
                si
            });

            it += 1;
            if (cas > 0 && static_cast<bool>(s0[nl])) {
                break;
            }
        }
    }

    vector<vector<double> > yemm;

private:
    const vector<double> &s0, &yii;
};

class Sym : protected Matrix {
public:
    Sym(const vector<double> &s0, const vector<double> &yii)
        : Matrix(s0, yii),
          nl((size(yii) % 2 == 0) ? size(yii) / 2 : (size(yii) - 1) / 2) {
    }

    Sym &symc(const int &cas, const unsigned long idxf) {
        sfunction(cas, idxf, nl);
        return *this;
    }

    vector<vector<double> > &get() {
        return yemm;
    }

private:
    unsigned long nl;
};
