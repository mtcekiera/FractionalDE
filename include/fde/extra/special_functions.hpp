#pragma once
#include <fde/core/types.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions/gamma.hpp>

namespace fde{
    using High_precision = boost::multiprecision::cpp_dec_float_50;
    // operates on high precision but returns double for simplicity
    double mittag_leffler_explicit(
        double z,
        double alpha,
        std::size_t terms = 50
    ){
        if (alpha<=0)
            throw std::invalid_argument("alpha must be positive");
        const High_precision z_prec = static_cast<High_precision>(z);
        const High_precision alpha_prec = static_cast<High_precision>(alpha);
      
        High_precision sum = 0.0;
        High_precision zk = 1.0;

        for (std::size_t k=0; k<=terms; k++){
            High_precision sk = 
                zk / boost::math::tgamma(static_cast<High_precision>(k)*alpha + 1);
            sum += sk;

            zk *= z;
        }

        return static_cast<double>(sum);
    };

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

