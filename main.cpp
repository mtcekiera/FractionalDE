#include <iostream>
#include <fde/fde.hpp>



int main(){

    // P11();       // basic Mittag-Leffler, z domain (z = -lambda*t), z = [-1, 0]; alpha = 0.5, 1.0
    // P11_extra();    // Mittag-Leffler extra plots, time domain t in [0,5]; alpha = 0.3, 0.5, 0.8, 1.0
    P11_extra2();    // ML Gerrappa algorithm vs explicit series calculation
    // P12();
    // P13();   // time step h vs error
    // P14();
 
    return 0;
}