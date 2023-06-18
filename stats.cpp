#include "stats.hpp"

std::random_device stats::rd;
std::mt19937 stats::gen(stats::rd());
std::uniform_int_distribution<unsigned> stats::rndint(0, 1);
std::uniform_real_distribution<float> stats::rndfloat(0, 1);

void stats::setab(unsigned a, unsigned b) {
    stats::rndint = std::uniform_int_distribution<unsigned>(a, b);
    // stats::rndfloat = std::uniform_real_distribution<float>(a, b);
}

unsigned stats::rint() {
    return stats::rndint(gen);
}

float stats::rfloat() {
    return stats::rndfloat(gen);
}

