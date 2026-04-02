#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "../Utility/LTexture.hpp"
#include "../Utility/Font.hpp"

class FpsCounter {
public:
    SDL_Color defaultTextColour = {0, 0, 0, 0};

    FpsCounter(SDL_Renderer *gRenderer) : 
        gRenderer_(gRenderer),
        texture_(LTexture()),
        font_(openDefaultFont(28)),
        now_(SDL_GetPerformanceCounter()),
        last_(0)
    {}

    ~FpsCounter() {
        TTF_CloseFont(font_);
    }

    void loadMedia() {
        if (nextFpsUpdate_== 0) {
            nextFpsUpdate_ = 10;
            last_ = now_;
            now_ = SDL_GetPerformanceCounter();
            double deltaTime = (double)((now_ - last_) / (double)SDL_GetPerformanceFrequency());
            texture_.loadFromRenderedText("FPS: " + std::to_string(10.0 / deltaTime), {0, 0, 0, 0}, gRenderer_, font_);
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
    TTF_Font *font_;
    Uint64 now_ = 0;
    Uint64 last_ = 0;
    Uint64 nextFpsUpdate_ = 0;

    SDL_Event e_;
};

#endif // FPS_COUNTER_HPP