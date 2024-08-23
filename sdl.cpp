#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"
#include "LButton.h"
#include "constants.h"

#pragma region FUNCTION_DECLARATIONS
bool init();
bool loadMedia();
void close();
#pragma endregion

SDL_Window* 	gWindow 	= NULL;
SDL_Renderer* 	gRenderer 	= NULL; 

Mix_Music* 		gMusic 		= NULL; 

Mix_Chunk* 		gScratch 	= NULL; 
Mix_Chunk* 		gHigh 		= NULL;
Mix_Chunk* 		gMedium 	= NULL;
Mix_Chunk* 		gLow 		= NULL;

LTexture		gPromptTexture( &gRenderer );

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
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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

	if( !gPromptTexture.loadFromFile( "prompt.png" ) )
	{
		printf( "Failed to load prompt texture!\n" );
		success = false;
	}

	gMusic = Mix_LoadMUS( "beat.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gScratch = Mix_LoadWAV( "scratch.wav" );
	if( gScratch == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gHigh = Mix_LoadWAV( "high.wav" );
	if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gMedium = Mix_LoadWAV( "medium.wav" );
	if( gMedium == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gLow = Mix_LoadWAV( "low.wav" );
	if( gLow == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	return success; 
}

void close()
{
	gPromptTexture.free();

	Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( gMedium );
	Mix_FreeChunk( gLow );

	gScratch = NULL; 
	gHigh = NULL; 
	gMedium = NULL;
	gLow = NULL; 

	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
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
					else if( e.type == SDL_KEYDOWN )
					{
						switch ( e.key.keysym.sym )
						{
							case SDLK_1:
								Mix_PlayChannel( -1, gHigh, 0 );
								break;
							
							case SDLK_2: 
								Mix_PlayChannel( -1, gMedium, 0 );
								break;

							case SDLK_3:
								Mix_PlayChannel( -1, gLow, 0 );
								break;
							
							case SDLK_4:
								Mix_PlayChannel( -1, gScratch, 0 );
								break;

							case SDLK_9:
								if( Mix_PlayingMusic() == 0 )
								{
									Mix_PlayMusic( gMusic, -1 );
								}
								else 
								{
									if( Mix_PausedMusic() == 1 )
									{
										Mix_ResumeMusic();
									}
									else
									{
										Mix_PauseMusic();
									}
								}
								break;
							
							case SDLK_0:
								Mix_HaltMusic();
								break;
						}
					}
				}

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				gPromptTexture.render( 0, 0 );
				
                SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}