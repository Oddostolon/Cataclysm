#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class LTexture
{
    public: 
        LTexture( SDL_Renderer** renderer );
        ~LTexture();

        bool loadFromFile( std::string path );

        void free();

        bool loadFromRenderedText( TTF_Font* font, std::string textureText, SDL_Color textColor );

        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        int getWidth();
        int getHeight();
        SDL_Texture* getTexture();

    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
        SDL_Renderer** mRenderer;
};