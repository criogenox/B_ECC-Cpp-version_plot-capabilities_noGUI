//
// Created by criogenox 03/2024
//
#include <queue>

#include "matplotlibcpp.h"
#include "prepro/switchcase.cpp"
#include "prepro/rzero.cpp"
#include "prepro/riemann_sum.cpp"
#include "util/plotting.cpp"

using std::vector;
using std::string;

int main() {
    //#####################################################
    const string file = "data/dr_ce_E6.dat";
    auto r = Read(file).get();
    const string leg = "Asymmetric Case - E6";
    // #***************************************************
    // type=c; % (-1) asymmetric case - 5 / 6 / 7 / 8 / 9
    //         %  (1)  symmetric case - 1 / 2 / 3 / 4
    // #***************************************************
    // #***************************************************
    // ## yii=(-6.8:0.1:6.8); % 1 / 2 / 3 / 5 / 9  || n=137
    // ## yii=(-7.1:0.1:7.1); % 4                  || n=143
    // ## yii=(-6.9:0.1:6.8); % 6                  || n=138
    // ## yii=(-6.8:0.1:6.7); % 7                  || n=136
    // ## yii=(-7.1:0.1:7.0); % 8                  || n=142
    // #***************************************************
    constexpr double min = -6.9, max = 6.8, dy = 0.1;
    constexpr int c{-1};
    const auto yii = Linspace(min, max, dy).linspacec();
    //#####################################################
    const auto idxz = Rzero(r).rzero();
    //#####################################################
    const auto s0 = Riemman(r, static_cast<int>(idxz)).sum();
    //#####################################################
    constexpr double e = 1435.16, r0 = 920;
    Switchcase switchcaseo{e, r0, c, s0, yii};
    const vector<vector<double> > &con_eval = switchcaseo.switchcasec().get();
    //#####################################################
    Plot(con_eval[0], con_eval[1], leg).plotc();
    //#####################################################
    return 0;
}
