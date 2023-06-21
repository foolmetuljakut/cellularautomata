#ifndef ca_trealisation_hpp
#define ca_trealisation_hpp

#include <ostream>
#include <functional>
#include "stats.hpp"

namespace ca {
    
    template<typename tcell> class trealisation { 
    public:
        typedef std::function<void(tcell&)> cellinit; // initializes a cell using some initial condition
        typedef std::function<void(trealisation<tcell>&, tcell&)> cellupdate; // initializes a cell using some initial condition
    private:
        size_t operations;
        cellinit initialcondition;
        cellupdate cellupdatefn;
        std::vector<tcell> cells;
        bool processingupdate;
    public:
        trealisation(size_t n, size_t operations, cellinit initialcondition, cellupdate cellupdate) :
            operations{operations}, initialcondition(initialcondition), cellupdatefn(cellupdate), cells(n), processingupdate{false} { 
            for(size_t i = 0; i < n; i++)
                initialcondition(cells[i]);
        }

        size_t newcell(bool initcond = false) {
            cells.emplace_back();
            if(initcond)
                initialcondition(cells.back());
            return cells.size() -1;
        }

        void update(bool stochastic = true) {
            if(processingupdate)
                return;

            processingupdate = true;
            if(stochastic) {
                stats::setab(0, cells.size()-1);
                for(unsigned i = 0; i < cells.size() * operations; i++) {
                    unsigned ri = stats::rint();
                    cellupdatefn(*this, cells[ri]);
                }
            } // deterministic
            else {
                for(unsigned i = 0; i < cells.size(); i++)
                    cellupdatefn(*this, cells[i]);
            }
            processingupdate = false;
        }

        std::vector<tcell>& getcells() { return cells; }

        template<typename T> friend std::ostream& operator<<(std::ostream& out, const trealisation<T>& r);
    };

    template<typename T> std::ostream& operator<<(std::ostream& out, const trealisation<T>& r) {
        unsigned i = 0;
        for(auto& cell : r.cells)
            out << (i++) << ": " << cell << "\n";
        out.flush();
        return out;
    }
}

#endif //ca_trealisation_hpp