#pragma once

#include "../std.hpp"
#include "../scenarios.hpp"
namespace scen = CellularAutomata::Scenarios;
#include "../reporting.hpp"
namespace sim = CellularAutomata::Simulation;
namespace rep = CellularAutomata::Reporting;

namespace CellularAutomata::Gui {

    class Application {

        size_t w,h;
        sf::RenderWindow window;
        bool backg; // window set to background => advance physics, stop rendering
        bool click;
        size_t fpsUpdateCounter;
        float fps, drawTime, drawTimeInFps, updateTime, updateTimeInFps, updateTimePerCell;
        sf::Font font; 
        
        sf::RectangleShape fieldShape;
        scen::StochasticRealisation realisation;
        rep::Reason graphicsEvaluation;
        std::string fieldDataKey;

        bool handleEvents();
        void onMouseClicked();
        void showFps();
        void updateGraphics();

    public:
        Application(const std::string& windowtitle, size_t width, size_t height, float fieldSize, size_t initialNumberOfCells);
        void run();
    };

};