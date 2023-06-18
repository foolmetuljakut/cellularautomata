#include "cell.hpp"

float& cell::operator[](const char *key) {
    auto k = std::string(key);
    if(states.find(k) == states.end())
        states.insert(std::make_pair(k, 0));
    return states.at(k);
}

void cell::update(unsigned& opi) {
    if(operators.size() == 0)
        return;

    if(operators.size() == 1)
        (*operators.begin())(*this);
    else
        operators[opi](*this);
}

void cell::addop(std::function<void(cell&)> op) {
    operators.push_back(op);
}

bool cell::contains(const char *state) {
    auto k = std::string(state);
    if(states.find(k) == states.end())
        return false;
    return true;
}

std::ostream& operator<<(std::ostream& out, const cell& c) {
    out << "{";
    unsigned i = 0;
    for(auto& s : c.states)
        out << s.first << ":" << s.second << (i++ == c.states.size()-1 ? "}" : ", ");
    out.flush();
    return out;
}
