//
// Created by criogenox 03/2024
//
#include <algorithm>
#include <fstream>
#include <functional>
#include <sstream>

using std::ranges::generate;
using std::vector;
using std::function;
using std::string;
using std::ifstream;

class Read {
public:
    explicit Read(const string &dr) : in(dr) {
        read_row(in);
    }

    void read_row(ifstream &in) {
        double fc;
        while (getline(in, row)) {
            std::istringstream lineStream(row);
            lineStream >> fc;
            r.push_back(fc);
        }
    }

    const vector<double> &get() {
        return r;
    }

private:
    vector<double> r;
    string row;
    ifstream in;
};

class Opl {
protected:
    function<double()> linf;

    explicit Opl(const long double &min, const double &dy)
        : linf{
            [&min, inc = 0, &dy]() mutable {
                return min + (dy * inc++);
            }
        } {
    }
};

class Linspace : protected Opl {
public:
    Linspace(const long double &min, const long double &max, const double &dy)
        : Opl(min, dy),
          min(min),
          max(max),
          dy(dy) {
    }

    [[nodiscard]] vector<double> linspacec() const {
        const int len = static_cast<int>(1 + (max - min) / dy);
        vector<double> yii(len + 1);
        generate(begin(yii), end(yii), linf);
        return yii;
    };

private:
    const long double &min, &max;
    const double &dy;
};
