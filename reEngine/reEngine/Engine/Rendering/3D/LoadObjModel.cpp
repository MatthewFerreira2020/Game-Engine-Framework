#include "LoadObjModel.h"
#include <list>


LoadObjModel::LoadObjModel() : currentMaterial(Material()){

}


LoadObjModel::~LoadObjModel(){
	vertices.clear();
	vertices.shrink_to_fit();

	normals.clear();
	normals.shrink_to_fit();

	textureCoords.clear();
	textureCoords.shrink_to_fit();

	indices.clear();
	indices.shrink_to_fit();

	normalIndices.clear();
	normalIndices.shrink_to_fit();

	textureIndices.clear();
	textureIndices.shrink_to_fit();

	meshVertices.clear();
	meshVertices.shrink_to_fit();

	meshes.clear();
	meshes.shrink_to_fit();
}

void LoadObjModel::LoadModel(const std::string& fileName_) {
	bool first = true;
	std::ifstream in(fileName_.c_str(), std::ios::in);
	if (!in) {
		Debug::Error("Cannot open obj file " + fileName_, __FILE__, __LINE__);
		return;
	}
	else {
		std::string line;
		while (std::getline(in, line)) {
			//vertex data
			if (line.substr(0, 2) == "v ") {
				std::istringstream v(line.substr(2));
				double x, y, z;
				v >> x >> y >> z;
				vertices.push_back(glm::vec3(x, y, z));
				//set bounding box verts here
				if (first) {
					boundingBox.maxVert.x = x;
					boundingBox.maxVert.y = y;
					boundingBox.maxVert.z = z;
					boundingBox.minVert.x = x;
					boundingBox.minVert.y = y;
					boundingBox.minVert.z = z;
					first = false;
				}

				if (boundingBox.maxVert.x < x) {
					boundingBox.maxVert.x = x;
				} else if (boundingBox.maxVert.y < y) {
					boundingBox.maxVert.y = y;
				} else if (boundingBox.maxVert.z < z) {
					boundingBox.maxVert.z = z;
				}
				if (boundingBox.minVert.x > x) {
					boundingBox.minVert.x = x;
				}
				else if (boundingBox.minVert.y > y) {
					boundingBox.minVert.y = y;
				}
				else if (boundingBox.minVert.z > z) {
					boundingBox.minVert.z = z;
				}
			}

			//normal data
			if (line.substr(0, 3) == "vn ") {
				std::istringstream v(line.substr(3));
				double x, y, z;
				v >> x >> y >> z;
				normals.push_back(glm::vec3(x, y, z));
			}

			//texture coordinate data
			if (line.substr(0, 3) == "vt ") {
				std::istringstream v(line.substr(3));
				double x, y;
				v >> x >> y;
				textureCoords.push_back(glm::vec2(x, y));
			}

			//face data
			//vertex index/texture coordinate index/normal index
			if (line.substr(0, 2) == "f ") {
				std::istringstream v(line.substr(2));
				double x, y, z, x1, y1, z1, x2, y2, z2;
				char trash;
				v >> x >> trash >> y >> trash >> z >> x1 >> trash >> y1 >> trash >> z1 >> x2 >> trash >> y2 >> trash >> z2;
				indices.push_back(x -1);
				indices.push_back(x1 - 1);
				indices.push_back(x2 - 1);
				textureIndices.push_back(y - 1);
				textureIndices.push_back(y1 - 1);
				textureIndices.push_back(y2 - 1);
				normalIndices.push_back(z - 1);
				normalIndices.push_back(z1 - 1);
				normalIndices.push_back(z2 - 1);
			}

			//new Material
			else if (line.substr(0, 7) == "usemtl ") {
				if (indices.size() > 0) {
					PostProcessing();
				}
				LoadMaterial(line.substr(7));
			}
		}
		PostProcessing();
	}
	first = true;
}

void LoadObjModel::LoadModel(const std::string& fileName_, const std::string& matName_) {
	LoadMaterialLibrary(matName_);
	LoadModel(fileName_);
}

std::vector<Vertex> LoadObjModel::GetVerts() {
	return meshVertices;
}

std::vector<GLuint> LoadObjModel::GetIndices() {
	return indices;
}

std::vector<SubMesh> LoadObjModel::GetMeshes() {
	return meshes;
}

void LoadObjModel::PostProcessing() {
	for (int i = 0; i < indices.size(); i++) {
		Vertex vert;
		vert.position = vertices[indices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.texCoords = textureCoords[textureIndices[i]];
		meshVertices.push_back(vert);
	}
	SubMesh mesh; 
	mesh.vertexList = meshVertices;
	mesh.meshIndices = indices;
	mesh.material = currentMaterial;
	meshes.push_back(mesh);
	indices.clear();
	indices.shrink_to_fit();
	normalIndices.clear();
	normalIndices.shrink_to_fit();
	textureIndices.clear();
	textureIndices.shrink_to_fit();
	meshVertices.clear();
	meshVertices.shrink_to_fit();
	currentMaterial = Material();
}

void LoadObjModel::LoadMaterial(const std::string& fileName_) {
	currentMaterial = MaterialHandler::GetInstance()->GetMaterial(fileName_);
}

void LoadObjModel::LoadMaterialLibrary(const std::string& matName_) {
	MaterialLoader::LoadMaterial(matName_);
}

BoundingBox LoadObjModel::GetBoundingBox() {
	return boundingBox;
}