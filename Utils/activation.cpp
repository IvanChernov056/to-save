#include "activation.h"

#include <cmath>

namespace acfn {
    using Func = std::function<double(double)>;

    ActivationFunc  genSigmoid (double i_a, double i_b) {
        Func func = [i_a, i_b](double x) {
            return 1.0 / (1.0 + exp(-(i_a*x+i_b)));
        };
        Func diff = [i_a, i_b](double x) {
            double e = exp(-(i_a*x+i_b));
            double denum = (1.0 + e)*(1.0 + e);
            return e*i_a/denum;
        };
        return ActivationFunc(func, diff);
    }

    ActivationFunc  genTanh (double i_a, double i_b) {
        Func func = [i_a, i_b](double x) {
            return tanh(i_a*x + i_b);
        };
        Func diff = [i_a, i_b](double x) {
            double e = exp(2*(i_a*x+i_b));
            double num = 4*e;
            double denum = (e+1)*(e+1);
            return i_a*num/denum;
        };

        return ActivationFunc(func, diff);
    }
}
