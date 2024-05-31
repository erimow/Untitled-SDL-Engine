//
//  Button.cpp
//  SDL
//
//  Created by Erik Morris on 5/31/24.
//

#include "Button.hpp"

LButton::LButton()
{
    buttonInfo.x = 0;
    buttonInfo.y = 0;
    buttonInfo.w = 50;
    buttonInfo.h = 25;
    //loadTextures("ButtonBackground.png", "Button");
    //mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}
LButton::LButton(float x,float y,float w,float h)
{
    buttonInfo.x = x;
    buttonInfo.y = y;
    buttonInfo.w = w;
    buttonInfo.h = h;
    //loadTextures("ButtonBackground.png", "Button");
}

LButton::~LButton()
{
    free();
}

void LButton::setPosition(float x,float y )
{
    buttonInfo.x = x;
    buttonInfo.y = y;
}

bool LButton::loadTextures(SDL_Renderer * renderer, std::string buttonBackground, std::string buttonText, TTF_Font *gFont, SDL_Color textColor)
{
    bool success = true;
    if (!texture.loadFromFile(renderer, buttonBackground))
    {
        printf("Failed to load buttonBackground!\n");
        success = false;
    }
    if (!text.loadFromRenderedText(renderer, gFont, buttonText, textColor))
    {
        printf("Failed to load buttonText!\n");
        success = false;
    }
     
    return success;
}

void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < buttonInfo.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > buttonInfo.x + buttonInfo.w)
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < buttonInfo.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > buttonInfo.y + buttonInfo.h)
        {
            inside = false;
        }
        //Mouse is outside button
        if( !inside )
        {
            texture.setColor(255, 255, 255);
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                    texture.setColor(200, 200, 200);
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                    texture.setColor(130, 130, 130);
                break;
                
                case SDL_MOUSEBUTTONUP:
                    texture.setColor(200, 200, 200);
                break;
            }
        }
    }
}
void LButton::render(SDL_Renderer *renderer)
{
    //Show current button sprite
    texture.render(renderer, NULL, &buttonInfo);
    text.render(renderer, NULL, &buttonInfo);
}


void LButton::free()
{
    texture.free();
    text.free();
}
