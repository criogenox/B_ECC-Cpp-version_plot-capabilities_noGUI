//
// Created by criogenox 03/2024
//
#include <algorithm>
#include <complex>
#include <functional>

using std::ranges::min_element;
using std::vector;
using std::function;
using std::back_insert_iterator;

class Opo {
protected:
    function<double(vector<double>)> yone;

    explicit Opo()
        : yone{
            [](const vector<double> &yemm_i) {
                return std::abs(yemm_i[0] - 1);
            }
        } {
    }
};

class Ycmm : protected Opo {
public:
    explicit Ycmm(vector<vector<double> > &yemm)
        : yemm(yemm) {
    }

    vector<vector<double> > &ycmmc() {
        transform(cbegin(yemm), cend(yemm), back_insert_iterator(yemmone), move(yone));
        const long idone = distance(begin(yemmone), min_element(yemmone));
        reverse_copy(cbegin(yemm), cbegin(yemm) + idone + 1, back_insert_iterator(yc));
        return yc;
    }

private:
    vector<vector<double> > &yemm, yc;
    vector<double> yemmone;
};