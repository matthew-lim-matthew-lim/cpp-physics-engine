#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SDL3/SDL.h>

class Slider {
public:
  SDL_Renderer *renderer;
  SDL_FRect track; // The slider track (the background bar)
  SDL_FRect knob;  // The slider knob (the draggable part)
  bool dragging;  // Whether the knob is being dragged
  float value;    // The current value (0.0 to 1.0)

  Slider(SDL_Renderer *renderer, SDL_FRect track);

  Slider(SDL_Renderer *renderer, SDL_FRect track, float initialSliderPos);

  void updateSlider(float mouseX);

  void draw();
};

#endif // SLIDER_HPP