#include "wrappers/LTexture.h"
#include "wrappers/LWindow.h"

#pragma region FUNCTION_DECLARATIONS
bool init ();
bool loadMedia ();
void close ();
#pragma endregion

LWindow gWindow;
LTexture gSceneTexture;

bool
init ()
{
  bool success = true;

  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
      printf ("SDL could not initialize! SDL Error: %s\n", SDL_GetError ());
      success = false;
    }
  else
    {
      if (!gWindow.init ())
        {
          printf ("Window could not be created! SDL Error: %s\n", SDL_GetError ());
          success = false;
        }
      else
        {
          gWindow.createRenderer ();
          if (!gWindow.getRenderer ())
            {
              printf ("Renderer could not be created! SDL Error: %s\n", SDL_GetError ());
              success = false;
            }
        }
    }

  return success;
}

bool
loadMedia ()
{
  bool success = true;

  gSceneTexture.setRenderer (gWindow.getRenderer ());
  if (!gSceneTexture.loadFromFile ("Assets/CharacterSprites/Main.png"))
    {
      printf ("Could not load scene texture! SDL Error: %s\n", SDL_GetError ());
      success = false;
    }

  return success;
}

void
close ()
{
  gSceneTexture.free ();

  IMG_Quit ();
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

                  gWindow.handleEvent (e);
                }

              if (!gWindow.isMinimized ())
                {
                  SDL_SetRenderDrawColor (gWindow.getRenderer ().get (), 0xFF, 0xFF, 0xFF, 0xFF);
                  SDL_RenderClear (gWindow.getRenderer ().get ());

                  gSceneTexture.render (0, 0);

                  SDL_RenderPresent (gWindow.getRenderer ().get ());
                }
            }
        }
    }

  // Free resources and close SDL
  close ();

  return 0;
}