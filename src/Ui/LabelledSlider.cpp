#include "LabelledSlider.hpp"

#include <iostream>

void LabelledSlider::loadRender() {
    if (!texture_.loadFromRenderedText(label_ + ": " + std::to_string(slider_.value), defaultTextColour, gRenderer_, font_)) {
        std::cerr << "loadRender(): Error loading from rendered text: " << label_ << std::endl;
    }
}

void LabelledSlider::drawAndRender() {
    slider_.draw();
    texture_.render(coordinates_.x, coordinates_.y - 50, gRenderer_);
}

bool LabelledSlider::pointInKnob(SDL_FPoint& mousePoint) {
    if (SDL_PointInRectFloat(&mousePoint, &slider_.knob)) {
        slider_.dragging = true;
        return true;
    }
    return false;
}

void LabelledSlider::resetDragging() {
    slider_.dragging = false;
}

void LabelledSlider::tryUpdateSliderPosition(float x) {
    if (slider_.dragging) {
        slider_.updateSlider(x);
    }
}

float LabelledSlider::getSliderValue() {
    return slider_.value;
}