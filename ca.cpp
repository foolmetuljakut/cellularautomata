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

void pricesimtest1() {

    /*setup: 
        cells represent buyers and sellers 
            a cell contains a price level to interact with others at
            a cell contains a function to adjust the price
                if buyers can't be found => lower prices by a constant
                if sellers can't be found => increase prices by a constant
        measure global price distribution*/

    unsigned A = 50;
    const unsigned N = 10;
    for(unsigned a = 1; a < A; a++) 
        std::cout << ".";
    std::cout << std::endl;

    for(unsigned a = 1; a < A; a++) {
        realisation r;
        // create buyers: { if trades == 0 => increase price }
        r.create(a, {"price", "trades", "id", "buyer"}, {
            [](cell& c) {
                if(c["trades"] < 1)
                    c["price"] += 1;
                else
                    c["price"] -= 1;
            }
        });
        // create sellers: { if trades == 0 => decrease price }
        r.create(A - a, {"price", "trades", "id", "seller"}, {
            [](cell& c) {
                if(c["trades"] < 1)
                    c["price"] -= 1;
                else
                    c["price"] += 1;
            }
        });

        for(unsigned i = 0; i < r.cells.size(); i++)
            r.cells[i]["price"] = r.cells[i]["id"] = i;

        std::vector<float> ttrack, id0track, idmaxtrack;
        std::vector<std::array<float, N>> histotrack, bhtrack, shtrack;
        ttrack.push_back(0);
        id0track.push_back(r.cells[0]["price"]);
        idmaxtrack.push_back(r.cells.back()["price"]);
        // "manual" update function
        for(unsigned t = 0; t < 100; t++) {

            // reset everything
            for(auto& trader : r.cells) 
                trader["trades"] = 0;

            // start next round of negotiations
            for(auto& trader : r.cells) {
                float pricelvl = trader["price"];
                bool isbuyer = trader.contains("buyer");
                auto matches = stats::filter<cell>(r.cells,
                [pricelvl, isbuyer](cell& partner) {
                    if(isbuyer)
                        return partner.contains("seller") && 
                                pricelvl >= partner["price"];
                    else
                        return partner.contains("buyer") && 
                                pricelvl <= partner["price"];

                    // return ((partner.contains("seller") && isbuyer) || 
                    //             (partner.contains("buyer") && !isbuyer)) &&
                    //         abs(partner["price"] - pricelvl) < 1;
                });

                unsigned trades = matches.size();
                if( trades > 0) {
                    trader["trades"] += trades;
                    for(auto& partner : matches)
                        partner["trades"] += 1;
                }
            }

            for(auto& trader : r.cells) 
                trader.update();

            ttrack.push_back(t+1);
            id0track.push_back(r.cells[0]["price"]);
            idmaxtrack.push_back(r.cells.back()["price"]);
            
            auto buyers = stats::filter<cell>(r.cells, [](cell& c) { return c.contains("buyer"); });
            auto sellers = stats::filter<cell>(r.cells, [](cell& c) { return c.contains("seller"); });
            auto prices = stats::project<float, cell>(r.cells, [](cell& c) { return c["price"]; });
            auto bprices = stats::project<float, cell>(buyers, [](cell& c) { return c["price"]; });
            auto sprices = stats::project<float, cell>(sellers, [](cell& c) { return c["price"]; });

            auto binbounds = stats::range<N+1>(0, r.cells.size());

            if(a == 1 && t == 0) {
                auto bounds = std::ofstream((std::stringstream() << "ca/data/bounds.csv").str());
                for(unsigned i = 0; i < N; i++)
                    bounds << binbounds[i] << ", ";
                bounds << std::endl;
            }

            histotrack.push_back(stats::bin<N>(prices, binbounds));
            bhtrack.push_back(stats::bin<N>(bprices, binbounds));
            shtrack.push_back(stats::bin<N>(sprices, binbounds));
        }
        auto idtrack = std::ofstream((std::stringstream() << "ca/data/idtracka" << a << ".csv").str());
        for(unsigned n = 0; n < id0track.size(); n++)
            idtrack << ttrack[n] << ", "
                    << id0track[n] << ", "
                    << idmaxtrack[n] << "\n";

        auto htrack = std::ofstream((std::stringstream() << "ca/data/htracka" << a << ".csv").str());
        for(unsigned n = 0; n < histotrack.size(); n++) {
            htrack << n << ", ";
            for(unsigned i = 0; i < N; i++)
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
    float posx;
    friend std::ostream& operator<<(std::ostream& out, const tcell& c);
};

std::ostream& operator<<(std::ostream& out, const tcell& c) { return out << c.posx; }

void inittcell(tcell& c) { c.posx = 0; }

void updatetcell(ca::trealisation<tcell>& r, tcell& c) {
    static float p = 0.5;
    if(stats::rfloat() < p)
        c.posx += 1;
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
    
    ca::trealisation<tcell> r(5, 1, inittcell, updatetcell); 
    std::cout << r << std::endl;

    for(size_t i{0}; i < 25; i++) {
        r.update();
        std::cout << r << std::endl << "-----------------" << std::endl; 
    }
    return 0; 
}

