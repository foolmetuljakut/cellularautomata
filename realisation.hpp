#ifndef ca_realisation_hpp
#define ca_realisation_hpp

#include <vector>
#include <functional>

#include "stats.hpp"
#include "cell.hpp"

class realisation {
public:
    std::vector<cell> cells;
    void create(unsigned n, 
        std::initializer_list<std::string> states,
        std::initializer_list<std::function<void(cell&)> > operators);
    void update();

    friend std::ostream& operator<<(std::ostream& out, const realisation& c);
};

std::ostream& operator<<(std::ostream& out, const realisation& c);

#endif //ca_realisation_hpp