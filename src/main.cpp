#include "Shapes/Circle.hpp"
#include "Shapes/Rectangle.hpp"
#include "Shapes/Shape.hpp"
#include "Ui/Slider.hpp"
#include "Utility/LTexture.hpp"
#include "Utility/Vec.hpp"

// Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <cstddef>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Globally used font
TTF_Font *gFont = NULL;

// Rendered texture
std::vector<std::unique_ptr<LTexture>> gTextTextures;

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  } else {
    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
      printf("Warning: Linear texture filtering not enabled!");
    }

    // Create window
    gWindow = SDL_CreateWindow("Awesome Physics Engine",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else {
      // Create renderer for window
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n",
               SDL_GetError());
        success = false;
      } else {
        // Initialize renderer color
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
          printf("SDL_image could not initialize! SDL_image Error: %s\n",
                 IMG_GetError());
          success = false;
        }

        // Initialize SDL_ttf
        if (TTF_Init() == -1) {
          printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                 TTF_GetError());
          success = false;
        }
      }
    }
  }

  return success;
}

bool loadMedia() {
  // Loading success flag
  bool success = true;

  // Open the font
  gFont = TTF_OpenFont("fontCenturyGothic.ttf", 28);
  if (gFont == NULL) {
    printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    success = false;
  } else {
    // Render text
    SDL_Color textColor = {0, 0, 0};
    gTextTextures.push_back(std::make_unique<LTexture>(LTexture()));
    if (!gTextTextures.back()->loadFromRenderedText("Speed", textColor,
                                                    gRenderer, gFont)) {
      printf("Failed to render text texture!\n");
      success = false;
    }
    gTextTextures.push_back(std::make_unique<LTexture>(LTexture()));
    if (!gTextTextures.back()->loadFromRenderedText("Direction", textColor,
                                                    gRenderer, gFont)) {
      printf("Failed to render text texture!\n");
      success = false;
    }
    gTextTextures.push_back(std::make_unique<LTexture>(LTexture()));
    if (!gTextTextures.back()->loadFromRenderedText("Size", textColor,
                                                    gRenderer, gFont)) {
      printf("Failed to render text texture!\n");
      success = false;
    }
    gTextTextures.push_back(std::make_unique<LTexture>(LTexture()));
    if (!gTextTextures.back()->loadFromRenderedText("Mass", textColor,
                                                    gRenderer, gFont)) {
      printf("Failed to render text texture!\n");
      success = false;
    }
  }

  return success;
}

void close() {
  // Free loaded images
  for (size_t i = 0; i < gTextTextures.size(); i++) {
    gTextTextures[i]->free();
  }

  // Free global font
  TTF_CloseFont(gFont);
  gFont = NULL;

  // Destroy window
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  // Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}

SDL_Texture *loadTexture(std::string path) {
  // The final texture
  SDL_Texture *newTexture = NULL;

  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
           IMG_GetError());
  } else {
    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
             SDL_GetError());
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  return newTexture;
}

const int SPEED_SCALE = 100; // Higher means slower
const float INFINITE_MASS = 0;

int main(int, char *[]) {
  // Start up SDL and create window
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    // Load media
    if (!loadMedia()) {
      printf("Failed to load media!\n");
    } else {
      // Main loop flag
      bool quit = false;

      // Event handler
      SDL_Event e;

      std::unique_ptr<Rectangle> staticRec = std::make_unique<Rectangle>(
          Vec(50, 550), Vec(550, 600), Vec(0, 0), INFINITE_MASS, 1);

      std::unique_ptr<Rectangle> movingRec = std::make_unique<Rectangle>(
          Vec(300, 100), Vec(400, 150), Vec(0, 0), 50, 0.7);

      std::unique_ptr<Circle> movingCircle =
          std::make_unique<Circle>(Vec(200, 100), 60, Vec(0, 0), 10, 0.8);

      Uint64 NOW = SDL_GetPerformanceCounter();
      Uint64 LAST = 0;
      double deltaTime = 0;

      std::vector<std::unique_ptr<Shape>> shapes;
      shapes.push_back(std::move(staticRec));
      shapes.push_back(std::move(movingRec));
      shapes.push_back(std::move(movingCircle));

      Slider sliderSpeed = Slider(gRenderer, {800, 200, 400, 10});
      Slider sliderDirection = Slider(gRenderer, {800, 300, 400, 10});
      Slider sliderSize = Slider(gRenderer, {800, 400, 400, 10});
      Slider sliderWeight = Slider(gRenderer, {800, 500, 400, 10});

      // While application is running
      while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
          // User requests quit
          if (e.type == SDL_QUIT) {
            quit = true;
          } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
              // Check if mouse is inside the knob rectangle
              SDL_Point mousePoint = {e.button.x, e.button.y};
              if (SDL_PointInRect(&mousePoint, &sliderSpeed.knob)) {
                sliderSpeed.dragging = true;
              } else if (SDL_PointInRect(&mousePoint, &sliderDirection.knob)) {
                sliderDirection.dragging = true;
              } else if (SDL_PointInRect(&mousePoint, &sliderSize.knob)) {
                sliderSize.dragging = true;
              } else if (SDL_PointInRect(&mousePoint, &sliderWeight.knob)) {
                sliderWeight.dragging = true;
              } else {
                std::unique_ptr<Circle> userCircle = std::make_unique<Circle>(
                    Vec(e.button.x, e.button.y),
                    100 * std::max(sliderSize.value, static_cast<float>(0.1)),
                    Vec(sliderSpeed.value *
                            std::cos(M_PI * sliderDirection.value),
                        sliderSpeed.value) *
                        std::sin(M_PI * sliderDirection.value),
                    10 * std::max(sliderWeight.value, static_cast<float>(0.1)),
                    0.8);

                shapes.push_back(std::move(userCircle));
              }
            }
          } else if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
              sliderSpeed.dragging = false;
              sliderDirection.dragging = false;
              sliderSize.dragging = false;
              sliderWeight.dragging = false;
            }
          } else if (e.type == SDL_MOUSEMOTION) {
            if (sliderSpeed.dragging) {
              sliderSpeed.updateSlider(sliderSpeed, e.motion.x);
            }
            if (sliderDirection.dragging) {
              sliderDirection.updateSlider(sliderDirection, e.motion.x);
            }
            if (sliderSize.dragging) {
              sliderSize.updateSlider(sliderSize, e.motion.x);
            }
            if (sliderWeight.dragging) {
              sliderWeight.updateSlider(sliderWeight, e.motion.x);
            }
          }
        }

        // Calculate deltaTime for movement
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime =
            (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

        // Process collisions
        for (std::size_t i = 0; i < shapes.size(); i++) {
          for (std::size_t j = i + 1; j < shapes.size(); j++) {
            if (shapes[i]->mass != INFINITE_MASS) {
              shapes[i]->velocity.y += (9.8 / SPEED_SCALE) * deltaTime;
            }

            if (shapes[j]->mass != INFINITE_MASS) {
              shapes[j]->velocity.y += (9.8 / SPEED_SCALE) * deltaTime;
            }

            shapes[i]->collidesWith(*shapes[j]);
          }
          shapes[i]->move();
        }

        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        for (std::size_t i = 0; i < shapes.size(); i++) {
          if (auto circlePtr = dynamic_cast<Circle *>(shapes[i].get())) {
            for (double i = 0; i < 2 * M_PI; i += 0.001) {
              SDL_RenderDrawPoint(
                  gRenderer,
                  circlePtr->center.x + circlePtr->radius * std::cos(i),
                  circlePtr->center.y + circlePtr->radius * std::sin(i));
            }
          } else if (auto rectPtr =
                         dynamic_cast<Rectangle *>(shapes[i].get())) {
            SDL_Rect recColored = {
                (int)rectPtr->tlPoint.x, (int)rectPtr->tlPoint.y,
                (int)(rectPtr->brPoint.x - rectPtr->tlPoint.x),
                (int)(rectPtr->brPoint.y - rectPtr->tlPoint.y)};
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderDrawRect(gRenderer, &recColored);
          }
        }

        sliderSpeed.draw();
        sliderDirection.draw();
        sliderSize.draw();
        sliderWeight.draw();

        // Render current frame
        gTextTextures[0]->render(800, 150, gRenderer);
        gTextTextures[1]->render(800, 250, gRenderer);
        gTextTextures[2]->render(800, 350, gRenderer);
        gTextTextures[3]->render(800, 450, gRenderer);

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}