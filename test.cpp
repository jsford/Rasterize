#include "SDL/SDL.h"
#include <string>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cassert>
#include "draw.hpp"

#define TIMER

timespec diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

int main( int argc, char* args[] )
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        return 1;
    }

    //Set up the screen
    SDL_Surface * screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_DOUBLEBUF | SDL_SWSURFACE);

    //If there was an error in setting up the screen
    if( screen == NULL ) {
        return 1;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Jordan's Supa Cool Rasterizer", NULL );

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    
    int i = 0;

    SDL_Event event;
    bool quit = false;

    #ifdef TIMER
        float Sum_of_Times = 0;
    #endif

    int frame_count = 0;
    while( !quit ) {

        while (SDL_PollEvent( &event ) );
        if( event.type == SDL_QUIT || ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE ) ) { 
            quit = true; 
        }

        ++frame_count;
        if(frame_count == 10000) {quit = true;}

        #ifdef TIMER
            timespec start, end;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        #endif
        
        render_screen(screen);

        #ifdef TIMER
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

            timespec elapsed = diff(start, end);
            assert(elapsed.tv_sec == 0);
            std::cout <<  elapsed.tv_nsec / 1000.0 << " us" << std::endl;

            Sum_of_Times += elapsed.tv_nsec / 1000.0;
        #endif

        if( SDL_Flip(screen) == -1 ) { return 1; }        
    }

    #ifdef TIMER
        std::cout << "Average_Time = " << Sum_of_Times/frame_count << std::endl;
    #endif

    //Quit SDL
    SDL_Quit();
    return 0;
}