#pragma once

#include "../pch.h"

class LWindow
{
public:
  LWindow ();

  bool init ();

  void handleEvent (SDL_Event &e);
  void focus ();
  void render ();

  int getWidth ();
  int getHeight ();
  int getWindowId ();
  std::shared_ptr<SDL_Renderer> getRenderer ();

  bool hasMouseFocus ();
  bool hasKeyboardFocus ();
  bool isMinimized ();
  bool isShown ();

private:
  bool createRenderer ();
  bool initSDLSubsystems ();

  std::shared_ptr<SDL_Window> mWindow;
  std::shared_ptr<SDL_Renderer> mRenderer;

  int mWidth;
  int mHeight;
  int mWindowId;

  bool mShown;
  bool mMouseFocus;
  bool mKeyboardFocus;
  bool mFullScreen;
  bool mMinimized;
};