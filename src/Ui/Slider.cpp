#include "Slider.hpp"

Slider::Slider(SDL_Renderer *renderer, SDL_Rect track)
    : renderer(renderer), track(track), knob({track.x, track.y - 5, 20, 20}),
      dragging(false), value(0){};

void Slider::updateSlider(Slider &slider, int mouseX) {
  // Clamp mouseX to the track's bounds
  if (mouseX < slider.track.x) {
    mouseX = slider.track.x;
  } else if (mouseX > slider.track.x + slider.track.w) {
    mouseX = slider.track.x + slider.track.w;
  }

  // Update the knob's x position (centering the knob on the mouse)
  slider.knob.x = mouseX - slider.knob.w / 2;

  // Calculate the slider's value based on knob position
  slider.value = float(mouseX - slider.track.x) / slider.track.w;
}

void Slider::draw() {
  // Track (gray)
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderFillRect(renderer, &track);

  // Knob (blue)
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, &knob);
}