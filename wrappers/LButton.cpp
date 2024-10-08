#include "LButton.h"

LButton::LButton ()
{
  mPosition.x = 0;
  mPosition.y = 0;

  mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
  mButtonSpriteClip->w = BUTTON_WIDTH;
  mButtonSpriteClip->h = BUTTON_HEIGHT;
  mButtonSpriteClip->x = 0;
}

void
LButton::setPosition (int x, int y)
{
  mPosition.x = x;
  mPosition.y = y;
}

void
LButton::setSpriteSheet (std::shared_ptr<LTexture> spriteSheetTexture)
{
  mButtonSpriteSheetTexture = spriteSheetTexture;
}

void
LButton::render ()
{
  mButtonSpriteSheetTexture->render (mPosition.x, mPosition.y, mButtonSpriteClip);
}

void
LButton::handleEvent (SDL_Event *e)
{
  if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
      int x, y;
      SDL_GetMouseState (&x, &y);

      bool inside = true;

      if (x < mPosition.x)
        {
          inside = false;
        }
      else if (x > mPosition.x + BUTTON_WIDTH)
        {
          inside = false;
        }
      else if (y < mPosition.y)
        {
          inside = false;
        }
      else if (y > mPosition.y + BUTTON_HEIGHT)
        {
          inside = false;
        }

      if (!inside)
        {
          mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
      else
        {
          switch (e->type)
            {
            case SDL_MOUSEMOTION:
              mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
              break;

            case SDL_MOUSEBUTTONDOWN:
              mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
              break;

            case SDL_MOUSEBUTTONUP:
              mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
              break;
            }
        }

      setSpriteClip ();
    }
}

void
LButton::setSpriteClip ()
{
  mButtonSpriteClip->y = BUTTON_HEIGHT * mCurrentSprite;
}