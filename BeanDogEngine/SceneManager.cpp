#include "SceneManager.h"
#include <algorithm>
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
	result &= ParseLights(valueIn->first_node("Lights"));
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
		result &= ParseTextures(child->first_node("Textures"), model.textures);

		currentLevel.models.push_back(model);
	}

	//copy list of names
	for (ModelInfo model : currentLevel.models)
	{
		currentLevel.meshsToLoad.push_back(model.fileName);
		for (TextureInfo texture : model.textures)
		{
			currentLevel.texturesToLoad.push_back(texture.texName);
		}
	}

	//erase all duplicates
	//We only do this once so its not terrible but should be replaced if it becomes too slow
	currentLevel.meshsToLoad.erase(std::unique(currentLevel.meshsToLoad.begin(), currentLevel.meshsToLoad.end()), currentLevel.meshsToLoad.end());
	currentLevel.texturesToLoad.erase(std::unique(currentLevel.texturesToLoad.begin(), currentLevel.texturesToLoad.end()), currentLevel.texturesToLoad.end());

	return result;
}

bool SceneManager::ParseLights(rapidxml::xml_node<>* valueIn)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;

	//start at first node of Model and go until theres no more
	for (rapidxml::xml_node<>* child = valueIn->first_node("Light"); child; child = child->next_sibling())
	{
		//set all the needed variables
		LightInfo light;
		result &= ParseVec3(child->first_node("Transform"), light.transform);
		result &= ParseVec3(child->first_node("Direction"), light.direction);
		result &= ParseVec3(child->first_node("Param1"), light.param1);
		result &= ParseVec3(child->first_node("Atten"), light.atten);
		result &= ParseVec3(child->first_node("Diffuse"), light.diffuse);

		currentLevel.lights.push_back(light);
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

bool SceneManager::ParseTextures(rapidxml::xml_node<>* valueIn, std::vector<TextureInfo>& model)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;
	for (rapidxml::xml_node<>* child = valueIn->first_node("Texture"); child; child = child->next_sibling())
	{
		TextureInfo texture;
		result &= SetValue(child->first_attribute("name"), texture.texName);
		result &= SetValue(child->first_attribute("ratio"), texture.ratio);
		model.push_back(texture);
	}

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

bool SceneManager::SetValue(rapidxml::xml_node<>* valueIn, std::string& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	//convert the node to the included string
	valueOut = valueIn->value();
	return false;
}

bool SceneManager::SaveScene(std::string sceneName)
{
	return true;
}