#include "../Utility/Font.hpp"
#include "../Utility/Vec.hpp"
#include "../Utility/LTexture.hpp"

class CameraPositionDisplay {
public:
    CameraPositionDisplay(SDL_Renderer *gRenderer, Vec& cameraOffset) :
        gRenderer_(gRenderer),
        cameraOffset_(cameraOffset),
        texture_(LTexture()),
        font_(openDefaultFont(28))
    {}

    ~CameraPositionDisplay() {
        TTF_CloseFont(font_);
    }

    void checkCameraMovement(SDL_Event&e_) {
        if (e_.type == SDL_EVENT_KEY_DOWN) {
            switch (e_.key.key) {
                case SDLK_UP:
                    cameraOffset_.y += 25;
                    break;
                case SDLK_DOWN:
                    cameraOffset_.y -= 25;
                    break;
                case SDLK_LEFT:
                    cameraOffset_.x += 25;
                    break;
                case SDLK_RIGHT:
                    cameraOffset_.x -= 25;
                    break;
                default:
                    break;
            }
        }
    }

    void loadMedia() {
        texture_.loadFromRenderedText("Camera: (" + std::to_string(cameraOffset_.x) + ", " + std::to_string(cameraOffset_.y) + ")", {0, 0, 0, 0}, gRenderer_, font_);
    }

    void drawAndRender() {
        texture_.render(800, 100, gRenderer_);
    }

    float getX() {
        return cameraOffset_.x; 
    }

    float getY() {
        return cameraOffset_.y;
    }

private:
    SDL_Renderer *gRenderer_ = NULL;
    Vec& cameraOffset_;
    LTexture texture_;
    TTF_Font *font_;
};