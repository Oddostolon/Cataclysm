#include "KeyboardInput.h"

bool HandleTextInput( SDL_Event* e, std::string* input )
{
    if( !( SDL_GetModState() & KMOD_CTRL && ( e->text.text[ 0 ] == 'c' || e->text.text[ 0 ] == 'C' || e->text.text[ 0 ] == 'v' || e->text.text[ 0 ] == 'V' )))
    {
        input->append(e->text.text);
        return true;
    }

    return false;
}

bool HandleControlEvents(SDL_Event *e, std::string* input)
{
    if( SDL_GetModState() & KMOD_CTRL )
    {
        if( e->key.keysym.sym == SDLK_c )
        {
            SDL_SetClipboardText( input->c_str() );
            return false;
        }
        else if( e->key.keysym.sym == SDLK_v )
        {
            char* tempText = SDL_GetClipboardText();
            *input += tempText; 
            SDL_free(tempText);

            return true;
        }
    }
    else if( e->key.keysym.sym == SDLK_BACKSPACE && input->length() > 0 )
    {
        input->pop_back();
        return true;
    }
    
    return false;    
}
