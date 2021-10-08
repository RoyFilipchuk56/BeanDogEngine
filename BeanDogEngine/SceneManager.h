#pragma once

#include <string>
#include <vector>
#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_utils.hpp>
#include <glm/vec3.hpp>

struct ModelInfo
{
	glm::vec3 transform;
	glm::vec3 rotation;
	glm::vec3 scale;
};
struct SceneInfo
{
	std::string fileName;
	std::vector<ModelInfo> models;
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
	bool ParseTransform(rapidxml::xml_node<>* valueIn, ModelInfo& valueOut);
	bool ParseRotation(rapidxml::xml_node<>* valueIn, ModelInfo& valueOut);
	bool ParseScale(rapidxml::xml_node<>* valueIn, ModelInfo& valueOut);
	bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut);
	bool SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut);

	bool SaveScene(std::string sceneName);

	SceneInfo currentLevel;
};
