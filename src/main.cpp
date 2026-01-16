#include "Shapes/Circle.hpp"
#include "Shapes/Rectangle.hpp"
#include "Shapes/Shape.hpp"
#include "Ui/Slider.hpp"
#include "Utility/LTexture.hpp"
#include "Utility/Vec.hpp"
#include "Ui/UI.hpp"

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
#include <iostream>
#include <optional>

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Starts up SDL and creates window
bool init();


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

void close() {
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

Vec cameraOffset;

int main(int, char *[]) {
  // Start up SDL and create window
  if (!init()) {
    std::cerr << "Failed to init SDL" << std::endl;
  } else {
    // Main loop flag
    bool quit = false;

    // Event handler
    std::unique_ptr<Circle> displayShape =
        std::make_unique<Circle>(Vec(500, 500), 60, Vec(0, 0), INFINITE_MASS, 0.8);

    std::unique_ptr<Rectangle> staticRec = std::make_unique<Rectangle>(
        Vec(50, 600), Vec(550, 650), Vec(0, 0), INFINITE_MASS, 1);

    std::unique_ptr<Rectangle> movingRec = std::make_unique<Rectangle>(
        Vec(300, 100), Vec(400, 150), Vec(0, 0), 50, 0.7);

    std::unique_ptr<Circle> movingCircle =
        std::make_unique<Circle>(Vec(200, 100), 60, Vec(0, 0), 10, 0.8);

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::move(displayShape));
    shapes.push_back(std::move(staticRec));
    shapes.push_back(std::move(movingRec));
    shapes.push_back(std::move(movingCircle));

    // load the media (sliders and display)
    UI ui(gRenderer, cameraOffset);

    // While application is running
    while (!quit) {
      ui.processPollEvents(quit, shapes);
      ui.loadMedia();

      // Calculate deltaTime for movement
      LAST = NOW;
      NOW = SDL_GetPerformanceCounter();
      deltaTime =
          (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

      // Process collisions
      // Skip `shapes[0]` as that is the display shape (shape on user's cursor)
      for (std::size_t i = 1; i < shapes.size(); i++) {
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
      SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

      for (std::size_t i = 0; i < shapes.size(); i++) {
        if (auto circlePtr = dynamic_cast<Circle *>(shapes[i].get())) {
          for (double i = 0; i < 2 * M_PI; i += 0.001) {
            SDL_RenderDrawPoint(
                gRenderer,
                circlePtr->center.x + (int)cameraOffset.x + circlePtr->radius * std::cos(i),
                circlePtr->center.y + (int)cameraOffset.y + circlePtr->radius * std::sin(i));
          }
        } else if (auto rectPtr =
                        dynamic_cast<Rectangle *>(shapes[i].get())) {
          SDL_Rect recColored = {
              (int)rectPtr->tlPoint.x + (int)cameraOffset.x, (int)rectPtr->tlPoint.y + (int)cameraOffset.y,
              (int)(rectPtr->brPoint.x - rectPtr->tlPoint.x),
              (int)(rectPtr->brPoint.y - rectPtr->tlPoint.y)};
          SDL_RenderDrawRect(gRenderer, &recColored);
        }
      }

      ui.drawMedia();

      // Update screen
      SDL_RenderPresent(gRenderer);
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}