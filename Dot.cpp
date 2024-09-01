#include "Dot.h"
#include "constants.h"

Dot::Dot( LTexture* texture )
{
    mPosX = 0;
    mPosY = 0;

    mVelX = 0;
    mVelY = 0;

    mCollider.w = DOT_WIDTH;
    mCollider.h = DOT_HEIGHT;
    mDotTexture = texture;
}

void Dot::handleEvent( SDL_Event& e )
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_w: mVelY -= DOT_VEL; break;
            case SDLK_s: mVelY += DOT_VEL; break; 
            case SDLK_a: mVelX -= DOT_VEL; break;
            case SDLK_d: mVelX += DOT_VEL; break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_w: mVelY += DOT_VEL; break;
            case SDLK_s: mVelY -= DOT_VEL; break; 
            case SDLK_a: mVelX += DOT_VEL; break;
            case SDLK_d: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move( SDL_Rect& wall )
{
    mPosX += mVelX;
    mCollider.x = mPosX;

    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision( wall ) )
    {
        mPosX -= mVelX;
    }

    mPosY += mVelY;
    mCollider.y = mPosY;

    if( ( mPosY < 0 ) || ( mPosY + DOT_WIDTH > SCREEN_HEIGHT ) || checkCollision( wall ) )
    {
        mPosY -= mVelY;
    }
}

void Dot::render()
{
    mDotTexture->render( mPosX, mPosY);
}

bool Dot::checkCollision( SDL_Rect collidingObject )
{
    int leftA, leftB; 
    int rightA, rightB; 
    int topA, topB; 
    int bottomA, bottomB;

    leftA = mCollider.x;
    rightA = mCollider.x + mCollider.w;
    topA = mCollider.y;
    bottomA = mCollider.y + mCollider.h;

    leftB = collidingObject.x;
    rightB = collidingObject.x + collidingObject.w;
    topB = collidingObject.y;
    bottomB = collidingObject.y + collidingObject.h;

    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    return true;
}
