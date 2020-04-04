#include "CollisionDetection.h"
#include "Ray.h"


Ray CollisionDetection::ScreenToWorldRay(glm::vec2 mousePosition_, glm::vec2 screenSize_) {
	glm::vec4 rayStart_NDC(((mousePosition_.x / screenSize_.x) - 0.5f) * 2.0f, ((mousePosition_.y / screenSize_.y) - 0.5f) * 2.0f, -1.0f, 1.0f);
	glm::vec4 rayEnd_NDC(((mousePosition_.x / screenSize_.x) - 0.5f) * 2.0f, ((mousePosition_.y / screenSize_.y) - 0.5f) * 2.0f, 0.0f, 1.0f);
	glm::mat4 inverse = glm::inverse(Camera::GetInstance()->GetPerspective() * Camera::GetInstance()->GetView());

	glm::vec4 rayStart_World = inverse * rayStart_NDC;
	rayStart_World /= rayStart_World.w;

	glm::vec4 rayEnd_World = inverse * rayEnd_NDC;
	rayEnd_World /= rayEnd_World.w;

	glm::vec4 rayDir_World(rayEnd_World - rayStart_World);
	rayDir_World = glm::normalize(rayDir_World);

	return Ray(glm::vec3(rayStart_World), glm::vec3(rayDir_World));

}

bool CollisionDetection::RayOBBIntersection(Ray* ray_, BoundingBox* box_) {
	glm::mat4 modelMatrix = box_->transform;
	glm::vec3 rayOrigin = ray_->origin;
	glm::vec3 rayDirection = ray_->direction;


	glm::vec3 aabbMin = box_->minVert;
	glm::vec3 aabbMax = box_->maxVert;
	float tMin = Camera::GetInstance()->GetClippingPlanes().x;
	float tMax = Camera::GetInstance()->GetClippingPlanes().y;

	glm::vec3 obbPosition_World(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
	glm::vec3 delta = obbPosition_World - rayOrigin;

	//x axis
	glm::vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
	float e = glm::dot(xAxis, delta);
	float f = glm::dot(rayDirection, xAxis);

	if (fabs(f) > 0.001f) {
		float t1 = (e + aabbMin.x) / f;
		float t2 = (e + aabbMax.x) / f;

		if (t1 > t2) {
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		if (t2 < tMax) {
			tMax = t2;
		}

		if (t1 > tMin) {
			tMin = t1;
		}

		if (tMax < tMin) {
			return false;
		}
	}
	else {
		if (-e + aabbMin.x > 0.0f || -e + aabbMax.x < 0.0f) {
			return false;
		}
	}

	//y axis
	glm::vec3 yAxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
	float g = glm::dot(yAxis, delta);
	float h = glm::dot(rayDirection, yAxis);

	if (fabs(h) > 0.001f) {
		float t1 = (g + aabbMin.x) / h;
		float t2 = (g + aabbMax.x) / h;

		if (t1 > t2) {
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		if (t2 < tMax) {
			tMax = t2;
		}

		if (t1 > tMin) {
			tMin = t1;
		}

		if (tMax < tMin) {
			return false;
		}
	}
	else {
		if (-g + aabbMin.x > 0.0f || -g + aabbMax.x < 0.0f) {
			return false;
		}
	}

	//z axis
	glm::vec3 zAxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
	float i = glm::dot(zAxis, delta);
	float j = glm::dot(rayDirection, zAxis);

	if (fabs(j) > 0.001f) {
		float t1 = (i + aabbMin.x) / j;
		float t2 = (i + aabbMax.x) / j;

		if (t1 > t2) {
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		if (t2 < tMax) {
			tMax = t2;
		}

		if (t1 > tMin) {
			tMin = t1;
		}

		if (tMax < tMin) {
			return false;
		}
	}
	else {
		if (-i + aabbMin.x > 0.0f || -i + aabbMax.x < 0.0f) {
			return false;
		}
	}


	ray_->intersectionDistance = tMin;
	return true;

}