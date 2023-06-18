#include "realisation.hpp"

void realisation::create(unsigned n, 
        std::initializer_list<std::string> states,
        std::initializer_list<std::function<void(cell&)> > operators) {

    std::vector<const char*> cstates;
    cstates.reserve(states.size());
    for(auto& s: states)
        cstates.push_back(s.c_str());

    for(unsigned i = 0; i < n; i++) {
        cells.emplace_back();
        for(auto& state : cstates)
            cells.back()[state];
        for(auto& op : operators)
            cells.back().addop(op);
    }
}

void realisation::update() {
    stats::setab(0, cells.size()-1);
    for(unsigned i = 0; i < cells.size(); i++) {
        unsigned ri = stats::rint();
        unsigned opi = cells[ri].coperators() == 1 ? 
                            0 : stats::rint() % cells[ri].coperators();
        cells[ri].update(opi);
    }
}

std::ostream& operator<<(std::ostream& out, const realisation& c) {
    unsigned i = 0;
    for(auto& s : c.cells)
        out << (i++) << ": " << s << "\n";
    out.flush();
    return out;
}

