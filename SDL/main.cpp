//
//  main.cpp
//  SDL
//
//  Created by Erik Morris on 5/4/24.
//

#include <stdio.h>
#include "Game.hpp"



int main(int argc, char *args[])
{
    if(!ERK::init())
        printf("Init not loaded properly");
    else
    {
        ERK::gameLoop();
    }
    ERK::quit();
}
