//
// Created by criogenox 03/2024
//
#include <algorithm>
#include <functional>

using std::vector;
using std::ranges::min_element;
using std::function;
using std::ranges::find;

class Opz {
protected:
    function<bool(double, double)> comp{};

    explicit Opz()
        : comp{
            [](const double i1, const double i2) {
                return i1 >= 0 && (i2 < 0 || i1 < i2);
            }
        } {
    }
};

class Rzero : protected Opz {
public:
    explicit Rzero(const vector<double> &r)
        : r(r) {
    }

    [[nodiscard]] auto rzero() const {
        const double rz = *min_element(cbegin(r), cend(r), comp);
        //********
        const auto &idxz = distance(cbegin(r), find(cbegin(r), cend(r), rz));
        return idxz;
    }

private:
    const vector<double> &r;
};
