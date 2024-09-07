#pragma once 

#include <SDL2/SDL.h> 
#include <string>

bool HandleTextInput( SDL_Event* e, std::string* input );

bool HandleControlEvents( SDL_Event* e, std::string* input );