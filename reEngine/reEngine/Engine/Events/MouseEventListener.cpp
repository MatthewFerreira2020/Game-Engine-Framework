#include "MouseEventListener.h"
#include "../Core/Engine.h"

EngineClass* MouseEventListener::engineInstance = nullptr;
int MouseEventListener::mouseX = 0;
int MouseEventListener::mouseY = 0;
int MouseEventListener::prevMouseX = 0;
int MouseEventListener::prevMouseY = 0;
bool MouseEventListener::firstUpdate = true;

MouseEventListener::~MouseEventListener(){
	engineInstance = nullptr;
}

void MouseEventListener::RegisterEngineObject(EngineClass* engine_) {
	engineInstance = engine_;
}

void MouseEventListener::update(SDL_Event e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		UpdateMousePosition();
		NotifyOfMousePressed(e.button.button);
	}
	else if (e.type == SDL_MOUSEBUTTONUP) {
		UpdateMousePosition();
		NotifyOfMouseReleased(e.button.button);
	}
	else if (e.type == SDL_MOUSEMOTION) {
		UpdateMousePosition();
		NotifyOfMouseMove();
	}
	else if (e.type == SDL_MOUSEWHEEL) {
		NotifyOfMouseScroll(e.wheel.y);
	}
}

void MouseEventListener::NotifyOfMouseMove() {
	if (engineInstance) {
		engineInstance->NotifyOfMouseMove(mouseX, mouseY);
	}
}

void MouseEventListener::NotifyOfMousePressed(int buttonType_) {
	if (engineInstance) {
		engineInstance->NotifyOfMousePressed(mouseX, mouseY);
	}
}

void MouseEventListener::NotifyOfMouseReleased(int buttonType_) {
	if (engineInstance) {
		engineInstance->NotifyOfMouseReleased(mouseX, mouseY, buttonType_);
	}
}

void MouseEventListener::NotifyOfMouseScroll(int y_) {
	if (engineInstance) {
		engineInstance->NotifyOfMouseScroll(y_);
	}
}

glm::vec2 MouseEventListener::GetPreviousMousePosition() {
	return glm::vec2(prevMouseX, prevMouseY);
}

glm::vec2 MouseEventListener::GetMousePosition() {
	return glm::vec2(mouseX, mouseY);
}

glm::vec2 MouseEventListener::GetMouseOffSet() {
	return glm::vec2(mouseX - prevMouseX, prevMouseY - mouseY);
}

void MouseEventListener::UpdateMousePosition() {
	int tempX, tempY;
	SDL_GetMouseState(&tempX, &tempY);
	tempY = engineInstance->GetScreenSize().y - tempY;
	if (firstUpdate) {
		prevMouseX = mouseX = tempX;
		prevMouseY = mouseY = tempY;
		firstUpdate = false;
	}
	else if (tempX != mouseX && tempY != mouseY) {
		prevMouseX = mouseX;
		prevMouseY = mouseY;
		mouseX = tempX;
		mouseY = tempY;
	}
}