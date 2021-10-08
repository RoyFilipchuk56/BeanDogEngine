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
	result &= ParseModel(valueIn->first_node("Model"));

	return result;
}

bool SceneManager::ParseModel(rapidxml::xml_node<>* valueIn)
{
	if (!valueIn)
	{
		return false;
	}
	ModelInfo model;
	bool result = true;

	result &= SetValue(valueIn->first_attribute("Name"), currentLevel.fileName);

	result &= ParseTransform(valueIn->first_node("Transform"), model);
	result &= ParseRotation(valueIn->first_node("Rotation"), model);
	result &= ParseScale(valueIn->first_node("Scale"), model);

	currentLevel.models.push_back(model);

	return result;
}

bool SceneManager::ParseTransform(rapidxml::xml_node<>* valueIn, ModelInfo& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;
	result &= SetValue(valueIn->first_attribute("x"), valueOut.transform.x);
	result &= SetValue(valueIn->first_attribute("y"), valueOut.transform.y);
	result &= SetValue(valueIn->first_attribute("z"), valueOut.transform.z);

	return result;
}

bool SceneManager::ParseRotation(rapidxml::xml_node<>* valueIn, ModelInfo& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;
	result &= SetValue(valueIn->first_attribute("x"), valueOut.rotation.x);
	result &= SetValue(valueIn->first_attribute("y"), valueOut.rotation.y);
	result &= SetValue(valueIn->first_attribute("z"), valueOut.rotation.z);

	return result;
}

bool SceneManager::ParseScale(rapidxml::xml_node<>* valueIn, ModelInfo& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;
	result &= SetValue(valueIn->first_attribute("x"), valueOut.scale.x);
	result &= SetValue(valueIn->first_attribute("y"), valueOut.scale.y);
	result &= SetValue(valueIn->first_attribute("z"), valueOut.scale.z);

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