
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glew.h>				// this must go before the one below it
#include <SDL_opengl.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Debug.h"

#define GL3_PROTOTYPES 1

// we are using programmable pipeline not fixed pipeline(will not accept fixed pipeline)

class Window
{
private:
	int width;
	int height;
	SDL_Window* window;
	SDL_GLContext context;
public:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	Window();
	~Window();
	bool Initialize(std::string name_, int width_, int height_);
	void Shutdown();
	int GetHeight();
	int GetWidth();
	SDL_Window* GetWindow() const;
private:
	void SetAttributes();
};


#endif // !WINDOW_H

