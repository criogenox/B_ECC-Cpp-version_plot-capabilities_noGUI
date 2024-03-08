//
// Created by criogenox 03/2024
//
#include <algorithm>
#include <functional>
#include <numeric>

using std::ranges::transform;
using std::vector;

class Op {
protected:
    std::function<double(double)> prod{};

    explicit Op(const double &sg)
        : prod{
            [sg](const double element) {
                return element * sg * 0.1;
            }
        } {
    }
};

class Mul : protected Op {
protected:
    explicit Mul(vector<double> &r, const int &z, const double &sg) : Op(sg), r(r), z(z) {
    }

    void transf(const int &ixa, const int &idx,
                const vector<double>::iterator &arg2) {
        transform(cbegin(r) + ixa, arg2 + idx, begin(s), move(prod));
    }

    vector<double> &r;
    const int z{};
    vector<double> s = vector<double>(z);
};

class Left : protected Mul {
protected:
    explicit Left(vector<double> &r, const int &z)
        : Mul(r, z, -1) {
    }

    void sleft() {
        transf(0, z, r.begin());
        inclusive_scan(rbegin(s), rend(s), rbegin(s0));
    }

    vector<double> s0 = vector<double>(z);
};

class Right : protected Mul {
protected:
    explicit Right(vector<double> &r, const int &z)
        : Mul(r, (static_cast<int>(size(r)) - z), 1), ixa(z) {
    }

    void sright() {
        transf(ixa, 0, end(r));
        inclusive_scan(begin(s), end(s), begin(s0r));
    }

    const int ixa;
    vector<double> s0r = vector<double>(z);
};

class Riemman : virtual protected Left, virtual protected Right {
public:
    explicit Riemman(vector<double> &r, const int &z) : Left(r, z), Right(r, z) {
    }

    const vector<double> &sum() {
        sright();
        sleft();
        s0.reserve(size(s0) + size(s0r));
        s0.insert(cend(s0), cbegin(s0r), cend(s0r));
        return s0;
    }
};
