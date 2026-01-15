#ifndef UI_HPP
#define UI_HPP

#include <optional>
#include <vector>
#include <memory>

class UI {
public:
    void processPollEvents(bool& quit, std::vector<std::unique_ptr<Shape>>& shapes) {
        // Handle events on queue
        while (SDL_PollEvent(&e_) != 0) {
            // User requests quit
            if (e_.type == SDL_QUIT) {
                quit = true;
            }
            
            if (e_.type == SDL_MOUSEBUTTONDOWN) {
                if (e_.button.button == SDL_BUTTON_LEFT) {
                // Check if mouse is inside the knob rectangle
                SDL_Point mousePoint = {e_.button.x, e_.button.y};
                if (SDL_PointInRect(&mousePoint, &sliderSpeed_.knob)) {
                    sliderSpeed_.dragging = true;
                } else if (SDL_PointInRect(&mousePoint, &sliderDirection_.knob)) {
                    sliderDirection_.dragging = true;
                } else if (SDL_PointInRect(&mousePoint, &sliderSize_.knob)) {
                    sliderSize_.dragging = true;
                } else if (SDL_PointInRect(&mousePoint, &sliderWeight_.knob)) {
                    sliderWeight_.dragging = true;
                } else {
                    std::unique_ptr<Circle> userCircle = std::make_unique<Circle>(
                        Vec(e_.button.x, e_.button.y),
                        100 * std::max(sliderSize_.value, static_cast<float>(0.1)),
                        Vec(sliderSpeed_.value *
                                std::cos(M_PI * sliderDirection_.value),
                            sliderSpeed_.value) *
                            std::sin(M_PI * sliderDirection_.value),
                        10 * std::max(sliderWeight_.value, static_cast<float>(0.1)),
                        0.8);

                    shapes.push_back(std::move(userCircle));
                }
                }
            }
            
            if (e_.type == SDL_MOUSEBUTTONUP) {
                if (e_.button.button == SDL_BUTTON_LEFT) {
                sliderSpeed_.dragging = false;
                sliderDirection_.dragging = false;
                sliderSize_.dragging = false;
                sliderWeight_.dragging = false;
                }
            }
            
            if (e_.type == SDL_MOUSEMOTION) {
                if (sliderSpeed_.dragging) {
                sliderSpeed_.updateSlider(e_.motion.x);
                }
                if (sliderDirection_.dragging) {
                sliderDirection_.updateSlider(e_.motion.x);
                }
                if (sliderSize_.dragging) {
                sliderSize_.updateSlider(e_.motion.x);
                }
                if (sliderWeight_.dragging) {
                sliderWeight_.updateSlider(e_.motion.x);
                }
            }
        }
    }

    void loadMedia() {
        TTF_Font *gFont = TTF_OpenFont("fontCenturyGothic.ttf", 28);
        SDL_Color textColor = {0, 0, 0, 0};
        gTextTextures_[0]->loadFromRenderedText("Speed: " +
          std::to_string(sliderSpeed_.value), textColor, gRenderer_, gFont);
        gTextTextures_[1]->loadFromRenderedText("Direction: " +
          std::to_string(sliderDirection_.value), textColor, gRenderer_, gFont);
        gTextTextures_[2]->loadFromRenderedText("Size: " +
          std::to_string(sliderSize_.value), textColor, gRenderer_, gFont);
        gTextTextures_[3]->loadFromRenderedText("Mass: " +
          std::to_string(sliderWeight_.value), textColor, gRenderer_, gFont);
    }

    void drawMedia() {
        sliderSpeed_.draw();
        sliderDirection_.draw();
        sliderSize_.draw();
        sliderWeight_.draw();
    }

    static std::optional<UI> create(std::vector<std::unique_ptr<LTexture>>& gTextTextures,
        SDL_Renderer *gRenderer
    ) {
        try {
            return UI(gTextTextures, gRenderer);
        } catch (...) {
            return std::nullopt;
        }
    }

private:
    std::vector<std::unique_ptr<LTexture>>& gTextTextures_;
    SDL_Renderer *gRenderer_ = NULL;

    SDL_Event e_;

    Slider sliderSpeed_;
    Slider sliderDirection_;
    Slider sliderSize_;
    Slider sliderWeight_;

    UI(std::vector<std::unique_ptr<LTexture>>& gTextTextures,
        SDL_Renderer *gRenderer
    ) : gTextTextures_(gTextTextures), 
        gRenderer_(gRenderer),
        sliderSpeed_(gRenderer_, {800, 200, 400, 10}),
        sliderDirection_(gRenderer_, {800, 300, 400, 10}),
        sliderSize_(gRenderer_, {800, 400, 400, 10}),
        sliderWeight_(gRenderer_, {800, 500, 400, 10})
    {
        // Loading success flag
        bool success = true;

        // Open the font
        TTF_Font *gFont = TTF_OpenFont("fontCenturyGothic.ttf", 28);
        if (gFont == NULL) {
            printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
        } else {
            // Render text
            SDL_Color textColor = {0, 0, 0, 0};
            gTextTextures_.push_back(std::make_unique<LTexture>(LTexture()));
            if (!gTextTextures_.back()->loadFromRenderedText("Speed: " + std::to_string(sliderSpeed_.value), textColor,
                                                            gRenderer_, gFont)) {
            printf("Failed to render text texture!\n");
            success = false;
            }
            gTextTextures_.push_back(std::make_unique<LTexture>(LTexture()));
            if (!gTextTextures_.back()->loadFromRenderedText("Direction: " + std::to_string(sliderDirection_.value), textColor,
                                                            gRenderer_, gFont)) {
            printf("Failed to render text texture!\n");
            success = false;
            }
            gTextTextures_.push_back(std::make_unique<LTexture>(LTexture()));
            if (!gTextTextures_.back()->loadFromRenderedText("Size: " + std::to_string(sliderSize_.value), textColor,
                                                            gRenderer_, gFont)) {
            printf("Failed to render text texture!\n");
            success = false;
            }
            gTextTextures_.push_back(std::make_unique<LTexture>(LTexture()));
            if (!gTextTextures_.back()->loadFromRenderedText("Mass: " + std::to_string(sliderWeight_.value), textColor,
                                                            gRenderer_, gFont)) {
            printf("Failed to render text texture!\n");
            success = false;
            }
        }

        if (!success) {
            throw std::runtime_error("Unable to init Ui");
        }
    }
};

#endif // UI_HPP