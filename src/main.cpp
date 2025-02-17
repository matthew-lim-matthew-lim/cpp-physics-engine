#include "Shapes/Circle.hpp"
#include "Shapes/Rectangle.hpp"
#include "Shapes/Shape.hpp"
#include "Utility/Vec.hpp"

// Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
      }
    }
  }

  return success;
}

bool loadMedia() {
  // Loading success flag
  bool success = true;

  // Nothing to load
  return success;
}

void close() {
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
          Vec(100, 300), Vec(500, 400), Vec(0, 0), INFINITE_MASS, 1);

      std::unique_ptr<Rectangle> movingRec = std::make_unique<Rectangle>(
          Vec(300, 100), Vec(400, 150), Vec(0, 0.2), 10, 0.7);

      std::unique_ptr<Circle> movingCircle =
          std::make_unique<Circle>(Vec(200, 100), 50, Vec(0, 0), 10, 0.9);

      Uint64 NOW = SDL_GetPerformanceCounter();
      Uint64 LAST = 0;
      double deltaTime = 0;

      std::vector<std::unique_ptr<Shape>> shapes;
      shapes.push_back(std::move(staticRec));
      shapes.push_back(std::move(movingRec));
      // shapes.push_back(std::move(movingCircle));

      // While application is running
      while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
          // User requests quit
          if (e.type == SDL_QUIT) {
            quit = true;
          }
        }

        // Calculate deltaTime for movement
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime =
            (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

        // Process collisions
        for (std::size_t i = 0; i < shapes.size(); i++) {
          // std::cout << i << std::endl;
          for (std::size_t j = i + 1; j < shapes.size(); j++) {
            if (!shapes[i]->collidesWith(*shapes[j])) {
              if (shapes[i]->mass != INFINITE_MASS) {
                shapes[i]->velocity.y += (9.8 / SPEED_SCALE) * deltaTime;
              }

              if (shapes[j]->mass != INFINITE_MASS) {
                shapes[j]->velocity.y += (9.8 / SPEED_SCALE) * deltaTime;
              }
            }
            shapes[j]->move();
          }
          shapes[i]->move();
        }

        // std::cout << "Hello!" << std::endl;

        // // Accelerate the moving objects.
        // if (collidesWith(movingRec, staticRec)) {
        //   movingRec.velocity = Vec(0, 0);
        // } else {
        //   movingRec.velocity.y += 9.8 / SPEED_SCALE * deltaTime;
        //   movingRec.min.y += movingRec.velocity.y;
        //   movingRec.max.y += movingRec.velocity.y;
        // }

        // if (collidesWith(movingCircle, staticRec)) {
        //   movingCircle.velocity = Vec(0, 0);
        // } else {
        //   movingCircle.velocity.y += 9.8 / SPEED_SCALE * deltaTime;
        //   movingCircle.center.y += movingCircle.velocity.y;
        // }

        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        for (std::size_t i = 0; i < shapes.size(); i++) {
          // std::cout << i << std::endl;
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
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawRect(gRenderer, &recColored);
          }
        }

        // Draw Circle

        // // Render red filled quad
        // SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4,
        //                      SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

        // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
        // SDL_RenderFillRect(gRenderer, &fillRect);

        // // Render green outlined quad
        // SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6,
        //                         SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};

        // SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
        // SDL_RenderDrawRect(gRenderer, &outlineRect);

        // // Draw blue horizontal line
        // SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
        // SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH,
        //                    SCREEN_HEIGHT / 2);

        // // Draw vertical line of yellow dots
        // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
        // for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
        //   SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
        // }

        // // Pink Circle
        // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x69, 0xB4, 0xFF);
        // // y = r sin(theta)
        // // x = y cos(theta)
        // int radius = std::min(SCREEN_HEIGHT, SCREEN_WIDTH) / 2;
        // for (double i = 0; i < 2 * M_PI; i += 0.001) {
        //   SDL_RenderDrawPoint(gRenderer,
        //                       SCREEN_WIDTH / 2 + radius * std::cos(i),
        //                       SCREEN_HEIGHT / 2 + radius * std::sin(i));
        // }

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}