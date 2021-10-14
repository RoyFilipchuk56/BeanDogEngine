#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager()
{

}

bool SceneManager::LoadSceneFromXML(std::string sceneName)
{
	using namespace rapidxml;
	//get the directory and find the xml to parse
	std::string dir = SCENE_DIR + sceneName;
	file<>* xmlFile = new file<>(dir.c_str());
	xml_document<>* doc = new xml_document<>;
	doc->parse<0>(xmlFile->data());

	//start parsing the scene
	bool result = ParseScene(doc->first_node("Scene"));

	delete xmlFile;
	delete doc;

	return result;
}

bool SceneManager::ParseScene(rapidxml::xml_node<>* valueIn)
{
	if (!valueIn)
	{
		return false;
	}

	//parse the model and then the camera
	bool result = true;
	result &= ParseModel(valueIn->first_node("Models"));
	result &= ParseCamera(valueIn->first_node("Camera"));

	return result;
}

bool SceneManager::ParseModel(rapidxml::xml_node<>* valueIn)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;

	//start at first node of Model and go until theres no more
	for (rapidxml::xml_node<>* child = valueIn->first_node("Model"); child; child = child->next_sibling())
	{
		//set all the needed variables
		ModelInfo model;
		result &= SetValue(child->first_attribute("Name"), model.fileName);
		result &= ParseVec3(child->first_node("Transform"), model.transform);
		result &= ParseVec3(child->first_node("Rotation"), model.rotation);
		result &= ParseScale(child->first_node("Scale"), model.scale);

		currentLevel.models.push_back(model);
	}

	return result;
}

bool SceneManager::ParseCamera(rapidxml::xml_node<>* valueIn)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;

	//get the inital camera position
	result &= ParseVec3(valueIn->first_node("Transform"), currentLevel.camera.tranform);
	return result;
}

bool SceneManager::ParseVec3(rapidxml::xml_node<>* valueIn, glm::vec3& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//get the x,y, and z values for a Vec3
	bool result = true;
	result &= SetValue(valueIn->first_attribute("x"), valueOut.x);
	result &= SetValue(valueIn->first_attribute("y"), valueOut.y);
	result &= SetValue(valueIn->first_attribute("z"), valueOut.z);

	return result;
}

bool SceneManager::ParseScale(rapidxml::xml_node<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	
	//set the scale attribute
	bool result = true;
	result &= SetValue(valueIn->first_attribute("scale"), valueOut);

	return result;
}

bool SceneManager::SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to the included float
	valueOut = std::stof(valueIn->value());
	return true;
}

bool SceneManager::SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to the included string
	valueOut = valueIn->value();
	return true;
}

bool SceneManager::SaveScene(std::string sceneName)
{
	return true;
}