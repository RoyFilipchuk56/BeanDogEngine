#pragma once

#include <string>
#include <vector>
#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_utils.hpp>
#include <glm/vec3.hpp>
#include "GlobalItems.h"

struct TextureInfo
{
	std::string texName;
	float ratio;
};

struct ModelInfo
{
	std::string fileName;
	glm::vec3 transform;
	glm::vec3 rotation;
	std::vector<TextureInfo> textures;
	float scale;
};

struct LightInfo
{
	glm::vec3 transform;
	glm::vec3 direction;
	glm::vec3 param1;
	glm::vec3 atten;
	glm::vec3 diffuse;
};

struct CameraInfo
{
	glm::vec3 tranform;
};

struct SceneInfo
{
	std::vector<ModelInfo> models;
	std::vector<LightInfo> lights;
	CameraInfo camera;

	std::vector<std::string> meshsToLoad;
	std::vector<std::string> texturesToLoad;
};

class SceneManager
{
//private:
//	SceneInfo currentLevel;
public:
	SceneManager();
	//Load the xml file
	bool LoadSceneFromXML(std::string sceneName);
	//parse the scene node
	bool ParseScene(rapidxml::xml_node<>* valueIn);
	//parse the model node
	bool ParseModel(rapidxml::xml_node<>* valueIn);
	//parse the lights node
	bool ParseLights(rapidxml::xml_node<>* valueIn);
	//pase the camera node and set the initial transform of the camera
	bool ParseCamera(rapidxml::xml_node<>* valueIn);
	//parse through the textures
	bool ParseTextures(rapidxml::xml_node<>* valueIn, std::vector<TextureInfo>& model);
	//parse the x, y, and z and add them to included value
	bool ParseVec3(rapidxml::xml_node<>* valueIn, glm::vec3& valueOut);
	//parse the scale node and add values to included value
	bool ParseScale(rapidxml::xml_node<>* valueIn, float& valueOut);
	
	//Attribute Values
	//sets the value of value out to the attribute for a float
	bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut);
	//sets the value of value out to the attribute for a string
	bool SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut);

	//Node values
	//sets the value of a node to a string
	bool SetValue(rapidxml::xml_node<>* valueIn, std::string& valueOut);

	//TODO: Save a modified scene
	bool SaveScene(std::string sceneName);

	SceneInfo currentLevel;
};
