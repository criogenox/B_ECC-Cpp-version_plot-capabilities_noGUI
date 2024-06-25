//
// Created by criogenox 03/2024
//
#include "../conicity/asym.cpp"
#include "../conicity/calculations.cpp"
#include "../conicity/sym.cpp"
#include "../postpro/one_matrix.cpp"

using std::vector;

class Ymm {
protected:
    Ymm(const double &e, const double &r0)
        : e(e),
          r0(r0) {
    }

    const vector<vector<double> > &ymmc(vector<vector<double> > &yemm, const vector<double> &s,
                                        const double &step) {
        const auto yc = Ycmm(yemm).ycmmc();
        //
        const auto eqcon = Calc(yc, s).calcc(r0, e, step);
        //
        vector<double> yc_plot;
        for (const auto &i: yc) {
            yc_plot.push_back(i[0]);
        }
        feq.push_back(yc_plot);
        feq.push_back(eqcon);

        return feq;
    }

    vector<vector<double> > feq;
    const double &e, &r0;
};

class Switchcase : protected Ymm {
public:
    Switchcase(const double &e, const double &r0, const int &c,
               const vector<double> &s0, const vector<double> &yii)
        : Ymm(e, r0),
          c(c),
          s0(s0),
          yii(yii) {
    }

    Switchcase &switchcasec() {
        switch (c) {
            case -1: {
                Asym asymo{s0, yii};
                auto yemm = asymo.asymc().get_y();
                const auto s = asymo.get_s();
                ymmc(yemm, s, 0.01);
            }
            break;
            //
            case 1: {
                const unsigned long idxf = size(yii) - 1;
                auto yemm = Sym(s0, yii).symc(1, idxf).get();
                ymmc(yemm, s0, 0.1);
            }
            break;
            //
            default: assert("you're in big troubles!!!");
        }
        return *this;
    }

    const vector<vector<double> > &get() {
        return feq;
    }

private:
    const int &c;
    const vector<double> &s0, &yii;
};
