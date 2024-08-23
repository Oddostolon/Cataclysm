#pragma once

#include <SDL2/SDL.h>
#include "constants.h"
#include "LTexture.h"

class LButton
{
    public: 

        LButton();

        void setPosition( int x, int y );

        void setSpriteSheet( LTexture* spriteSheetTexture );

        void handleEvent( SDL_Event* e );

        void render();

    private: 

        void setSpriteClip();

        SDL_Point mPosition;

        LTexture* mButtonSpriteSheetTexture;

        SDL_Rect mButtonSpriteClip;

        enum LButtonSprite
        {
            BUTTON_SPRITE_MOUSE_OUT = 0,
            BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
            BUTTON_SPRITE_MOUSE_DOWN = 2,
            BUTTON_SPRITE_MOUSE_UP = 3,
            BUTTON_SPRITE_TOTAL = 4
        };

        LButtonSprite mCurrentSprite;
};