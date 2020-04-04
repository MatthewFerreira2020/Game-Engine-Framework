#include "EventListener.h"
#include "../Core/Engine.h"


void EventListener::Update() {
	SDL_Event SDLEvent;
	while (SDL_PollEvent(&SDLEvent)) {

		switch(SDLEvent.type) {
		case SDL_QUIT:
			EngineClass::GetInstance()->ExitGame();
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			MouseEventListener::update(SDLEvent);
			break;
		default:
			break;
		}

	}

}
