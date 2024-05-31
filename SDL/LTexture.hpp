//
//  LTexture.hpp
//  SDL
//
//  Created by Erik Morris on 5/7/24.
//

#ifndef LTexture_hpp
#define LTexture_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <string>

class LTexture
{
public:
    LTexture();
    ~LTexture();
    bool loadFromFile(SDL_Renderer* renderer, std::string);
    
    //Creates image from font string
    bool loadFromRenderedText(SDL_Renderer * renderer, TTF_Font * gFont ,std::string textureText, SDL_Color textColor );
    
    //Deallocates texture
    void free();
    
    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue );
    
    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void setAlpha( Uint8 alpha );

    //Renders texture at given point
    void render(SDL_Renderer * renderer, SDL_Rect* clip = NULL, SDL_FRect* pos = NULL, double angle = 0.0, SDL_FPoint* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );


    //Gets image dimensions
    int getWidth();
    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};





#endif /* LTexture_hpp */
