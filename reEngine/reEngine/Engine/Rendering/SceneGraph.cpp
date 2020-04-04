#include "SceneGraph.h"


std::unique_ptr<SceneGraph> SceneGraph::sceneGraphInstance = nullptr;
std::map<GLuint, std::vector<Model*>> SceneGraph::sceneModels = std::map<GLuint, std::vector<Model*>>();
std::map<std::string, GameObject*> SceneGraph::sceneGameObjects = std::map<std::string, GameObject*>();

SceneGraph::SceneGraph(){

}


SceneGraph::~SceneGraph(){
	if (sceneGameObjects.size() > 0) {
		for (auto go : sceneGameObjects) {
			delete go.second;
			go.second = nullptr;
		}
		sceneGameObjects.clear();
	}

	if (sceneModels.size() > 0) {
		for (auto entry : sceneModels) {
			if (entry.second.size() > 0) {
				for (auto m : entry.second) {
					delete m;
					m = nullptr;
				}
				entry.second.clear();
				entry.second.shrink_to_fit();
			}
		}
		sceneModels.clear();
	}

}


SceneGraph* SceneGraph::GetInstance() {
	if (sceneGraphInstance.get() == nullptr) {
		sceneGraphInstance.reset(new SceneGraph);
	}
	return sceneGraphInstance.get();
}

void SceneGraph::AddModel(Model* obj_) {
	if (sceneModels.find(obj_->GetShader()) == sceneModels.end()) {
		sceneModels.insert(std::pair<GLuint, std::vector<Model*>>(obj_->GetShader(), std::vector<Model*>{obj_}));
	}
	else {
		sceneModels[obj_->GetShader()].push_back(obj_);
	}
}

void SceneGraph::AddGameObject(GameObject* go_, std::string name_) {
	if (name_ == "") {
		std::string newName = "GameObject" + sceneGameObjects.size();
		go_->SetTag(newName);
		sceneGameObjects[newName] = go_;
		CollisionHandler::GetInstance()->AddObject(go_);
	}
	else if (sceneGameObjects.find(name_) == sceneGameObjects.end()) {
		go_->SetTag(name_);
		sceneGameObjects[name_] = go_;
		CollisionHandler::GetInstance()->AddObject(go_);
	}
	else {
		Debug::Error("Trying to add a gameobject with a name that already exists " + name_ , __FILE__, __LINE__);
	}

}

GameObject* SceneGraph::GetGameObject(std::string name_) {
	if (sceneGameObjects.find(name_) != sceneGameObjects.end()) {
		return sceneGameObjects[name_];
	}
	else {
		return nullptr;
	}
}

void SceneGraph::Update(const float deltaTime_) {
	for (auto go : sceneGameObjects) {
		go.second->Update(deltaTime_);
	}
}

void SceneGraph::Render(Camera* camera_) {
	for (auto entry : sceneModels) {
		glUseProgram(entry.first);
		for (auto m : entry.second) {
			m->Render(camera_);
		}
	}
}