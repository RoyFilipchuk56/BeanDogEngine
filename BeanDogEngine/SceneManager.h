#pragma once

#include <string>
#include <vector>
#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_utils.hpp>
#include <glm/vec3.hpp>

struct ModelInfo
{
	std::string fileName;
	glm::vec3 transform;
	glm::vec3 rotation;
	float scale;
};

struct CameraInfo
{
	glm::vec3 tranform;
};

struct SceneInfo
{
	std::vector<ModelInfo> models;
	CameraInfo camera;
};

class SceneManager
{
//private:
//	SceneInfo currentLevel;
public:
	SceneManager();
	bool LoadSceneFromXML(std::string sceneName);
	bool ParseScene(rapidxml::xml_node<>* valueIn);
	bool ParseModel(rapidxml::xml_node<>* valueIn);
	bool ParseCamera(rapidxml::xml_node<>* valueIn);
	bool ParseTransform(rapidxml::xml_node<>* valueIn, glm::vec3& valueOut);
	bool ParseRotation(rapidxml::xml_node<>* valueIn, glm::vec3& valueOut);
	bool ParseScale(rapidxml::xml_node<>* valueIn, float& valueOut);
	bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut);
	bool SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut);

	bool SaveScene(std::string sceneName);

	SceneInfo currentLevel;
};
