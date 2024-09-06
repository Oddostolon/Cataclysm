#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include "wrappers/LTexture.h"
#include "wrappers/LButton.h"
#include "wrappers/LTimer.h"
#include "constants.h"
#include "Dot.h"

#pragma region FUNCTION_DECLARATIONS
bool init();
bool loadMedia();
void close();
#pragma endregion

SDL_Window* 	gWindow 	= NULL;
SDL_Renderer* 	gRenderer 	= NULL; 

LTexture		gPromptTextTexture( &gRenderer );
LTexture		gInputTextTexture( &gRenderer );
TTF_Font*		gFont;

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
		gWindow = SDL_CreateWindow( "Cataclysm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

	return success; 
}

void close()
{
	gPromptTextTexture.free();
	gInputTextTexture.free();

	TTF_CloseFont( gFont );
	gFont = NULL; 

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
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;

			SDL_Event e;

			SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0xFF};

			std::string inputText = "Some Text";

			gPromptTextTexture.loadFromRenderedText( gFont, "Enter Text:", textColor );
			gInputTextTexture.loadFromRenderedText( gFont, inputText.c_str(), textColor );
			
			while( !quit )
			{
				bool renderText = false;

				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
					{
						inputText.pop_back();
						renderText = true;
					}
					else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
					{
						SDL_SetClipboardText( inputText.c_str() );
					}
					else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
					{
						char* tempText = SDL_GetClipboardText();
						inputText = tempText; 
						SDL_free( tempText);

						renderText = true;
					}
					else if( e.type == SDL_TEXTINPUT )
					{
						if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' )))
						{
							inputText += e.text.text;
							renderText = true;
						}
					}
				}

				if( renderText )
				{
					if( inputText.length() > 0 )
					{
						gInputTextTexture.loadFromRenderedText( gFont, inputText.c_str(), textColor );
					}
					else 
					{
						gInputTextTexture.loadFromRenderedText( gFont, " ", textColor );
					}
				}

				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
				SDL_RenderClear( gRenderer );

				gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
                gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );

                SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}