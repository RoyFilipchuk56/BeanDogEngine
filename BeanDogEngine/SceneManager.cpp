#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager()
{

}

bool SceneManager::LoadSceneFromXML(std::string sceneName)
{
	using namespace rapidxml;
	std::string dir = SCENE_DIR + sceneName;
	std::cout << dir << std::endl;
	file<>* xmlFile = new file<>(dir.c_str());
	xml_document<>* doc = new xml_document<>;
	doc->parse<0>(xmlFile->data());

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
		ModelInfo model;
		result &= SetValue(child->first_attribute("Name"), model.fileName);
		result &= ParseTransform(child->first_node("Transform"), model.transform);
		result &= ParseRotation(child->first_node("Rotation"), model.rotation);
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

	result &= ParseTransform(valueIn->first_node("Transform"), currentLevel.camera.tranform);
	std::cout << currentLevel.camera.tranform.x << std::endl;
	return result;
}

bool SceneManager::ParseTransform(rapidxml::xml_node<>* valueIn, glm::vec3& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;
	result &= SetValue(valueIn->first_attribute("x"), valueOut.x);
	result &= SetValue(valueIn->first_attribute("y"), valueOut.y);
	result &= SetValue(valueIn->first_attribute("z"), valueOut.z);

	return result;
}

bool SceneManager::ParseRotation(rapidxml::xml_node<>* valueIn, glm::vec3& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

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
	valueOut = std::stof(valueIn->value());
	return true;
}

bool SceneManager::SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	valueOut = valueIn->value();
	return true;
}

bool SceneManager::SaveScene(std::string sceneName)
{
	return true;
}