/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL; 
SDL_Texture* gTexture = NULL;

SDL_Texture* loadTexture( std::string path );

SDL_Rect gSpriteClips[ 4 ];
LTexture gSpriteSheetTexture(&gRenderer);

bool init()
{
	//Initialization flag
	bool success = true;

	printf("init started");
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
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
			}
		}
	}

	printf("init done");

	return success;
}

SDL_Texture* loadTexture( std::string path )
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool loadMedia()
{
	bool success = true;

	if( !gSpriteSheetTexture.loadFromFile( "sprites.png" ))
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
		gSpriteClips[ 0 ].x = 	0;
		gSpriteClips[ 0 ].y = 	0;
		gSpriteClips[ 0 ].w = 100;
		gSpriteClips[ 0 ].h = 100;

		gSpriteClips[ 1 ].x = 100;
		gSpriteClips[ 1 ].y = 	0;
		gSpriteClips[ 1 ].w = 100;
		gSpriteClips[ 1 ].h = 100;

		gSpriteClips[ 2 ].x = 	0;
		gSpriteClips[ 2 ].y = 100;
		gSpriteClips[ 2 ].w = 100;
		gSpriteClips[ 2 ].h = 100;
		
		gSpriteClips[ 3 ].x = 100;
		gSpriteClips[ 3 ].y = 100;
		gSpriteClips[ 3 ].w = 100;
		gSpriteClips[ 3 ].h = 100;
	}

	return success; 
}

void close()
{
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
				}

				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
				SDL_RenderClear( gRenderer );

				gSpriteSheetTexture.render( 0, 0, &gSpriteClips[ 0 ] );

				gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 1 ].w, 0, &gSpriteClips[ 1 ] );

				gSpriteSheetTexture.render( 0, SCREEN_HEIGHT - gSpriteClips[ 2 ].h, &gSpriteClips[ 2 ] );

				gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 3 ].w, SCREEN_HEIGHT - gSpriteClips[ 3 ].h, &gSpriteClips[ 3 ] );

                //Update screen
                SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
