//
//  Button.hpp
//  SDL
//
//  Created by Erik Morris on 5/31/24.
//

#ifndef Button_hpp
#define Button_hpp

#include <stdio.h>
#include "LTexture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>


class LButton
{
public:
    //Initializes internal variables
    LButton();
    LButton(float x, float y,float w,float h);
    ~LButton();

    //Sets top left position
    void setPosition( float x, float y );
    
    bool loadTextures(SDL_Renderer * renderer, std::string buttonBackground, std::string buttonText, TTF_Font *gFont, SDL_Color textColor);

    //Handles mouse event
    void handleEvent( SDL_Event* e , bool & pressed);

    //Shows button sprite
    void render(SDL_Renderer * renderer);
    
    void free();

private:
    //Top left position
    //SDL_Point mPosition;
    SDL_FRect buttonInfo;

    //Currently used global sprite
    LTexture texture;
    
    //Text
    LTexture text;
};


#endif /* Button_hpp */
