//
//  Game.hpp
//  SDL
//
//  Created by Erik Morris on 5/4/24.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>


namespace ERK
{
bool init();
//SDL_Texture * loadTexture(std::string path);
//class LTexture;
//bool loadMedia(std::string path);
void gameLoop();
void quit();
}

#endif /* Game_hpp */
