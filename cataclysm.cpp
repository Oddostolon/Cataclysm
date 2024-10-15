#include "constants.h"
#include "wrappers/LTexture.h"
#include "wrappers/LWindow.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include <string>


bool init ();
bool loadMedia ();
void close ();

LWindow gWindows[TOTAL_WINDOWS];
LTexture gSceneTexture;

bool
init ()
{
  bool success = true;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("SDL could not initialize! SDL Error: %s\n", SDL_GetError ());
      success = false;
    }
  else
    {
      if (!SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
          printf ("Warning: Linear texture filtering not enabled!");
        }

      if (!gWindows[0].init ())
        {
          printf ("Window initialization unsuccessful! SDL Error: %s\n",
                  SDL_GetError ());
          success = false;
        }
    }

  return success;
}

bool
loadMedia ()
{
  bool success = true;

  gSceneTexture.setRenderer (gWindows[0].getRenderer ());
  if (!gSceneTexture.loadFromFile ("Assets/CharacterSprites/Main.png"))
    {
      printf ("Could not load scene texture! SDL Error: %s\n",
              SDL_GetError ());
      success = false;
    }

  return success;
}

void
close ()
{
  gSceneTexture.free ();

  IMG_Quit ();
  TTF_Quit ();
  SDL_Quit ();
}

int
main (int argc, char *args[])
{
  if (!init ())
    {
      printf ("Failed to initialize!\n");
    }
  else
    {
      if (!loadMedia ())
        {
          printf ("Failed to load media!\n");
        }
      else
        {
          for (int i = 1; i < TOTAL_WINDOWS; ++i)
            {
              gWindows[i].init ();
            }

          bool quit = false;

          SDL_Event e;

          while (!quit)
            {
              while (SDL_PollEvent (&e) != 0)
                {
                  if (e.type == SDL_QUIT)
                    {
                      quit = true;
                    }

                  for (int i = 0; i < TOTAL_WINDOWS; ++i)
                    {
                      gWindows[i].handleEvent (e);
                    }

                  if (e.type == SDL_KEYDOWN)
                    {
                      switch (e.key.keysym.sym)
                        {
                        case SDLK_1:
                          gWindows[0].focus ();
                          break;
                        case SDLK_2:
                          gWindows[1].focus ();
                          break;
                        case SDLK_3:
                          gWindows[2].focus ();
                          break;
                        }
                    }
                }

              for (LWindow window : gWindows)
                {
                  window.render ();
                }

              bool allWindowsClosed = true;
              for (LWindow window : gWindows)
                {
                  if (window.isShown ())
                    {
                      allWindowsClosed = false;
                      break;
                    }
                }

              if (allWindowsClosed)
                {
                  quit = true;
                }
            }
        }
    }

  close ();

  return 0;
}