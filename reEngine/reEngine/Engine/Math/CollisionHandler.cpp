#include "CollisionHandler.h"
#include "../Core/Engine.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<GameObject*> CollisionHandler::previousCollisions = std::vector<GameObject*>();

CollisionHandler::CollisionHandler()
{
}


CollisionHandler::~CollisionHandler()
{
	delete scenePartition;
	scenePartition = nullptr;
}


CollisionHandler* CollisionHandler::GetInstance() {
	if (collisionInstance.get() == nullptr) {
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::Initialize(float worldSize_) {
	previousCollisions.clear();
	previousCollisions.shrink_to_fit();

	scenePartition = new OctSpatialPartition(worldSize_);
}

void CollisionHandler::AddObject(GameObject* go_) {
	scenePartition->AddObject(go_);
}

void CollisionHandler::Update(glm::vec2 mousePosition_, int buttonType_) {
	//create a ray screen to world position function
	Ray check = CollisionDetection::ScreenToWorldRay(mousePosition_, EngineClass::GetInstance()->GetScreenSize());
	//loop through objects and check if they are colliding with ray
	GameObject* hitResult = scenePartition->GetCollision(check);

	if (hitResult) {
		hitResult->SetHit(true, buttonType_);
	}

	for (auto c : previousCollisions) {
		if (hitResult != c && c != nullptr) {
			c->SetHit(false, buttonType_);
		}
	}

	previousCollisions.clear();
	previousCollisions.push_back(hitResult);

}