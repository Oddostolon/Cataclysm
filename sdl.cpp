#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"
#include "LButton.h"
#include "constants.h"

bool init();

SDL_Texture* loadTexture( std::string path );

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL; 
LTexture gArrowTexture( &gRenderer );

SDL_Joystick* gGameController = NULL; 

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!\n" );
		}

		if( SDL_NumJoysticks() < 1 )
		{
			printf( "Warning: No joysticks connected!\n" );
		}
		else
		{
			gGameController = SDL_JoystickOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}
		}

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

	if( !gArrowTexture.loadFromFile( "arrow.png" ) )
	{
		printf( "Couldn't load texture from file" );
		success = false;
	}

	return success; 
}

void close()
{
	gArrowTexture.free();

	SDL_JoystickClose( gGameController );
	gGameController = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
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

			int xDir = 0;
			int yDir = 0;

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
					else if( e.type == SDL_JOYAXISMOTION )
					{
						if( e.jaxis.which == 0 )
						{
							if( e.jaxis.axis == 0 )
							{
								if( e.jaxis.value < -JOYSTICK_DEADZONE )
								{
									xDir = -1;
								}
								else if( e.jaxis.value > JOYSTICK_DEADZONE )
								{
									xDir = 1;
								}
								else 
								{
									xDir = 0;
								}
							}
							else if( e.jaxis.axis == 1 )
							{
								if( e.jaxis.value < -JOYSTICK_DEADZONE )
								{
									yDir = -1;
								}
								else if( e.jaxis.value > JOYSTICK_DEADZONE )
								{
									yDir = 1;
								}
								else 
								{
									yDir = 0;
								}
							}
						}
					}
				}

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				double joystickAngle = atan2( (double)yDir, (double)xDir ) * ( 180.0 / M_PI );
                
                if( xDir == 0 && yDir == 0 )
                {
                    joystickAngle = 0;
                }
				
				gArrowTexture.render( ( SCREEN_WIDTH - gArrowTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gArrowTexture.getHeight() ) / 2, NULL, joystickAngle );

                SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}