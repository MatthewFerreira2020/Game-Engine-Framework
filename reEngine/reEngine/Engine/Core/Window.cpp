#include "Window.h"


Window::Window() : window(nullptr)
{
}


Window::~Window()
{
	Shutdown();
}

bool Window::Initialize(std::string name_, int width_, int height_)
{
	// try to initialize SDL Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Debug::FatalError("Failed to initialize SDL", "CORE/Window.cpp", __LINE__);
		return false;
	}
	// set dimension variables
	width = width_;
	height = height_;
	// initialize the window
	window = SDL_CreateWindow(name_.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL);
	// ensure window was initialized
	if (!window) {
		Debug::FatalError("Window could not be initialized", "CORE/Window.cpp", __LINE__);
		return false;
	}
	// create context
	context = SDL_GL_CreateContext(window);
	SetAttributes();
	// initialize Glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		Debug::FatalError("Glew initialization failed", "CORE/Window.cpp", __LINE__);
		return false;
	}
	// tell gl that when rendering take the z axis into consideration
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);
	
	Debug::Info("OpenGL version: " + std::string((char*)glGetString(GL_VERSION)), "CORE/Window.cpp", __LINE__);

	return true;
}

void Window::Shutdown()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}

int Window::GetHeight()
{
	return height;
}

int Window::GetWidth()
{
	return width;
}

SDL_Window * Window::GetWindow() const
{
	return window;
}

void Window::SetAttributes()
{
	// get rid of any depricated functions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	// set the major version (using 3.3)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	// set the minor version (using 3.3)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// enable the doublebuffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// set the depth buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	// set swap interval to be equal sychronized with the vertical retrace
	SDL_GL_SetSwapInterval(1);
	// enable error check of experimental GPU extensions (must be done before initializing GLEW)
	glewExperimental = GL_TRUE;
}
