#pragma once

#include <SDL2/SDL_events.h>
#include <vector>
#include <memory>
#include "wrappers/LTexture.h"


class Dot
{
    public:

        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;
        static const int DOT_VEL = 1;

        Dot( std::shared_ptr<LTexture> texture, int x, int y );

        void handleEvent( SDL_Event& e );
        void move();// std::vector<SDL_Rect>& otherColliders );
        void render( int camX, int camY );

        std::vector<SDL_Rect>& getColliders();
        int getPosX();
        int getPosY();

    private: 

        int mPosX, mPosY;
        int mVelX, mVelY;

        std::vector<SDL_Rect> mColliders;
        std::shared_ptr<LTexture> mDotTexture;

        bool checkCollision( std::vector<SDL_Rect>& otherColliders );

        void shiftColliders();
};