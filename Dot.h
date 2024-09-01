#pragma once

#include <SDL2/SDL_events.h>
#include "wrappers/LTexture.h"

class Dot
{
    public:

        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;
        static const int DOT_VEL = 10;

        Dot( LTexture* texture );

        void handleEvent( SDL_Event& e );
        void move( SDL_Rect& wall );
        void render();

    private: 

        int mPosX, mPosY;
        int mVelX, mVelY;

        SDL_Rect mCollider;
        LTexture* mDotTexture;

        bool checkCollision( SDL_Rect collidingObject );
};