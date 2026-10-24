#include <fde/core/types.hpp>


namespace fde{

    Sequence uniform_timegrid(
        double t0, double T, std::size_t N
    ){
        Sequence time_grid(N+1);
        const double dt = (T-t0) / static_cast<double>(N);
        for (std::size_t i=0; i<=N; i++)
            time_grid[i] = t0 + dt*static_cast<double>(i);
        return time_grid;
    }

    Sequence System::uniform_timegrid() const{
        return fde::uniform_timegrid(0.0, T, N);
    }    
    
    double System::dt() const{
        return T/N;
    }
}
