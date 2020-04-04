#ifndef ENGINE_H
#define ENGINE_H

#include "Window.h"
#include <memory>
#include "Timer.h"
#include "GameInterface.h"
#include "../Rendering/3D/Triangle.h"
#include "../Rendering/3D/Square.h"
#include "../Rendering/3D/Cube.h"
#include "../Graphics/ShaderHandler.h"
#include "../Camera/Camera.h"
#include "../Graphics/TextureHandler.h"
#include "../Rendering/3D/GameObject.h"
#include"../Events/EventListener.h"
#include "../Rendering/SceneGraph.h"

class EngineClass
{
private:
	Window* window;
	Timer timer;
	GameInterface* gameInterface;
	bool isRunning;
	unsigned int fps;
public:
	EngineClass(const EngineClass&) = delete;
	EngineClass(EngineClass&&) = delete;
	EngineClass& operator=(const EngineClass&) = delete;
	EngineClass& operator=(EngineClass&&) = delete;

	bool Initialize(std::string name_, int width_, int height_);
	void Run();
	bool GetIsRunning();
	static EngineClass* GetInstance();
	void SetGameInterface(GameInterface* gameInterface_);

	glm::vec2 GetScreenSize() const;

	void NotifyOfMousePressed(int x_, int y_);
	void NotifyOfMouseReleased(int x_, int y_, int buttonType_);
	void NotifyOfMouseMove(int x_, int y_);
	void NotifyOfMouseScroll(int y_);

	void ExitGame();

private:
	EngineClass();
	~EngineClass();
	void Update(const float deltaTime_);
	void Render();
	void Shutdown();

	static std::unique_ptr<EngineClass> engineInstance;
	friend std::default_delete<EngineClass>;
};

#endif // !LAMPLIGHTER_H
