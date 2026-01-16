#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>

class Slider {
public:
  SDL_Renderer *renderer;
  SDL_Rect track; // The slider track (the background bar)
  SDL_Rect knob;  // The slider knob (the draggable part)
  bool dragging;  // Whether the knob is being dragged
  float value;    // The current value (0.0 to 1.0)

  Slider(SDL_Renderer *renderer, SDL_Rect track);

  Slider(SDL_Renderer *renderer, SDL_Rect track, float initialSliderPos);

  void updateSlider(int mouseX);

  void draw();
};

#endif // SLIDER_HPP