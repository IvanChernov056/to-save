#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

#include <functional>

class ActivationFunc {
    using Func = std::function<double(double)>;

    public:

        explicit ActivationFunc (Func i_func = nullptr, Func i_diff = nullptr) :
            d_func(i_func), d_diff(i_diff) 
        {
            if (!d_diff && d_func) {
                d_diff = [this] (double x) ->double {
                    double h = 1e-3;
                    return (this->d_func(x - h) - this->d_func(x + h))/(2*h); 
                };
            }
        }


        operator Func () {
            return d_func;
        }
        operator bool () {
            return d_func != nullptr;
        }
        double operator()(double x) {
            if(d_func)
                return d_func(x);
            return x;
        }

        Func diff() {
            return d_diff;
        }
        bool isDifferentiable() {
            return d_diff != nullptr;
        }

    private:

        Func    d_func{nullptr};
        Func    d_diff{nullptr};
};

namespace acfn {
    ActivationFunc  genSigmoid (double i_a = 1.0, double i_b = 1.0);
    ActivationFunc  genTanh (double i_a = 1.0, double i_b = 1.0);
}
#endif