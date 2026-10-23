#include <fde/core/types.hpp>

fde::Sequence fde::uniform_timegrid(
    double t0, double T, std::size_t N
){
    fde::Sequence time_grid(N+1);
    double dt = (T-t0) / static_cast<double>(N);
    for (std::size_t i=0; i<=N; i++)
        time_grid[i] = t0 + dt*static_cast<double>(i);
    return time_grid;
}