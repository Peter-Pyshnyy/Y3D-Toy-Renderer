#pragma once
#include "../sceneNode.h"
#include "../../camera.h"

class CameraNode : public SceneNode 
public:
	CameraNode(const std::string& name, Camera* camera);
	virtual ~CameraNode();
	Camera* getCamera() const;
private:
	Camera* camera;
};