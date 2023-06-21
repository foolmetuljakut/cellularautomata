#include <iostream>
#include <fstream>
#include <sstream>

#include "stats.hpp"
#include "cell.hpp"
#include "realisation.hpp"
#include "bmp.hpp"

#include "trealisation.hpp"

#include <vector>
#include <array>
#include <random>
#include <algorithm>

void updatecelltest1() {
    srand(time(NULL));

    cell c;
    c["a"] = 0;
    c["b"] = 0;
    c["c"] = 0;
    
    c.addop([](cell& c) {
        c["a"] += 1;
    });
    c.addop([](cell& c) {
        c["b"] += 1;
    });
    c.addop([](cell& c) {
        c["c"] += 1;
    });

    for(unsigned i = 0; i < 100000; i++) {
        unsigned opi = stats::rint() % 3;
        c.update(opi);
    }
    std::cout << "c[a] = " << c["a"] << ", "
                << "c[b] = " << c["b"] << ", "
                << "c[c] = " << c["c"] << std::endl;
}

void updatecelltest2() {
    const float L = 1;
    const float dt = 0.1;
    const float r = 0.5;

    cell p;
    p["x"] = 0;
    p["v"] = 1;

    p.addop([L, dt, r](cell& c) { // move
        if(c["x"] >= L - 1e-7f)
            c["v"] = -1;
        if(c["x"] <= 1e-7f)
            c["v"] = 1;

        if(stats::rfloat() < r)
            c["x"] += c["v"] * dt;
    });

    for(unsigned i = 0; i < 40; i++) {
        std::cout << "x = " << p["x"] << ", v = " << p["v"] << std::endl;
        p.update();
    }
    std::cout << "x = " << p["x"] << ", v = " << p["v"] << std::endl;
}

void statsdistrotest1() {
    std::vector<float> samples;
    stats::setab(1, 6);
    for(unsigned i = 0; i < 100000; i++)
        samples.push_back(stats::rint());
    
    std::array<float, 7> bounds;
    for(float f = 0.5f; f <= 6.5f; f += 1)
        bounds[(unsigned)f] = f;
    
    auto histogram = stats::bin<6>(samples, bounds);
    for(unsigned i = 0; i < histogram.size(); i++)
        std::cout << "[" << i << "] = " << histogram[i] << std::endl;
    
    auto discdistro = stats::normalize<6>(histogram, bounds);
    for(unsigned i = 0; i < discdistro.size(); i++)
        std::cout << "[" << i << "] = " << discdistro[i] << std::endl;
    
    discdistro = stats::normalize<6>(histogram, bounds, false);
    for(unsigned i = 0; i < discdistro.size(); i++)
        std::cout << "[" << i << "] = " << discdistro[i] << std::endl;
}

void statsdistrotest2() {
    std::vector<float> samples;
    for(unsigned i = 0; i < 100000; i++)
        samples.push_back(stats::rfloat());
    
    std::array<float, 11> bounds;
    for(unsigned i = 0; i < bounds.size(); i++)
        bounds[i] = i * 0.1f;
    
    auto histogram = stats::bin<10>(samples, bounds);
    auto discdistro = stats::normalize<10>(histogram, bounds, false);
    for(unsigned i = 0; i < discdistro.size(); i++)
        std::cout << "[" << i << "] = " << discdistro[i] << std::endl;
}

void statsfilterselecttest() { 
    unsigned N = 3;

    std::vector<cell> cells;
    cells.reserve(N);
    
    for(unsigned i = 0; i < N; i++){
        cells.emplace_back();
        std::string key(1, (char)(i+(unsigned)'a'));
        cells.back()[key.c_str()] = i;
    }

    std::vector<cell> cells_with_a = stats::filter<cell>(cells, [](cell& c) { return c.contains("a"); });

    for(auto& c : cells_with_a)
        std::cout << c << std::endl;
    
    std::vector<float> as = stats::project<float, cell>(cells_with_a, [](cell &c) { return c["a"]; });

    for(auto& a : as)
        std::cout << a << std::endl;
}

void statsnormtest() {
    auto n = stats::range<10>();
    auto x = stats::range<11>(0, 10);
    auto y = stats::map<float, 10>(n, [](float& z){ return 1 / (1 + z*z); });
    float i = stats::integr<10>(y, x);
    auto yn = stats::normalize<10>(y, x);
    float i2 = stats::integr<10>(yn, x);

    std::cout << i << ", " << i2 << std::endl;
}

void statsmeanstdtest() {
    auto n = stats::range<10>();
    auto x = stats::range<11>(0, 10);
    auto y = stats::map<float, 10>(n, [](float& z){ return exp(-z/2); });
    float i = stats::integr<10>(y, x);
    
    float m = stats::mean<10>(x, y, i);
    float s = stats::std<10>(x, y, m, i); // failed

    std::cout << m << ", " << s << std::endl;
}

void realisationtest() {

    realisation r;
    r.create(1000, {"updates"}, {[](cell& c){ c["updates"] += 1; }});
    for(unsigned i = 0; i < 10000; i++)
        r.update();
    
    auto updates = stats::project<float, cell>(r.cells, [](cell& c){ return c["updates"]; });
    auto bounds = stats::range<21>(9900, 10100);

    auto histogram = stats::bin<20>(updates, bounds);

    for(unsigned i = 0; i < 20; i++)
        std::cout << (bounds[i+1] + bounds[i])/2 << ": " << histogram[i] << std::endl; 
    
    float min = stats::min<20>(histogram), mean = stats::mean<20>(histogram), max = stats::max<20>(histogram);
    std::cout << mean << " (-" << mean-min << "/+" << max-mean << ")" << std::endl;
}

struct trader {
    bool buyer; // true: buyer, false: seller
    size_t price, trades, id;
};

void pricesimtest1() {

    /*setup: 
        cells represent buyers and sellers 
            a cell contains a price level to interact with others at
            a cell contains a function to adjust the price
                if buyers can't be found => lower prices by a constant
                if sellers can't be found => increase prices by a constant
        measure global price distribution*/

    size_t A = 50;
    const size_t N = 10;
    for(size_t a = 1; a < A; a++) 
        std::cout << ".";
    std::cout << std::endl;

    for(size_t a = 1; a < A; a++) {
        size_t i = 0;
        ca::trealisation<trader> r(A, 1, 
            [&i](trader& t) { t.price = t.id = i++; },
            [](ca::trealisation<trader>& r, trader& t) {
                if(t.buyer)
                    t.price += t.trades == 0 ? 1 : -1;  
                else // seller
                    t.price += t.trades == 0 ? -1 : 1;  
            }
        );

        std::vector<float> ttrack, id0track, idmaxtrack;
        std::vector<std::array<float, N>> histotrack, bhtrack, shtrack;
        ttrack.push_back(0);
        id0track.push_back(r.getcells().front().price);
        idmaxtrack.push_back(r.getcells().back().price);

        // "manual" update function
        for(size_t t = 0; t < 100; t++) {

            // reset everything
            for(auto& t : r.getcells()) 
                t.trades = 0;

            // start next round of negotiations
            for(auto& t : r.getcells()) {
                float pricelvl = t.price;
                bool isbuyer = t.buyer;
                auto matches = stats::filter<trader>(r.getcells(),
                [pricelvl, isbuyer](trader& partner) {
                    if(isbuyer)
                        return (!partner.buyer) && pricelvl >= partner.price;
                    else
                        return partner.buyer && pricelvl <= partner.price;

                    // return ((partner.contains("seller") && isbuyer) || 
                    //             (partner.contains("buyer") && !isbuyer)) &&
                    //         abs(partner["price"] - pricelvl) < 1;
                });

                size_t trades = matches.size();
                if( trades > 0) {
                    t.trades += trades;
                    for(auto& partner : matches)
                        partner.trades += 1;
                }
            }

            r.update(false);

            ttrack.push_back(t+1);
            id0track.push_back(r.getcells().front().price);
            idmaxtrack.push_back(r.getcells().back().price);
            
            auto buyers = stats::filter<trader>(r.getcells(), [](trader& c) { return c.buyer; });
            auto sellers = stats::filter<trader>(r.getcells(), [](trader& c) { return !c.buyer; });
            auto prices = stats::project<float, trader>(r.getcells(), [](trader& c) { return c.price; });
            auto bprices = stats::project<float, trader>(buyers, [](trader& c) { return c.price; });
            auto sprices = stats::project<float, trader>(sellers, [](trader& c) { return c.price; });

            auto binbounds = stats::range<N+1>(0, r.getcells().size());

            if(a == 1 && t == 0) {
                auto bounds = std::ofstream((std::stringstream() << "data/bounds.csv").str());
                for(size_t i = 0; i < N; i++)
                    bounds << binbounds[i] << ", ";
                bounds << std::endl;
            }

            histotrack.push_back(stats::bin<N>(prices, binbounds));
            bhtrack.push_back(stats::bin<N>(bprices, binbounds));
            shtrack.push_back(stats::bin<N>(sprices, binbounds));
        }
        auto idtrack = std::ofstream((std::stringstream() << "data/idtracka" << a << ".csv").str());
        for(size_t n = 0; n < id0track.size(); n++)
            idtrack << ttrack[n] << ", "
                    << id0track[n] << ", "
                    << idmaxtrack[n] << "\n";

        auto htrack = std::ofstream((std::stringstream() << "data/htracka" << a << ".csv").str());
        for(size_t n = 0; n < histotrack.size(); n++) {
            htrack << n << ", ";
            for(size_t i = 0; i < N; i++)
                htrack << histotrack[n][i] << ", ";
            htrack << std::endl;
        }
        
        std::cout << ".";
        std::cout.flush();
    }
    std::cout << std::endl;
}

void imageprinttest() {
    unsigned width = 128, height = 128;
    for(unsigned rgb = 0; rgb < 3; rgb++) {
        ca::bitmap<3> bmp(width, height);
        for(unsigned i = 0; i < width; i++)
            for(unsigned j = 0; j < height; j++) {
                // bmp.at(i, j, 0) = (unsigned char) ( i * 255 / height );
                // bmp.at(i, j, 1) = (unsigned char) ( i * 255 / width );
                bmp.at(i, j, rgb) = (unsigned char) ( i * 255 / width );
                // bmp.at(i, j, 2) = (unsigned char) ( (i+j) * 255 / (height+width) );
            }
        std::stringstream s;
        s << "ca/bitmap" << rgb << ".bmp";
        bmp.save(s.str());
    }
}

void terraingenerationtest() {

    int width = 128, height = 128;
    
    std::uniform_int_distribution<int> rndint(-3, 3);
    std::random_device g;
    std::mt19937 r(g());
    std::vector<std::array<int,2>> open, closed, deltas;

    deltas.push_back(std::array<int,2>({1, 0}));
    deltas.push_back(std::array<int,2>({1, 1}));
    deltas.push_back(std::array<int,2>({0, 1}));
    deltas.push_back(std::array<int,2>({-1, 1}));
    deltas.push_back(std::array<int,2>({-1, 0}));
    deltas.push_back(std::array<int,2>({-1, -1}));
    deltas.push_back(std::array<int,2>({0, -1}));
    deltas.push_back(std::array<int,2>({1, -1}));

    ca::bitmap<3> bmp(width, height);
    open.push_back(std::array<int,2>({width/2, height/2}));
    for(unsigned i = 0; i < 3; i++)
        bmp.at(width/2, height/2, i) = 100;

    while(open.size() > 0) {
        auto current = open[0];
        int currx = current[0], curry = current[1];

        unsigned ctr = 0;
        float sum = 0;
        for(auto& diff : deltas) {
            if(current[0] + diff[0] < width && current[0] + diff[0] >= 0 && 
                current[1] + diff[1] < height && current[1] + diff[1] >= 0)
                {
                    currx = current[0] + diff[0];
                    curry = current[1] + diff[1];

                    sum += bmp.at(currx, curry, 2);
                    if(bmp.at(currx, curry, 2))
                        ctr++;
                }
        }
        if(ctr)
            sum /= ctr;

        sum += rndint(r);
        sum = std::clamp(sum, 0.f, 255.f);
        for(unsigned i = 0; i < 3; i++)
            bmp.at(currx, curry, i) += sum; 

        std::shuffle(deltas.begin(), deltas.end(), r);
        for(auto& diff : deltas) {
            if(current[0] + diff[0] < width && current[0] + diff[0] >= 0 && 
                current[1] + diff[1] < height && current[1] + diff[1] >= 0)
                {
                    currx = current[0] + diff[0];
                    curry = current[1] + diff[1];

                    if(std::find(closed.begin(), closed.end(), std::array<int,2>({currx, curry})) == closed.end() &&
                        std::find(open.begin(), open.end(), std::array<int,2>({currx, curry})) == open.end()) {
                        open.emplace_back();
                        open.back()[0] = currx;
                        open.back()[1] = curry;
                    }
                }
        }

        open.erase(open.begin());
        std::cout << "\r" << (closed.size() * 100 / (width*height)) << "% " 
            << ", closed: " << closed.size() << ", open = " << open.size() << ", " 
            << "(" << currx << "," << curry << ")"
            << "          ";
        closed.push_back(current);
    }

    std::stringstream s;
    s << "landscape.bmp";
    bmp.save(s.str());
    std::cout << std::endl;
    
}

struct tcell {
    float posx, posy;
    friend std::ostream& operator<<(std::ostream& out, const tcell& c);
};

std::ostream& operator<<(std::ostream& out, const tcell& c) { return out << c.posx; }

void trealisationtest_inittcell(tcell& c) { c.posx = 0; c.posy = 0; }

void trealisationtest_updatetcell(ca::trealisation<tcell>& r, tcell& c) {
    static float p = 0.5;
    if(stats::rfloat() < p)
        c.posx += 1;
    else if(stats::rfloat() < p)
        c.posy += 1;
}

void trealisationtest() {
    ca::trealisation<tcell> r(5, 1, trealisationtest_inittcell, trealisationtest_updatetcell); 
    std::cout << r << std::endl;

    for(size_t i{0}; i < 100; i++) {
        r.update();
        std::cout << r << std::endl << "-----------------" << std::endl; 
    }
}

int main(int argc, char **argv) {
    
    // next setup ideas:
    /*
        ca simulation with data and image output

        cells: clogging together, persistent motion, slightly randomly drifting
                maximizing speed by aligning motion locally

        locusts: mostly persistent motion, slightly randomly drifting
        locusts as above: direction given by neural network based on locally visible food
                            maximizing speed and food snacked

        mine clearing tanks: neural net dictates left/right gas based on locally visible mines
                            direction + speed dictated by tank equation
                            maximizing speed and mines cleared

    */
    
    pricesimtest1();

    return 0; 
}

