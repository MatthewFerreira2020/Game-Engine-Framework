#include "MaterialHandler.h"
std::unique_ptr<MaterialHandler> MaterialHandler::materialInstance = nullptr;
std::map<std::string, Material> MaterialHandler::materials = std::map<std::string, Material>();

MaterialHandler::MaterialHandler(){

}


MaterialHandler::~MaterialHandler(){
	if (materials.size() > 0) {
		materials.clear();
	}
}

MaterialHandler* MaterialHandler::GetInstance() {
	if (materialInstance.get() == nullptr) {
		materialInstance.reset(new MaterialHandler);
	}
	return materialInstance.get();
}

void MaterialHandler::AddMaterial(const std::string& name_, Material mat_) {
	materials.insert(std::pair<std::string, Material>(name_, mat_));
}

const Material MaterialHandler::GetMaterial(const std::string& matName_) {
	if (materials.find(matName_) != materials.end()) {
		return materials[matName_];
	}
	return Material();
}