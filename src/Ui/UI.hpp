#ifndef UI_HPP
#define UI_HPP

#include "./LabelledSlider.hpp"

#include <optional>
#include <vector>
#include <memory>
#include <unordered_map>

class UI {
public:
    UI(SDL_Renderer *gRenderer) :
        gRenderer_(gRenderer)
    {
        labelledSliders_.emplace(
            "Speed",
            std::make_unique<LabelledSlider>(
                gRenderer_, 
                Vec(800, 200),
                "Speed"
            )
        );

        labelledSliders_.emplace(
            "Direction",
            std::make_unique<LabelledSlider>(
                gRenderer_, 
                Vec(800, 300),
                "Direction"
            )
        );

        labelledSliders_.emplace(
            "Size",
            std::make_unique<LabelledSlider>(
                gRenderer_, 
                Vec(800, 400),
                "Size"
            )
        );

        labelledSliders_.emplace(
            "Weight",
            std::make_unique<LabelledSlider>(
                gRenderer_, 
                Vec(800, 500),
                "Weight"
            )
        );
    }

    void processPollEvents(bool& quit, std::vector<std::unique_ptr<Shape>>& shapes) {
        // Handle events on queue
        while (SDL_PollEvent(&e_) != 0) {
            // User requests quit
            if (e_.type == SDL_QUIT) {
                quit = true;
            }
            
            if (e_.type == SDL_MOUSEBUTTONDOWN) {
                if (e_.button.button == SDL_BUTTON_LEFT) {
                    // Check if mouse is inside the knob rectangle
                    SDL_Point mousePoint = {e_.button.x, e_.button.y};
                    bool knobClicked = false;
                    for (std::pair<const std::string, std::unique_ptr<LabelledSlider>>& labelledslider : labelledSliders_) {
                        knobClicked |= labelledslider.second->pointInKnob(mousePoint);
                    }
                    if (!knobClicked) {
                        std::unique_ptr<Circle> userCircle = std::make_unique<Circle>(
                            Vec(e_.button.x, e_.button.y),
                            100 * std::max(labelledSliders_["Size"]->getSliderValue(), static_cast<float>(0.1)),
                            Vec(labelledSliders_["Speed"]->getSliderValue() *
                                    std::cos(M_PI * labelledSliders_["Direction"]->getSliderValue()),
                                labelledSliders_["Speed"]->getSliderValue()) *
                                std::sin(M_PI * labelledSliders_["Direction"]->getSliderValue()),
                            10 * std::max(labelledSliders_["Weight"]->getSliderValue(), static_cast<float>(0.1)),
                            0.8);

                        shapes.push_back(std::move(userCircle));
                    }
                }
            }
            
            if (e_.type == SDL_MOUSEBUTTONUP) {
                for (std::pair<const std::string, std::unique_ptr<LabelledSlider>>& labelledslider : labelledSliders_) {
                    labelledslider.second->resetDragging();
                }
            }
            
            if (e_.type == SDL_MOUSEMOTION) {
                for (std::pair<const std::string, std::unique_ptr<LabelledSlider>>& labelledslider : labelledSliders_) {
                    labelledslider.second->tryUpdateSliderPosition(e_.motion.x);
                }
            }
        }
    }

    void loadMedia() {
        for (std::pair<const std::string, std::unique_ptr<LabelledSlider>>& labelledslider : labelledSliders_) {
            labelledslider.second->loadRender();
        }
    }

    void drawMedia() {
        for (std::pair<const std::string, std::unique_ptr<LabelledSlider>>& labelledslider : labelledSliders_) {
            labelledslider.second->drawAndRender();
        }
    }

private:
    // Rendered text textures
    std::unordered_map<std::string, std::unique_ptr<LabelledSlider>> labelledSliders_;
    SDL_Renderer *gRenderer_ = NULL;

    SDL_Event e_;
};

#endif // UI_HPP