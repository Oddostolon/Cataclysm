#include "Dot.h"
#include "constants.h"

Dot::Dot (std::shared_ptr<LTexture> texture, int x, int y)
{
  mPosX = x;
  mPosY = y;

  mColliders.resize (11);

  mVelX = 0;
  mVelY = 0;

  mColliders[0].w = 6;
  mColliders[1].w = 10;
  mColliders[2].w = 14;
  mColliders[3].w = 16;
  mColliders[4].w = 18;
  mColliders[5].w = 20;
  mColliders[6].w = 18;
  mColliders[7].w = 16;
  mColliders[8].w = 14;
  mColliders[9].w = 10;
  mColliders[10].w = 6;

  mColliders[0].h = 1;
  mColliders[1].h = 1;
  mColliders[2].h = 1;
  mColliders[3].h = 2;
  mColliders[4].h = 2;
  mColliders[5].h = 6;
  mColliders[6].h = 2;
  mColliders[7].h = 2;
  mColliders[8].h = 1;
  mColliders[9].h = 1;
  mColliders[10].h = 1;

  shiftColliders ();

  mDotTexture = texture;
}

void
Dot::handleEvent (SDL_Event &e)
{
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
      switch (e.key.keysym.sym)
        {
        case SDLK_w:
          mVelY -= DOT_VEL;
          break;
        case SDLK_s:
          mVelY += DOT_VEL;
          break;
        case SDLK_a:
          mVelX -= DOT_VEL;
          break;
        case SDLK_d:
          mVelX += DOT_VEL;
          break;
        }
    }
  else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
      switch (e.key.keysym.sym)
        {
        case SDLK_w:
          mVelY += DOT_VEL;
          break;
        case SDLK_s:
          mVelY -= DOT_VEL;
          break;
        case SDLK_a:
          mVelX += DOT_VEL;
          break;
        case SDLK_d:
          mVelX -= DOT_VEL;
          break;
        }
    }
}

void
Dot::move () // std::vector<SDL_Rect>& otherColliders )
{
  mPosX += mVelX;
  shiftColliders ();

  if ((mPosX < 0) || (mPosX + DOT_WIDTH > LEVEL_WIDTH)) // || checkCollision( otherColliders ) )
    {
      mPosX -= mVelX;
      shiftColliders ();
    }

  mPosY += mVelY;
  shiftColliders ();

  if ((mPosY < 0) || (mPosY + DOT_WIDTH > LEVEL_HEIGHT)) // || checkCollision( otherColliders ) )
    {
      mPosY -= mVelY;
      shiftColliders ();
    }
}

void
Dot::render (int camX, int camY)
{
  mDotTexture->render (mPosX - camX, mPosY - camY);
}

bool
Dot::checkCollision (std::vector<SDL_Rect> &otherColliders)
{
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  for (int myBox = 0; myBox < mColliders.size (); myBox++)
    {
      leftA = mColliders[myBox].x;
      rightA = mColliders[myBox].x + mColliders[myBox].w;
      topA = mColliders[myBox].y;
      bottomA = mColliders[myBox].y + mColliders[myBox].h;

      for (int otherBox = 0; otherBox < otherColliders.size (); otherBox++)
        {
          leftB = otherColliders[otherBox].x;
          rightB = otherColliders[otherBox].x + otherColliders[otherBox].w;
          topB = otherColliders[otherBox].y;
          bottomB = otherColliders[otherBox].y + otherColliders[otherBox].h;

          if (((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) == false)
            {
              return true;
            }
        }
    }

  return false;
}

void
Dot::shiftColliders ()
{
  int r = 0;

  for (int set = 0; set < mColliders.size (); ++set)
    {
      mColliders[set].x = mPosX + (DOT_WIDTH - mColliders[set].w) / 2;
      mColliders[set].y = mPosY + r;
      r += mColliders[set].h;
    }
}

std::vector<SDL_Rect> &
Dot::getColliders ()
{
  return mColliders;
}

int
Dot::getPosX ()
{
  return mPosX;
}

int
Dot::getPosY ()
{
  return mPosY;
}
