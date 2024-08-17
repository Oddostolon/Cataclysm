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

        int getWidth();
        int getHeight();

    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
        SDL_Renderer** mRenderer;
};