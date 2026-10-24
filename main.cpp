#include <iostream>
#include <fde/extra/special_functions.hpp>
#include <fde/core/types.hpp>
#include <tests/test_ml.hpp>
#include <tests/test_gl.hpp>
#include <vector>



int main(){
    // const double dz = 0.01;
    // const double z0 = -1.0;
    // const double zEnd = 0.0;

    // std::vector<double> alphas_05_1 = {0.5, 1.0};
    // test_ml("./data/ML_05_1.csv", alphas_05_1);
    // fde::Sequence time = fde::uniform_timegrid(1.0, 5.0, 10);
    // for (std::size_t i=0; i<time.size(); i++)
    //     std::cout<<time[i]<<' ';
    // std::cout<<"\n";


    test_gl("data/gl_test.csv");

    // std::cout<<fde::mittag_leffler_gerrappa(-1.0, 1.0, 2.0, 0.1, 120);
    return 0;
}