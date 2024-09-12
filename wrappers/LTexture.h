#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

class LTexture
{
    public: 
    
        LTexture( std::shared_ptr<SDL_Renderer> renderer );
        ~LTexture();

        bool loadFromFile( std::string path );

        void free();

        bool loadFromRenderedText( TTF_Font* font, std::string textureText, SDL_Color textColor );

        void render( int x, int y, std::shared_ptr<SDL_Rect> clip = NULL, double angle = 0.0, std::shared_ptr<SDL_Point> center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        int getWidth();
        int getHeight();
        std::shared_ptr<SDL_Texture> getTexture();

    private:
        
        int mWidth;
        int mHeight;
        std::shared_ptr<SDL_Texture> mTexture;
        std::shared_ptr<SDL_Renderer> mRenderer;
};