//
// Created by criogenox 03/2024
//
#include "../headers/matplotlibcpp.h"
#include "../src/prepro/switchcase.cpp"
#include "../src/prepro/rzero.cpp"
#include "../src/prepro/riemann_sum.cpp"
#include "../src/util/plotting.cpp"

using std::vector;
using std::string;

int main() {
    //#####################################################
    string file = "data/dr_ce_E8.dat";
    Read dat{file};
    auto r = dat.get();
    string leg = "Asymmetric Case - E8";
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
    constexpr double min = -7.1, max = 7.0, dy = 0.1;
    constexpr int c{-1};
    Linspace lino{min, max, dy};
    auto yii = lino.linspacec();
    //#####################################################
    Rzero rzeroo{r};
    auto idxz = rzeroo.rzero();
    //#####################################################
    Riemman sum{r, static_cast<int>(idxz)};
    auto s0 = sum.sum();
    //#####################################################
    constexpr double e = 1435.16, r0 = 920;
    Switchcase switchcaseo{e, r0, c, s0, yii};
    switchcaseo.switchcasec();
    auto con_eval = switchcaseo.get();
    //#####################################################
    Plot ploto{con_eval[0], con_eval[1], leg};
    ploto.plotc();
    //#####################################################
    return 0;
}
