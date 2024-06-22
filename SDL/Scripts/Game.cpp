//
//  Game.cpp
//  SDL
//
//  Created by Erik Morris on 5/4/24.
//

#include "Game.hpp"
#include "LTexture.hpp"
#include "Button.hpp"
#include "Timer.hpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int targetFps = 60;
const int ticksPerFrame = 1000.0f/targetFps;
SDL_Window * window = nullptr;
SDL_Renderer * renderer = nullptr;
LTexture playerArt[3];
LTexture currentRender;
SDL_FPoint cameraPos;
TTF_Font * gFont = NULL;
LTexture fontTexture;
LButton button((SCREEN_WIDTH/10), 25, SCREEN_WIDTH/10, SCREEN_HEIGHT/10);
bool isButtonPressed = false;
Mix_Chunk* jumpEffect = NULL;
Mix_Music* gameMusic = NULL;
SDL_Joystick * gamePad = NULL;
const int JOYSTICK_DEADZONE = 11000;
Uint8 currentControl = NULL;
Timer fpstimer;
Timer capTimer;
LTexture fpsTexture;
int frameCount = 0;




//REMEMBER TO FREE DECLARATIONS!!!!



//SDL_Texture * ERK::loadTexture (std::string path)
//{
//    SDL_Texture * loadedTexture = IMG_LoadTexture(renderer, path.c_str());
//    return loadedTexture;
//}

enum controlEnum
{
    MAK,
    GAMEPAD
};


bool loadMedia()
{
    bool success = true;
    
    if (!playerArt[0].loadFromFile(renderer, "SDL/Art/Starfish.png"))
    {
        printf("Failed to load Starfish.png texture image!\n");
        success = false;
    }
    if (!playerArt[1].loadFromFile(renderer, "SDL/Art/RelicArt.png"))
    {
        printf("Failed to load RelicArt.png texture image!\n");
        success = false;
    }
    if (!playerArt[2].loadFromFile(renderer, "SDL/Art/StarfishRightRoll.png"))
    {
        printf("Failed to load StarfishRightRoll.png texture image!\n");
        success = false;
    }
    gFont = TTF_OpenFont("SDL/Fonts/tuffy_regular.ttf", 56); //Location and font size;
    if(gFont!= NULL){
        SDL_Color fontCol= {0, 255, 122, 255};
        if(!fontTexture.loadFromRenderedText(renderer, gFont, "Starfish Adventures the Epilogue", fontCol))
        {
            printf("Failed to load Font texture!");
            success = false;
        }
        fontCol = {0, 0, 0, 255};
        if (!button.loadTextures(renderer, "SDL/Art/ButtonBackground.png", "Music", gFont, fontCol))
        {
            printf("Failed to load button texture!");
            success = false;
        }
        
    }else{printf("gFont is equal to NULL!");}
    jumpEffect = Mix_LoadWAV("SDL/Sounds/low.wav");
    if(jumpEffect == NULL)
    {
        printf("Could not set jumpEffect sound! Error: %s\n", Mix_GetError());
    }
//    gameMusic = Mix_LoadMUS("Sounds/game - music 1.wav");
    gameMusic = Mix_LoadMUS("SDL/Sounds/The Penguin God.wav");
    if(gameMusic == NULL)
    {
        printf("Could not set gameMusic! Error: %s\n", Mix_GetError());
    }
    
    return success;
}


void ERK::gameLoop()
{
    fpstimer.start();
    
    //set default control to keyboard and mouse
    currentControl = controlEnum::MAK;
    
   //Level objects
    float ground = 340.0f;
    SDL_FRect box = {350.0f, ground-100.0f, 100.0f, 100.0f};
    
  
    //Font loc
    SDL_FRect textLoc = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT/5};
    //fps display
    SDL_FRect fpsLoc = {SCREEN_WIDTH-SCREEN_WIDTH/8, 5.0f, SCREEN_WIDTH/9, SCREEN_HEIGHT/13};
    std::stringstream fpsText;
    
    //playerstats
     SDL_FRect player = {SCREEN_WIDTH/2-50.0f, SCREEN_HEIGHT/2-50.0f, 100.0f, 100.0f};
    SDL_Rect artBounds = {2, 5, 25, 22};
    float jumpStr = 12.0f;
    float speedX=0;
    float speedY=0;
    float gravity = 0.3f;
    float friction = .95f;
    float runStr=0.8f;
    
    //flags
    int left = 0;
    int right = 0;
    int up = 0;
    bool onGround = false;
    float maxX = 8.0f;

    bool quit = false;
    SDL_Event e;
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
//    SDL_FPoint center = {(float)player.w/2, (float)player.h/2};
    double playerRotation = 0.0;
    float onNub = false; //Used for animation purposes
    
    while (!quit)
    {
        capTimer.start();
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                currentControl = controlEnum::MAK;
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        up = 1;
                        break;
                    case SDLK_DOWN:
                        
                        break;
                    case SDLK_LEFT:
                        left = 1;
                        break;
                    case SDLK_RIGHT:
                        right = 1;
                        break;
                }
            }
            else if (e.type == SDL_KEYUP)
            {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        up = 0;
                        break;
                    case SDLK_DOWN:
                        
                        break;
                    case SDLK_LEFT:
                        left = 0;
                        break;
                    case SDLK_RIGHT:
                        right = 0;
                        break;
                }
            }
            //JOYSTICK CONTROLS
            else if (e.type == SDL_JOYAXISMOTION )
            {
                if (e.jaxis.which == 0) //gamepad at index 0
                {
                    if (e.jaxis.axis == 0) // x axis
                    {
                        if (e.jaxis.value > JOYSTICK_DEADZONE)
                        {
                            right = 1;
                            left = 0;
                            currentControl = controlEnum::GAMEPAD;
                        }
                        else if (e.jaxis.value < -JOYSTICK_DEADZONE)
                        {
                            left = 1;
                            right = 0;
                            currentControl = controlEnum::GAMEPAD;
                        }
                        else
                        {
                            if (currentControl == controlEnum::GAMEPAD)
                            {
                                left = 0;
                                right = 0;
                            }
                        }
                    }
                }
            }
            else if (e.type == SDL_JOYBUTTONDOWN)
            {
                switch (e.jbutton.button)
                {
                    case 0: //A button
                        up=1;
                        currentControl = controlEnum::GAMEPAD;
                        break;
                }
            }
            else if (e.type==SDL_JOYBUTTONUP)
            {
                switch (e.jbutton.button)
                {
                    case 0: //A button
                        up=0;
                        break;
                }
            }
            
            button.handleEvent(&e, isButtonPressed);
        }
        
        /*   FOR USING KEY STATES AS OPPOSED TO EVENTS... NO BUENO
         
        const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
        if (currentKeyState[SDL_SCANCODE_RIGHT])
        {
            right = 1;
            left = 0;
        }
        else if (currentKeyState[SDL_SCANCODE_LEFT])
        {
            right = 0;
            left = 1;
        }
        else if(currentKeyState[SDL_SCANCODE_UP])
        {
            up = 1;
        }
        else
        {
            right = 0;
            left = 0;
            up = 0;
        }
         IGNORE ALL THIS TRASH
        */
        
        //------------ MUSIC STUFF -----------------
        if (Mix_PlayingMusic() == 0)
        {
            if (isButtonPressed)
            {
                Mix_PlayMusic(gameMusic, -1);
            }
        }
        else
        {
            if (!isButtonPressed && (Mix_PausedMusic()==0))
            {
                Mix_PauseMusic();
            }
            else if (isButtonPressed && Mix_PausedMusic()==1)
            {
                Mix_ResumeMusic();
            }
        }
         
         
        //--------------- PLAYER PHYSICS -----------------
        speedX += ((right*runStr) - (left * runStr));
        speedX *= friction;
         if (speedX >= maxX)
            speedX = maxX;
        if(-speedX >= maxX)
            speedX = -maxX;
       //player.x += speedX;
        speedY -= (onGround * up * jumpStr) - gravity;
        //player.y += speedY;
        ground-=speedY;
        box.y = ground -100.0f;
        box.x -= speedX;
        //box.y -= speedY;
        
       if (onGround == true)
           playerRotation += speedX/2;
        
        //--------------- CAMERA ----------------------
//        cameraPos.x = player.x+player.w/2;
//        cameraPos.y = player.y+player.h/2;
        
        //------------- COLLISIONS ---------------
        
        if (((player.y + player.h) >= box.y && (player.y + player.h) <= box.y+10) && (player.x + ((player.w/2)+10)>=box.x && player.x+((player.w/2)-10) <= box.x+box.w) && speedY >= 0)
        {
            if(onGround==0)
                Mix_PlayChannel(-1, jumpEffect, 0);
            speedY = 0;
            ground = (player.y+player.h)+box.h;
            onGround=1;
            
        }
        else if(player.y + player.h >= ground)
        {
            if (onGround == 0)
                Mix_PlayChannel(-1, jumpEffect, 0);
            speedY = 0;
            ground = player.y+player.h;
            onGround=1;
            
        }
        else
        {
            onGround = 0;
        }
        
        
        //----------------------- RENDER STUFF ----------------------------------------
        //Clear screen
        SDL_SetRenderDrawColor( renderer, 0, 190, 0xFF, 0xFF );
        SDL_RenderClear( renderer );
        
        //Load Ground lines
        SDL_SetRenderDrawColor(renderer, 0, 0xFA, 0xF, 0xFA);
        SDL_RenderDrawLine(renderer, 0, (ground-17), SCREEN_WIDTH, (ground-17));
        SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0xF, 0xFF);
        SDL_RenderDrawLine(renderer, 0, ground+30, SCREEN_WIDTH, ground+30);
        
        //THIS WAS AN EXPIRIMENT WITH POINTS AND DRAWLINES
//        SDL_Point points[2];
//        points[0].x = 0;points[0].y = ground-12;points[1].x = 100;points[1].y = ground-35;
//        SDL_RenderDrawLines(renderer, points, 2);
        
        
       //FPS Rendering
        float avgFps = frameCount / (fpstimer.getTicks()/1000.f);
        fpsText.str("");
        fpsText << "fps: " << (avgFps);
        
        if (!fpsTexture.loadFromRenderedText(renderer, gFont, fpsText.str(), {0,0,0,255}))
        {
            printf("Couldn't render fps text!\n");
        }
        fpsTexture.render(renderer, NULL, &fpsLoc);
        
        //Load Cube
        SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
//        SDL_RenderDrawRectF(renderer, &box);
        SDL_RenderFillRectF(renderer, &box);
        
        //Render Player
//        SDL_SetRenderDrawColor( renderer, 0x1, 0xFF, 0xFF, 0xFF );
        if ((((int)playerRotation%360 <-195 && (int)playerRotation%360 > -270) || ((((int)playerRotation%360 >90 && (int)playerRotation%360 < 165)))) && speedX<0 && onGround ){
            if (!onNub){
                currentRender = playerArt[1];
                onNub = true;
            }}
        else if((((int)playerRotation%360 >100 && (int)playerRotation%360 < 180) || ((((int)playerRotation%360 <-180 && (int)playerRotation%360 > -260)))) && speedX>0 && onGround){
            if (!onNub){
                currentRender = playerArt[2];
                onNub = true;
            }
        }
        else{
            currentRender = playerArt[0]; //Custom render function apart of LTexture class
            onNub = false;
        }
        currentRender.setColor(0, 255, 0);
        currentRender.render(renderer, &artBounds, &player, playerRotation, NULL ,flip);
        
        
        fontTexture.render(renderer, NULL, &textLoc);
        
        button.render(renderer);
        
        //Update screen
        SDL_RenderPresent( renderer );
        frameCount++;
        
        //If frame finished early
        int frameTicks = capTimer.getTicks();
        if( frameTicks < ticksPerFrame)
        {
            //Wait remaining time
            SDL_Delay(ticksPerFrame - frameTicks );
        }
    }
}




bool ERK::init()
{
    bool success = false;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO |SDL_INIT_JOYSTICK)<0)
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    else{
       
        window = SDL_CreateWindow("Erik test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window was not loaded! SDL Error : %s\n", SDL_GetError());
        }
        else{
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL)
            {
                printf("Renderer was not loaded! SDL Error : %s\n", SDL_GetError());
            }
            else
            {
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not be initialized! SDL_image Error: %s\n", IMG_GetError());
                }
                
                else{
                    //Init audio stuff
                    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                    {
                        printf("Mix could not init! SDL_Mix Error: %s\n", Mix_GetError());
                    }
                    else
                    {
                        //Initialize SDL_ttf
                        if( TTF_Init() == -1 )
                        {
                            printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                        }
                        else
                        {
                            //LOAD MEDIA
                            if (!loadMedia())
                            {
                                printf("Could not load texture image!\n");
                            }
                            else{
                                //CHECK FOR JOYSTICKS AND SET TEXTURE FILTERING
                                
                                //Set texture filtering to linear
                                if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
                                {
                                    printf( "Warning: Linear texture filtering not enabled!" );
                                }
                                
                                //Check for joysticks
                                if( SDL_NumJoysticks() < 1 )
                                {
                                    printf( "Warning: No joysticks connected!\n" );
                                }
                                else
                                {
                                    //Load joystick
                                    gamePad = SDL_JoystickOpen( 0 );
                                    if( gamePad == NULL )
                                    {
                                        printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
                                    }
                                }
                                success = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return success;
}



void ERK::quit()
{
//    SDL_DestroyTexture(playerArt);
    playerArt[0].free();
    playerArt[1].free();
    playerArt[2].free();
    fpsTexture.free();
    Mix_FreeMusic(gameMusic);
    gameMusic = NULL;
    Mix_FreeChunk(jumpEffect);
    jumpEffect=NULL;
    SDL_JoystickClose(gamePad);
    gamePad=NULL;
    currentRender.free();
    fontTexture.free();
    TTF_CloseFont(gFont);
    gFont = NULL;
    button.free();
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    window = NULL;
    renderer = NULL;
    gFont = NULL;
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
