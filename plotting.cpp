//
// Created by emilio on 28/02/24.
//
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using std::vector;
using std::string;

class Plot {
public:
    Plot(const vector<double> &con_x, const vector<double> &con_y, const string &leg)
        : con_x(con_x),
          con_y(con_y),
          leg(leg) {
    }

    void plotc() const {
        //##############################################################
        // # Set the size of output image to 1200x780 pixels
        // plt::figure_size(1200, 780);
        // -------------------------------------------------------------
        // # Plot line from given x and y data.
        // # Color is selected automatically o manually (red asterics).
        // # Plot a line whose name will show up as "leg" in the legend.
        plt::named_plot(leg, con_x, con_y, "r*");
        // -------------------------------------------------------------
        // # Enable legend.
        plt::legend();
        // -------------------------------------------------------------
        // # Set x-axis to interval [0,10], or let it in auto mode.
        // plt::xlim(0, 10);
        // -------------------------------------------------------------
        // # Add graph title
        plt::title("Equivalent Conicity Calculation "
            "(according to the EN15302 non-linear method)");
        // -------------------------------------------------------------
        // # Add axes label names
        plt::xlabel("Lateral displacement [mm]");
        plt::ylabel("Nominal Equivalent conicity [tan(λ)]");
        // -------------------------------------------------------------
        // # Save the image (file format is determined by the extension)
        // plt::save("./plot.png");
        // -------------------------------------------------------------
        // # Enable grid reference lines
        plt::grid(true);
        // -------------------------------------------------------------
        plt::show();
        //##############################################################
    }

private:
    const vector<double> &con_x, &con_y;
    const string &leg;
};
