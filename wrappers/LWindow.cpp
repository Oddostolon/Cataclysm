#include "LWindow.h"
#include "../constants.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

LWindow::LWindow ()
{
  mWindow = nullptr;
  mRenderer = nullptr;
  mMouseFocus = false;
  mKeyboardFocus = false;
  mFullScreen = false;
  mMinimized = false;
  mShown = false;
  mWidth = 0;
  mHeight = 0;
}

bool
LWindow::init ()
{
  mWindow = std::shared_ptr<SDL_Window> (
      SDL_CreateWindow ("Cataclysm", SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
      SDL_DestroyWindow);

  if (!mWindow)
    {
      return false;
    }

  mMouseFocus = true;
  mKeyboardFocus = true;
  mWidth = SCREEN_WIDTH;
  mHeight = SCREEN_HEIGHT;
  mWindowId = SDL_GetWindowID (mWindow.get ());
  mShown = true;

  if (!createRenderer ())
    {
      printf ("Failed to create renderer! SDL Error: %s\n", SDL_GetError ());
      return false;
    }

  return mWindow && mRenderer;
}

bool
LWindow::createRenderer ()
{
  bool success = true;

  if (mRenderer)
    {
      return success;
    }

  mRenderer = std::shared_ptr<SDL_Renderer> (
      SDL_CreateRenderer (mWindow.get (), -1,
                          SDL_RENDERER_ACCELERATED
                              | SDL_RENDERER_PRESENTVSYNC),
      SDL_DestroyRenderer);
  if (!mRenderer)
    {
      printf ("Renderer could not be created! SDL Error %s\n",
              SDL_GetError ());
      success = false;
    }
  else
    {
      SDL_SetRenderDrawColor (mRenderer.get (), 0xFF, 0xFF, 0xFF, 0xFF);
    }

  return success;
}

bool
LWindow::initSDLSubsystems ()
{
  bool success = true;

  int imgFlags = IMG_INIT_PNG;
  int initializedFlags = IMG_Init (0);
  if ((imgFlags & initializedFlags) != initializedFlags)
    {
      if (!(IMG_Init (imgFlags) & imgFlags))
        {
          printf ("SDL_image could not initialize! SDL_image Error: %s\n",
                  IMG_GetError ());
          success = false;
        }
    }

  if (!TTF_WasInit ())
    {
      if (TTF_Init () == -1)
        {
          printf ("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                  TTF_GetError ());
          success = false;
        }
    }

  return success;
}

void
LWindow::handleEvent (SDL_Event &e)
{
  if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowId)
    {
      bool updateCaption = false;

      switch (e.window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:
          mShown = true;
          break;

        case SDL_WINDOWEVENT_HIDDEN:
          mShown = false;
          break;

        case SDL_WINDOWEVENT_SIZE_CHANGED:
          mWidth = e.window.data1;
          mHeight = e.window.data2;
          SDL_RenderPresent (mRenderer.get ());
          break;

        case SDL_WINDOWEVENT_EXPOSED:
          SDL_RenderPresent (mRenderer.get ());
          break;

        case SDL_WINDOWEVENT_ENTER:
          mMouseFocus = true;
          updateCaption = true;
          break;

        case SDL_WINDOWEVENT_LEAVE:
          mMouseFocus = false;
          updateCaption = true;
          break;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
          mKeyboardFocus = true;
          updateCaption = true;
          break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
          mKeyboardFocus = false;
          updateCaption = true;
          break;

        case SDL_WINDOWEVENT_MINIMIZED:
          mMinimized = true;
          break;

        case SDL_WINDOWEVENT_MAXIMIZED:
          mMinimized = false;
          break;

        case SDL_WINDOWEVENT_RESTORED:
          mMinimized = false;
          break;

        case SDL_WINDOWEVENT_CLOSE:
          SDL_HideWindow (mWindow.get ());
          break;
        }

      if (updateCaption)
        {
          std::stringstream caption;
          caption << "Cataclysm - MouseFocus:"
                  << (mMouseFocus ? "On " : "Off ")
                  << "KeyboardFocus: " << (mKeyboardFocus ? "On " : "Off ");
          SDL_SetWindowTitle (mWindow.get (), caption.str ().c_str ());
        }
    }
  else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
    {
      if (mFullScreen)
        {
          SDL_SetWindowFullscreen (mWindow.get (), 0);
          mFullScreen = false;
        }
      else
        {
          SDL_SetWindowFullscreen (mWindow.get (),
                                   SDL_WINDOW_FULLSCREEN_DESKTOP);
          mFullScreen = true;
          mMinimized = false;
        }
    }
}

void
LWindow::focus ()
{
  if (!mShown)
    {
      SDL_ShowWindow (mWindow.get ());
    }

  SDL_RaiseWindow (mWindow.get ());
}

void
LWindow::render ()
{
  if (true)
    {
      SDL_SetRenderDrawColor (mRenderer.get (), 0, 0, 0, 0xFF);
      SDL_RenderClear (mRenderer.get ());
      SDL_RenderPresent (mRenderer.get ());
    }
}

int
LWindow::getWidth ()
{
  return mWidth;
}

int
LWindow::getHeight ()
{
  return mHeight;
}

int
LWindow::getWindowId ()
{
  return mWindowId;
}

std::shared_ptr<SDL_Renderer>
LWindow::getRenderer ()
{
  return mRenderer;
}

bool
LWindow::hasMouseFocus ()
{
  return mMouseFocus;
}

bool
LWindow::hasKeyboardFocus ()
{
  return mKeyboardFocus;
}

bool
LWindow::isMinimized ()
{
  return mMinimized;
}

bool
LWindow::isShown ()
{
  return mShown;
}
