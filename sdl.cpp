#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "constants.h"

#pragma region FUNCTION_DECLARATIONS
bool init();
bool loadMedia();
void close();
#pragma endregion

SDL_Window* 	gWindow 	= NULL;
SDL_Renderer* 	gRenderer 	= NULL; 

TTF_Font* 		gFont 		= NULL;

LTexture		gS_PromptTexture( &gRenderer );
LTexture 		gP_PromptTexture( &gRenderer );
LTexture		gTimeTexture( &gRenderer );

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow( " SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n:", SDL_GetError() );
			success = false;
		}
		else 
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags))
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else 
	{
		SDL_Color textColor = { 255, 255, 255, 255 };

		if( !gS_PromptTexture.loadFromRenderedText( gFont, "Press S to start/stop the timer.", textColor ) )
		{
			printf( "Unable to render s_prompt texture!\n" );
			success = false;
		}
		if( !gP_PromptTexture.loadFromRenderedText( gFont, "Press P to pause/resume the timer.", textColor ) )
		{
			printf( "Unable to render p_prompt texture!\n" );
			success = false;
		}
	}

	return success; 
}

void close()
{
	gS_PromptTexture.free();
	gP_PromptTexture.free();
	gTimeTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			SDL_Color textColor = { 255, 255, 255, 255 };

			LTimer timer;

			std::stringstream timeText;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						if( e.key.keysym.sym == SDLK_s )
						{
							if( timer.isStarted() )
							{
								timer.stop();
							}
							else 
							{
								timer.start();
							}
						}
						else if( e.key.keysym.sym == SDLK_p )
						{
							if( timer.isPaused() )
							{
								timer.resume();
							}
							else 
							{
								timer.pause();
							}
						}
					}
				}

				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
				SDL_RenderClear( gRenderer );

				timeText.str("");
				timeText << "Seconds since start time " << ( timer.getTicks() / 1000.f );

				if( !gTimeTexture.loadFromRenderedText( gFont, timeText.str().c_str(), textColor ) )
				{
					printf( "Unable to render time texture!\n " );
				}

				gS_PromptTexture.render( ( SCREEN_WIDTH - gS_PromptTexture.getWidth() ) / 2, 0 );
				gP_PromptTexture.render( ( SCREEN_WIDTH - gS_PromptTexture.getWidth() ) / 2, gS_PromptTexture.getHeight() + ( gS_PromptTexture.getHeight() / 2 ) );

				gTimeTexture.render( ( SCREEN_WIDTH - gTimeTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTimeTexture.getHeight() ) / 2 );
				
                SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}