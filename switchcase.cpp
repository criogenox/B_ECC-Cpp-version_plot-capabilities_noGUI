//
// Created by criogenox 03/2024
//
#include "calculations.cpp"
#include "one_matrix.cpp"
#include "sym.cpp"
#include "asym.cpp"

using std::vector;

class Ymm {
protected:
    Ymm(const double &e, const double &r0)
        : e(e),
          r0(r0) {
    }

    vector<vector<double> > &ymmc(vector<vector<double> > &yemm, const vector<double> &s,
                                  const double &step) {
        Ycmm ycmmo{yemm};
        const auto yc = ycmmo.ycmmc();
        //
        Calc calco{yc, s};
        const auto eqcon = calco.calcc(r0, e, step);
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

    void switchcasec() {
        switch (c) {
            case -1: {
                Asym asymo{s0, yii};
                asymo.asymc();
                auto yemm = asymo.get_y();
                const auto s = asymo.get_s();
                ymmc(yemm, s, 0.01);
            }
            break;
            //
            case 1: {
                Sym symo{s0, yii};
                const unsigned long idxf = size(yii) - 1;
                symo.symc(1, idxf);
                auto yemm = symo.get();
                ymmc(yemm, s0, 0.1);
            }
            break;
            //
            default: assert("you're in big troubles!!!");
        }
    }

    vector<vector<double> > &get() {
        return feq;
    }

private:
    const int &c;
    const vector<double> &s0, &yii;
};
