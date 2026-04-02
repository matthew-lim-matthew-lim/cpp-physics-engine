#ifndef FONT_HPP
#define FONT_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

// Opens a TTF font, searching relative to the executable first, then falling
// back to a bundled system font.
inline TTF_Font* openDefaultFont(int ptsize) {
    // Try font relative to executable directory
    const char* basePath = SDL_GetBasePath();
    if (basePath) {
        std::string path = std::string(basePath) + "fontCenturyGothic.ttf";
        TTF_Font* font = TTF_OpenFont(path.c_str(), ptsize);
        if (font) return font;
    }

    // Fall back to a system font
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", ptsize);
    if (font) return font;

    SDL_Log("openDefaultFont: could not open any font: %s", SDL_GetError());
    return nullptr;
}

#endif // FONT_HPP
