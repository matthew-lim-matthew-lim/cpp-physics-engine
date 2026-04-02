#include "Slider.hpp"

Slider::Slider(SDL_Renderer *renderer, SDL_FRect track)
    : renderer(renderer), 
      track(track),
      knob({track.x, track.y - 5, 20, 20}),
      dragging(false),
      value(0)
{};

Slider::Slider(SDL_Renderer *renderer, SDL_FRect track, float initialSliderPos)
    : renderer(renderer), 
      track(track),
      knob({track.x + initialSliderPos * track.w, track.y - 5, 20, 20}),
      dragging(false),
      value(initialSliderPos)
{};

void Slider::updateSlider(float mouseX) {
  // Clamp mouseX to the track's bounds
  if (mouseX < track.x) {
    mouseX = track.x;
  } else if (mouseX > track.x + track.w) {
    mouseX = track.x + track.w;
  }

  // Update the knob's x position (centering the knob on the mouse)
  knob.x = mouseX - knob.w / 2.0f;

  // Calculate the slider's value based on knob position
  value = float(mouseX - track.x) / track.w;
}

void Slider::draw() {
  // Track (gray)
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderFillRect(renderer, &track);

  // Knob (blue)
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, &knob);
}