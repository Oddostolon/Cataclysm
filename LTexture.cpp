#include "LTexture.h"
#include <SDL2/SDL_image.h>

LTexture::LTexture(SDL_Renderer** renderer)
{
    mTexture = NULL;
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

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( *mRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else 
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface( loadedSurface );
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( TTF_Font* font, std::string textureText, SDL_Color textColor )
{
    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else 
    {
        mTexture = SDL_CreateTextureFromSurface( *mRenderer, textSurface );
        if( mTexture == NULL )
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

    bool success = mTexture != NULL;

    return mTexture != NULL;
}
#endif

void LTexture::render( int x, int y, SDL_Rect* clip /*= NULL*/, double angle /*= 0.0*/, SDL_Point* center /*= NULL*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    if ( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    SDL_RenderCopyEx( *mRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

void LTexture::free()
{
    if ( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}