#ifndef LABELLED_SLIDER_HPP
#define LABELLED_SLIDER_HPP

#include <iostream>

class LabelledSlider {
public:
    SDL_Color defaultTextColour = {0, 0, 0, 0};

    LabelledSlider(SDL_Renderer *gRenderer, Vec coordinates, std::string label) : 
        gRenderer_(gRenderer), 
        coordinates_(coordinates),
        label_(label),
        texture_(LTexture()),
        slider_(gRenderer_, {coordinates_.x, coordinates_.y, 400, 10})
    {}

    void loadRender() {
        TTF_Font *gFont = TTF_OpenFont("fontCenturyGothic.ttf", 28);
        if (!texture_.loadFromRenderedText(label_ + ": " + std::to_string(slider_.value), defaultTextColour, gRenderer_, gFont)) {
            std::cerr << "loadRender(): Error loading from rendered text: " << label_ << std::endl;
        } 
    }

    void drawAndRender() {
        slider_.draw();
        texture_.render(coordinates_.x, coordinates_.y - 50, gRenderer_);
    }

    bool pointInKnob(SDL_Point& mousePoint) {
        if (SDL_PointInRect(&mousePoint, &slider_.knob)) {
            slider_.dragging = true;
            return true;
        }
        return false;
    }

    void resetDragging() {
        slider_.dragging = false;
    }

    void tryUpdateSliderPosition(float x) {
        if (slider_.dragging) {
            slider_.updateSlider(x);
        }
    }

    float getSliderValue() {
        return slider_.value;
    }

private:
    SDL_Renderer *gRenderer_;
    Vec coordinates_;
    std::string label_;
    LTexture texture_;
    Slider slider_;
};

#endif // LABELLED_SLIDER_HPP