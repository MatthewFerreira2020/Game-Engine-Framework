#include "Game1.h"



Game1::Game1()
{
}


Game1::~Game1()
{

}

bool Game1::Initialize()
{
	Camera::GetInstance()->SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
	Camera::GetInstance()->AddLightSource(new LightSource(glm::vec3(0.0f, 0.0f, 2.0f), 0.1f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f)));
	CollisionHandler::GetInstance()->Initialize(100.0f);

	Model* apple = new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("baseShader"));
	Model* dice = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("baseShader"));

	SceneGraph::GetInstance()->AddModel(apple);
	SceneGraph::GetInstance()->AddModel(dice);

	SceneGraph::GetInstance()->AddGameObject(new GameObject(apple), "Apple1");
	SceneGraph::GetInstance()->AddGameObject(new GameObject(dice, glm::vec3(3.0f, 0.0f, 0.0f)));

	SceneGraph::GetInstance()->GetGameObject("Apple1")->SetScale(glm::vec3(0.5f));
	std::cout << "Object's bounding box:" << std::endl;
	std::cout << "minVert: " << glm::to_string(SceneGraph::GetInstance()->GetGameObject("Apple1")->GetBoundingBox().minVert) << std::endl;
	std::cout << "maxVert: " << glm::to_string(SceneGraph::GetInstance()->GetGameObject("Apple1")->GetBoundingBox().maxVert) << std::endl;

	return true;
}

void Game1::Update(float deltaTime_)
{
	//std::cout << "Game Update: " << deltaTime_ << std::endl;
	SceneGraph::GetInstance()->Update(deltaTime_);
}

void Game1::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//::cout << "Game Render: " << std::endl;
	SceneGraph::GetInstance()->Render(Camera::GetInstance());
}
