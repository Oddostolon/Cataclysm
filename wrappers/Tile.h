#include "../pch.h"
#include <SDL2/SDL_rect.h>
class Tile
{
public:
  Tile (int x, int y, int tileType,
        std::shared_ptr<SDL_Renderer> renderer = nullptr);

  void render (std::shared_ptr<SDL_Rect> camera);

  int getType ();

  SDL_Rect getBox ();

private:
  SDL_Rect mBox;
  int mType;

  std::shared_ptr<SDL_Renderer> mRenderer;
};