#ifndef ca_tcell_hpp
#define ca_tcell_hpp

#include <functional>

namespace ca {
    class tcell;
    typedef std::function<void(tcell&)> cellinitializer;

    class tcell {

    public:
        tcell()
    };
}

#endif //ca_tcell_hpp