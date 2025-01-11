#include "application.hpp"

namespace CellularAutomata::Gui {

    bool Application::handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
                case sf::Event::MouseButtonPressed:
                    click = true;
                    break;
                case sf::Event::MouseButtonReleased:
                    click = false;
                    break;
                case sf::Event::LostFocus:
                    backg = true;
                    break;
                case sf::Event::GainedFocus:
                    backg = false;
                    break;
                case sf::Event::KeyPressed:
                    // escape just passes through to closed event
                    if(event.key.code != sf::Keyboard::Escape)
                        break;
                case sf::Event::Closed:
                    window.close();
                    return false;
                default: break;
            }
        }

        if(click) {
            onMouseClicked();
        }

        return true;
    }

    void Application::onMouseClicked() {
        std::cout << "mouse has been clicked\n";
    }

    void Application::showFps() {
        std::stringstream s;
        s   << "[avg stats over >= 0.1s]\n"
            << "fps: " << fps << "\n"
            << "physics: "  << updateTimeInFps << " fps\n"
            << "physics time per unit: "  << (1'000'000.f * updateTimePerCell) << " us\n"
            << "graphics: " << drawTimeInFps << " fps"
            ;
        sf::Text fpstext(s.str(), font, 12);
        window.draw(fpstext);
    }

    void Application::updateGraphics() {
        if(backg)
            return;

        window.clear(sf::Color::Black);
        
        graphicsEvaluation.clear();
        graphicsEvaluation.process(realisation);

        std::vector<float> maxCellPerField = graphicsEvaluation.getFieldEnsembleData(fieldDataKey);
        size_t drawingRects = 0;
        for(size_t i = 0; i < maxCellPerField.size(); i++) {
            if(maxCellPerField[i] > 0.05f * realisation.getFieldSize()) {
                drawingRects++;

                float x = realisation.fields[i].x,
                        y = realisation.fields[i].y;
                float value = maxCellPerField[i];
                float grayScale = static_cast<uint8_t>(value / realisation.getFieldSize() * 255);
                fieldShape.setPosition(sf::Vector2f(x,y));
                fieldShape.setFillColor(sf::Color(grayScale, grayScale, grayScale));
                
                window.draw(fieldShape);
            }
        }
        spdlog::info("drawing {} relevant cells", drawingRects);

        showFps();
        window.display();
    }

    Application::Application(const std::string& windowtitle, size_t width, size_t height, 
    float fieldSize, size_t initialNumberOfCells) 
        : w{width}, h{height}, window(sf::VideoMode(w, h), windowtitle),
        backg{false}, click{false}, fpsUpdateCounter{0}, fps{0}, drawTime{0}, drawTimeInFps{0}, updateTime{0}, updateTimeInFps{0}, updateTimePerCell{0}, fieldShape{sf::Vector2f{1.f, 1.f}},
        realisation{
            scen::SettingsBuilder().standardSetting()
            .width(width)
            .height(height)
            .fieldSize(fieldSize)
            .initialCellCount(initialNumberOfCells)
            .initializeWith([width, height](sim::Cell& cell) {
                cell.size = 33.f;
                cell.pos = width / 2 * height + height / 2;
            })
            .build()}, 
        graphicsEvaluation("gui_recording"),
        fieldDataKey{"max_cell_per_field"} {

        window.setVerticalSyncEnabled(true);
        window.setActive(true);
        if(!font.loadFromFile("resources/tuffy.ttf")) {
            throw "could not load resources/tuffy.ttf";
        }

        graphicsEvaluation.addClaim(rep::MetaInformation(fieldDataKey, "find biggest cell on a field"));
        graphicsEvaluation.addFieldEnsembleMap(fieldDataKey, 
        [this](const std::vector<sim::Field>& fields) {
            return stats::map<float, sim::Field>(fields, [this](const sim::Field& field) {
                if(field.cellIndices.size() > 0) {
                    // stats -> filter by predicate
                    // access cells -> filter cells by field.cellIndices
                    // -> get biggest cell size -> return 
                    std::vector<float> cellSizes;
                    for(const size_t& index: field.cellIndices) {
                        cellSizes.push_back(realisation.cells[index].size);
                    }
                    return stats::max(cellSizes);
                } else {
                    return 0.f;
                }
            });
        });
    }

    void Application::run() {
        sf::Int64 dt = 0;
        while (window.isOpen())
        {
            spdlog::info("---starting-update----");

            if(!handleEvents()) { // maybe get this into a different thread to avoid blocking gui
                return;
            }

            sf::Clock clock;

            size_t subSteps = 1;
            realisation.update(subSteps);
            float updateTime0{clock.restart().asSeconds()};

            updateGraphics();
            float drawTime0{clock.restart().asSeconds()};

            spdlog::info("update time: {:1.2f} s, update graphics time {:1.2f} s", updateTime0, drawTime0);
            
            drawTime += drawTime0;
            updateTime += updateTime0;
            dt += drawTime0 + updateTime0;
            fpsUpdateCounter++;

            if(dt > 0.1f) { // 0.1s
                fps = fpsUpdateCounter / dt; // average fps over the last 0.1s

                float fps_squared = fps * fps;
                drawTimeInFps = drawTime / fpsUpdateCounter * fps_squared;
                updateTimeInFps = updateTime / fpsUpdateCounter * fps_squared;
                updateTimePerCell = updateTime / fpsUpdateCounter / realisation.cells.size();

                dt = 0;
                fpsUpdateCounter = 0;
            }
            
            spdlog::info("---ending-update------");
        }
    }
};