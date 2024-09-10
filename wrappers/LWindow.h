#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

class LWindow
{
    public: 

        LWindow();

        bool init();

        bool createRenderer();

        void handleEvent( SDL_Event& e );
        
        void free();

        int getWidth();
        int getHeight();
        SDL_Renderer** getRenderer();

        bool hasMouseFocus();
        bool hasKeyboardFocus();
        bool isMinimized();

    private: 

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        int mWidth;
        int mHeight;

        bool mMouseFocus;
        bool mKeyboardFocus;
        bool mFullScreen;
        bool mMinimized;
};