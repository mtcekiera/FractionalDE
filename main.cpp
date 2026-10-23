#include <iostream>
#include <fde/extra/special_functions.hpp>
#include <fde/core/types.hpp>
#include <vector>

int main(){
    fde::Sequence time = fde::uniform_timegrid(1.0, 5.0, 10);
    for (std::size_t i=0; i<time.size(); i++)
        std::cout<<time[i]<<' ';
    std::cout<<"\n";
    // std::cout<<fde::mittag_leffler_gerrappa(-1.0, 1.0, 2.0, 0.1, 120);
    return 0;
}