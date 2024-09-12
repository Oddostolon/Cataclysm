#include "LTexture.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <memory>

LTexture::LTexture(std::shared_ptr<SDL_Renderer> renderer)
{
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
    mRenderer = renderer;
}

LTexture::~LTexture()
{
    free();
}

bool LTexture::loadFromFile( std::string path)
{
    free();

    auto loadedSurface = std::shared_ptr<SDL_Surface>(IMG_Load( path.c_str() ), SDL_FreeSurface);
    if( !loadedSurface )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey( loadedSurface.get(), SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        mTexture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
            SDL_CreateTextureFromSurface( mRenderer.get(), loadedSurface.get() ), 
            SDL_DestroyTexture);

        if( !mTexture )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else 
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
    }

    return mTexture != nullptr;
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    SDL_SetTextureColorMod( mTexture.get(), red, green, blue );
}

void LTexture::setRenderer( std::shared_ptr<SDL_Renderer> renderer )
{
    mRenderer = renderer;
}

bool LTexture::loadFromRenderedText( TTF_Font* font, std::string textureText, SDL_Color textColor )
{
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
    if( textSurface == nullptr )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else 
    {
        mTexture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
            SDL_CreateTextureFromSurface(mRenderer.get(), textSurface), 
            SDL_DestroyTexture);
        if( !mTexture )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else 
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        SDL_FreeSurface( textSurface );
    }

    bool success = mTexture != nullptr;

    return mTexture != nullptr;
}

void LTexture::render( int x, int y, std::shared_ptr<SDL_Rect> clip /*= nullptr*/, double angle /*= 0.0*/, std::shared_ptr<SDL_Point> center /*= nullptr*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    if ( clip )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    SDL_RenderCopyEx( mRenderer.get(), mTexture.get(), clip.get(), &renderQuad, angle, center.get(), flip );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

std::shared_ptr<SDL_Texture> LTexture::getTexture()
{
    return mTexture;
}

void LTexture::free()
{
    mTexture.reset();
    mWidth = 0;
    mHeight = 0;
}