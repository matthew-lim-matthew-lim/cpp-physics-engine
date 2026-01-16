#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../Utility/LTexture.hpp"

class FpsCounter {
public:
    SDL_Color defaultTextColour = {0, 0, 0, 0};

    FpsCounter(SDL_Renderer *gRenderer) : 
        gRenderer_(gRenderer),
        texture_(LTexture()),
        now_(SDL_GetPerformanceCounter()),
        last_(0)
    {}

    void loadMedia() {
        if (nextFpsUpdate_== 0) {
            nextFpsUpdate_ = 10;
            last_ = now_;
            now_ = SDL_GetPerformanceCounter();
            double deltaTime = (double)((now_ - last_) / (double)SDL_GetPerformanceFrequency());
            TTF_Font *gFont = TTF_OpenFont("fontCenturyGothic.ttf", 28);
            texture_.loadFromRenderedText("FPS: " + std::to_string(10.0 / deltaTime), {0, 0, 0, 0}, gRenderer_, gFont);
        } else {
            nextFpsUpdate_--;
        }
    }

    void drawAndRender() {
        texture_.render(800, 50, gRenderer_);
    }


private:
    SDL_Renderer *gRenderer_ = NULL;
    LTexture texture_;
    Uint64 now_ = 0;
    Uint64 last_ = 0;
    Uint64 nextFpsUpdate_ = 0;

    SDL_Event e_;
};

#endif // FPS_COUNTER_HPP