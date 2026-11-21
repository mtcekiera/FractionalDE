#pragma once

#include <fde/extra/special_functions.hpp>

#include <boost/multiprecision/cpp_dec_float.hpp>
// #include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>


void P11_extra2(
    double t0 = 0.0,
    double T = 5.0,
    double dt = 0.01,
    double lambda = 1.0,
    std::vector<double> alphas = {0.3, 0.5, 0.8, 1.0},
    std::size_t max_terms = 200
){
    namespace fs = std::filesystem;
    using Real = boost::multiprecision::cpp_dec_float_100;

    fs::path data_dir = fs::current_path() / "data/P11_extra2";
    fs::create_directories(data_dir);

    std::ofstream file(data_dir / "P11_extra2.csv");

    file << std::setprecision(
        std::numeric_limits<double>::max_digits10
    );

    const std::size_t steps = static_cast<std::size_t>(
        std::ceil((T - t0) / dt)
    );

    const std::size_t n_alpha = alphas.size();
    const std::size_t n_points = steps + 1;

    // Time grid and arguments
    std::vector<double> z_double(n_points);
    std::vector<Real> z(n_points);

    for (std::size_t i = 0; i < n_points; ++i) {
        double t = std::min(t0 + i * dt, T);

        z_double[i] = -lambda * t;
        z[i] = Real(z_double[i]);
    }

    // Garrappa reference
    std::vector<std::vector<double>> reference(
        n_alpha, std::vector<double>(n_points)
    );

    for (std::size_t a = 0; a < n_alpha; ++a) {
        for (std::size_t i = 0; i < n_points; ++i) {
            reference[a][i] = fde::mittag_leffler_gerrappa(
                z_double[i],
                alphas[a],
                p11_extra::MU,
                p11_extra::H,
                p11_extra::N
            );
        }
    }

    // Running sums, initially S_0 = 0
    std::vector<std::vector<Real>> sums(
        n_alpha, std::vector<Real>(n_points, Real(0))
    );

    // Powers z^k, initially z^0 = 1
    std::vector<std::vector<Real>> powers(
        n_alpha, std::vector<Real>(n_points, Real(1))
    );

    // CSV header
    file << "terms";

    for (double alpha : alphas) {
        const int a = static_cast<int>(std::lround(10 * alpha));

        file << ",final_error_a" << a
             << ",max_error_a" << a;
    }

    file << '\n';

    // Incrementally calculate the explicit series
    for (std::size_t k = 0; k < max_terms; ++k) {

        file << k + 1;

        for (std::size_t a = 0; a < n_alpha; ++a) {

            const Real alpha = Real(alphas[a]);

            // Calculated once per alpha and term index
            const Real gamma_inv = Real(1) /
                boost::math::tgamma(alpha * Real(k) + 1);

            double final_error = 0.0;
            double max_error = 0.0;

            for (std::size_t i = 0; i < n_points; ++i) {

                // Add only the new term
                sums[a][i] += powers[a][i] * gamma_inv;

                // Prepare the next power
                powers[a][i] *= z[i];

                // Compare at double precision
                const double explicit_val =
                    static_cast<double>(sums[a][i]);

                const double error = std::abs(
                    explicit_val - reference[a][i]
                );

                max_error = std::max(max_error, error);

                if (i == steps)
                    final_error = error;
            }

            file << "," << final_error
                 << "," << max_error;
        }

        file << '\n';
    }
}