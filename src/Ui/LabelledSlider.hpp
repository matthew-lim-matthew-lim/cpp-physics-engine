#ifndef LABELLED_SLIDER_HPP
#define LABELLED_SLIDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Slider.hpp"
#include "../Utility/Vec.hpp"
#include "../Utility/LTexture.hpp"

class LabelledSlider {
public:
    SDL_Color defaultTextColour = {0, 0, 0, 0};

    LabelledSlider(SDL_Renderer *gRenderer, Vec coordinates, std::string label) : 
        gRenderer_(gRenderer), 
        coordinates_(coordinates),
        label_(label),
        texture_(LTexture()),
        slider_(gRenderer_, {static_cast<int>(coordinates_.x), static_cast<int>(coordinates_.y), 400, 10})
    {}

    void loadRender();

    void drawAndRender();

    bool pointInKnob(SDL_Point& mousePoint);

    void resetDragging();

    void tryUpdateSliderPosition(float x);

    float getSliderValue();

private:
    SDL_Renderer *gRenderer_;
    Vec coordinates_;
    std::string label_;
    LTexture texture_;
    Slider slider_;
};

#endif // LABELLED_SLIDER_HPP