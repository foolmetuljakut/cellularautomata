#ifndef ca_stats_hpp
#define ca_stats_hpp

#include <memory>
#include <random>
#include <algorithm>
#include <functional>

class stats {
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_int_distribution<unsigned> rndint;
    static std::uniform_real_distribution<float> rndfloat;
public:
    static void setab(unsigned a, unsigned b);
    static unsigned rint();
    static float rfloat();

    template<typename T> static std::vector<T> map(std::vector<T>& samples, std::function<T(T&)> selector) {
        std::vector<T> smpls;
        smpls.reserve(samples.size());

        for(auto& sample : samples)
            smpls.push_back(selector(sample));

        return smpls;
    }

    template<typename T, unsigned N> static std::array<T, N> map(std::array<T, N>& samples, std::function<T(T&)> selector) {
        std::array<T, N> smpls;

        for(unsigned i = 0; i < N; i++)
            smpls[i] = selector(samples[i]);

        return smpls;
    }

    template<typename Out, typename In> static std::vector<Out> project(std::vector<In>& samples, std::function<Out(In&)> selector) {
        std::vector<Out> smpls;
        smpls.reserve(samples.size());

        for(auto& sample : samples)
            smpls.push_back(selector(sample));

        return smpls;
    }

    template<typename T> static std::vector<T> filter(std::vector<T>& samples, std::function<bool(T&)> predicate) {
        std::vector<T> filtered;

        for(auto it = samples.begin(); it != samples.end(); it++)
            if(predicate(*it))
                filtered.emplace_back(*it);
                // std::move(it, it+1, std::back_inserter(filtered)); 
                // also uses a copy?

        return filtered;
    }

    template<unsigned N> static std::array<float, N> range() {
        std::array<float, N> r;
        for(unsigned i = 0; i < N; i++)
            r[i] = i;
        return r;
    }

    template<unsigned N> static std::array<float, N> range(float start, float stop) {
        std::array<float, N> r;
        float step = (stop - start) / (N-1);
        for(unsigned i = 0; i < N; i++)
            r[i] = start + i * step;
        return r;
    }

    template<unsigned N> static std::array<float, N> bin(std::vector<float>& samples, std::array<float, N+1u>& bounds) {
        std::array<float, N> histogram{{}};

        for(float& s : samples) {
            auto it = std::find_if(bounds.begin(), bounds.end(),
                [s](float& b){ return s < b; });
            // it points to upper bound now
            unsigned binindex = (--it) - bounds.begin();
            if(binindex >= N) // binindex == 4294967295
                continue;
            histogram[binindex]++;
        }
        return histogram;
    }
    
    template<unsigned N> static float integr(std::array<float, N>& y, std::array<float, N+1u>& x) {
        float s = 0;
        for(unsigned i = 0; i < x.size()-1; i++)
            s += y[i] * (x[i+1] - x[i]);
        return s;
    }

    template<unsigned N> static float sum(std::array<float, N>& y) {
        float s = 0;
        for(unsigned i = 0; i < y.size(); i++)
            s += y[i];
        return s;
    }

    template<unsigned N> static std::array<float, N> normalize(std::array<float, N>& y, std::array<float, N+1u>& x, bool integrate = true) {
        float s = integrate ? integr<N>(y, x) : sum<N>(y);
        std::array<float, N> n;
        for(unsigned i = 0; i < y.size(); i++)
            n[i] = y[i] / s;
        return n;
    }

    template<unsigned N> static float mean(std::array<float, N>& y) {
        float m = 0;
        for(auto& f : y)
            m += f;
        return m / y.size();
    }

    template<unsigned N> static float mean(std::array<float, N+1>& xbounds, std::array<float, N>& y, float ynorm = 0) {
        if(ynorm == 0)
            ynorm = integr<N>(y, xbounds);
        
        float m = 0;
        for(unsigned i = 0; i < y.size(); i++)
            m += (xbounds[i+1] + xbounds[i]) * y[i];
        return m / (2*ynorm);
    }

    template<unsigned N> static float min(std::array<float, N>& y) {
        float m = y[0];
        for(auto& f : y)
            if(f < m) m = f;
        return m;
    }

    template<unsigned N> static float max(std::array<float, N>& y) {
        float m = y[0];
        for(auto& f : y)
            if(f > m) m = f;
        return m;
    }

    template<unsigned N> static float std(std::array<float, N>& y, float mean) {
        float f2 = 0;
        for(auto& f : y)
            f2 += f*f;
        return sqrt((f2 - mean*mean) / (y.size()-1));
    }

    template<unsigned N> static float std(std::array<float, N+1>& xbounds, std::array<float, N>& y, float mean, float ynorm = 0) {
        if(ynorm == 0)
            ynorm = integr<N>(y, xbounds);
            
        float m = 0;
        for(unsigned i = 0; i < y.size(); i++)
            m += ((xbounds[i+1] + xbounds[i])/2 - mean) * ((xbounds[i+1] + xbounds[i])/2 - mean) * y[i];
        return sqrt(m / ynorm);
    }

};

#endif // ca_stats_hpp