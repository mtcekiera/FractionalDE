#pragma once
#include <fde/core/types.hpp>

namespace fde{

double mittag_leffler_gerrappa(
    double z, 
    double alpha, 
    double mu, 
    double h, 
    int N
){
    // invalid argument controls
    // +-inf and NaN control
    if (!std::isfinite(alpha) or alpha<0.0 or alpha>1.0)
        throw std::invalid_argument("Require 0<alpha<1");

    if (!std::isfinite(z) or z>0.0)
        throw std::invalid_argument("Require finite z<0");

    if (mu <= 0.0 or h <= 0.0 or N <= 0)
        throw std::invalid_argument("Invalid contour parameters");

    if (z == 0.0)
        return 1.0;

    Complex sum = 0.0;

    for (int k=-N; k<=N; k++){
        const double u = k*h;

        // w = (1 + iu)
        const Complex w = {1.0, u};

        // s = mu*(1+iu)^2
        const Complex s = mu * w * w;

        // Laplace transform
        const Complex F = std::exp(s) * std::pow(s, alpha-1.0) 
            / (std::pow(s, alpha) - z);
        sum += w * F;
    }
    return (mu*h/pi) * sum.real();   
}
}

