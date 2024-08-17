#include <SDL2/SDL.h>
#include <string>

class LTexture
{
    public: 
        LTexture(SDL_Renderer** renderer);
        ~LTexture();

        bool loadFromFile( std::string path );

        void free();

        void render( int x, int y, SDL_Rect* clip = NULL );

        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
        SDL_Renderer** mRenderer;
};