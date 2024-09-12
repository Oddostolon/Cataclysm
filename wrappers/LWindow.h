#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <memory>

class LWindow
{
    public: 

        LWindow();

        bool init();

        bool createRenderer();

        void handleEvent( SDL_Event& e );

        int getWidth();
        int getHeight();
        std::shared_ptr<SDL_Renderer> getRenderer();

        bool hasMouseFocus();
        bool hasKeyboardFocus();
        bool isMinimized();

    private: 

        std::shared_ptr<SDL_Window> mWindow;
        std::shared_ptr<SDL_Renderer> mRenderer;

        int mWidth;
        int mHeight;

        bool mMouseFocus;
        bool mKeyboardFocus;
        bool mFullScreen;
        bool mMinimized;
};