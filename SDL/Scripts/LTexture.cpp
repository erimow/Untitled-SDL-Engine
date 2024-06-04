//
//  LTexture.cpp
//  SDL
//
//  Created by Erik Morris on 5/7/24.
//

#include "LTexture.hpp"


LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    free();
}


bool LTexture::loadFromFile(SDL_Renderer *renderer, std::string path)
{
    free();
    
    SDL_Texture* newTexture = NULL;
    
    newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == NULL)
    {
        printf("Unable to create new texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else
    {
        SDL_Point size;
        SDL_QueryTexture(newTexture, NULL, NULL, &size.x, &size.y); // Gets information from texture
        mWidth = size.x;
        mHeight = size.y;
    }
    mTexture = newTexture;
    return (mTexture != NULL);
}

bool LTexture::loadFromRenderedText( SDL_Renderer * renderer, TTF_Font * gFont,  std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor );
    
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(renderer , textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}
        
void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(SDL_Renderer * renderer, SDL_Rect* clip, SDL_FRect* pos, double angle, SDL_FPoint* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
//       SDL_FRect renderQuad = { x, y};

       //Set clip rendering dimensions
//       if( clip != NULL )
//       {
//           renderQuad.w = clip->w;
//           renderQuad.h = clip->h;
//       }
    //Set rendering space and render to screen
    SDL_RenderCopyExF(renderer, mTexture, clip, pos, angle, center, flip);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}
