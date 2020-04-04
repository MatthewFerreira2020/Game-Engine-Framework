#include "Engine.h"

// redeclare static variables
std::unique_ptr<EngineClass> EngineClass::engineInstance;

EngineClass::EngineClass() : window(nullptr), isRunning(false), 
	fps(200), gameInterface(nullptr)
{
}


EngineClass::~EngineClass()
{
}

bool EngineClass::Initialize(std::string name_, int width_, int height_) {
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	window = new Window();
	if (!window->Initialize(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize window object", "CORE/Engine.cpp", __LINE__);
		return isRunning = false;
	}
	timer.Start();

	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);
	MouseEventListener::RegisterEngineObject(this);

	ShaderHandler::GetInstance()->CreateProgram("colorShader", "Engine/Shaders/ColourVertexShader.glsl", "Engine/Shaders/ColourFragmentShader.glsl");
	ShaderHandler::GetInstance()->CreateProgram("baseShader", "Engine/Shaders/VertexShader.glsl", "Engine/Shaders/FragmentShader.glsl");

	//check to see if exists
	if ( gameInterface ) {
		if (!gameInterface->Initialize()) {
			Debug::FatalError("Failed to initialize game interface", "CORE/Engine.cpp", __LINE__);
			return isRunning = false;
		}
	}

	Debug::Info("Everything Initialized fine in engine class", "CORE/Engine.cpp", __LINE__);

	return isRunning = true;
}

void EngineClass::Run() {

	while (isRunning) {
		EventListener::Update();
		timer.UpdateFrameTicks();
		// first update then render always
		Update(timer.GetDeltaTime());
		Render();
		SDL_Delay(timer.GetSleepTime(fps));
	}
	if (!isRunning) {
		Shutdown();
	}
}

bool EngineClass::GetIsRunning() {
	return isRunning;
}

EngineClass * EngineClass::GetInstance()
{
	if (engineInstance.get() == nullptr) {
		engineInstance.reset(new EngineClass);
	}
	return engineInstance.get();
}

void EngineClass::SetGameInterface(GameInterface * gameInterface_)
{
	gameInterface = gameInterface_;
}

glm::vec2 EngineClass::GetScreenSize() const
{
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

void EngineClass::Update(const float deltaTime_) {
	//std::cout << deltaTime_ << std::endl;
	if (gameInterface) {
		gameInterface->Update(deltaTime_);
	}
}

void EngineClass::Render() {
	// clear the current window of color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// clear both these buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Call game render
	if (gameInterface) {
		gameInterface->Render();
	}

	// swap windows
	SDL_GL_SwapWindow(window->GetWindow());
}

void EngineClass::Shutdown() {
	window->Shutdown();
	delete window;
	window = nullptr;

	delete gameInterface;
	gameInterface = nullptr;

	// closes sdl
	SDL_Quit();
	// exit the program
	exit(0);
}

void EngineClass::NotifyOfMousePressed(int x_, int y_) {
	
}

void EngineClass::NotifyOfMouseReleased(int x_, int y_, int buttonType_) {
	CollisionHandler::GetInstance()->Update(glm::vec2(x_, y_), buttonType_);
}

void EngineClass::NotifyOfMouseMove(int x_, int y_) {
	Camera::GetInstance()->ProcessMouseMovement(MouseEventListener::GetMouseOffSet().x, MouseEventListener::GetMouseOffSet().y);
}

void EngineClass::NotifyOfMouseScroll(int y_) {
	Camera::GetInstance()->ProcessMouseZoom(y_);
}

void EngineClass::ExitGame() {
	isRunning = false;
}