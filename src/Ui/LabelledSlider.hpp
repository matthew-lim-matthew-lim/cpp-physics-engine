#ifndef LABELLED_SLIDER_HPP
#define LABELLED_SLIDER_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "Slider.hpp"
#include "../Utility/Vec.hpp"
#include "../Utility/LTexture.hpp"
#include "../Utility/Font.hpp"

class LabelledSlider {
public:
    SDL_Color defaultTextColour = {0, 0, 0, 0};

    LabelledSlider(SDL_Renderer *gRenderer, Vec coordinates, std::string label) : 
        gRenderer_(gRenderer), 
        coordinates_(coordinates),
        label_(label),
        texture_(LTexture()),
        font_(openDefaultFont(28)),
        slider_(gRenderer_, {(float)coordinates_.x, (float)coordinates_.y, 400, 10})
    {}

    LabelledSlider(SDL_Renderer *gRenderer, Vec coordinates, std::string label, float initialSliderPos) : 
        gRenderer_(gRenderer), 
        coordinates_(coordinates),
        label_(label),
        texture_(LTexture()),
        font_(openDefaultFont(28)),
        slider_(gRenderer_, {(float)coordinates_.x, (float)coordinates_.y, 400, 10}, initialSliderPos)
    {}

    ~LabelledSlider() {
        TTF_CloseFont(font_);
    }

    void loadRender();

    void drawAndRender();

    bool pointInKnob(SDL_FPoint& mousePoint);

    void resetDragging();

    void tryUpdateSliderPosition(float x);

    float getSliderValue();

private:
    SDL_Renderer *gRenderer_;
    Vec coordinates_;
    std::string label_;
    LTexture texture_;
    TTF_Font *font_;
    Slider slider_;
};

#endif // LABELLED_SLIDER_HPP