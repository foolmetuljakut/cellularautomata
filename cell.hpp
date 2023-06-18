#ifndef ca_cell_hpp
#define ca_cell_hpp

#include <iostream>
#include <map>
#include <vector>
#include <functional>

#include "stats.hpp"

class cell {
    std::map<std::string, float> states;
    std::vector<std::function<void(cell&)> > operators;
public:
    // cell() { std::cout << "ctor" << std::endl; }
    // cell(const cell& other) : states(other.states), operators(other.operators) { std::cout << "cpy ctor" << std::endl; }
    float& operator[](const char *key);
    void addop(std::function<void(cell&)> op);
    void update(unsigned& opi);
    void update() { unsigned i = 0; update(i); }
    bool contains(const char *state);
    unsigned cstates() { return states.size(); }
    unsigned coperators() { return operators.size(); }

    friend std::ostream& operator<<(std::ostream& out, const cell& c);
};

std::ostream& operator<<(std::ostream& out, const cell& c);

#endif // ca_cell_hpp