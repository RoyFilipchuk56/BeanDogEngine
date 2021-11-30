#include "SceneManager.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

SceneManager::SceneManager()
{
	doc = new rapidxml::xml_document<>;
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
		result &= ParseVec3(child->first_node("Specular"), light.specular);

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
		result &= SetValue(child->first_attribute("textureUnit"), texture.textureUnit);
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

bool SceneManager::SetValue(rapidxml::xml_attribute<>* valueIn, int& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to the included float
	valueOut = std::stoi(valueIn->value());
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

bool SceneManager::SaveScene(std::string sceneName, std::vector<cMesh*> vecMeshes)
{
	//Create a new root node
	rapidxml::xml_node<>* root = doc->allocate_node(rapidxml::node_element, "Scene");
	doc->append_node(root);

	//Add the models
	rapidxml::xml_node<>* modelsNode = doc->allocate_node(rapidxml::node_element, "Models");
	root->append_node(modelsNode);
	for (cMesh* mesh : vecMeshes)
	{
		//Start the model node
		rapidxml::xml_node<>* modelNode = doc->allocate_node(rapidxml::node_element, "Model");
		modelsNode->append_node(modelNode);
		rapidxml::xml_attribute<>* nameAttribute = doc->allocate_attribute("Name", mesh->meshFriendlyName.c_str());
		modelNode->append_attribute(nameAttribute);

		//Transform node
		rapidxml::xml_node<>* transformNode = doc->allocate_node(rapidxml::node_element, "Transform");
		modelNode->append_node(transformNode);

		//Transform Attribute
		rapidxml::xml_attribute<>* tXAttribute = doc->allocate_attribute("x", floatToChar(mesh->transformXYZ.x));
		rapidxml::xml_attribute<>* tYAttribute = doc->allocate_attribute("y", floatToChar(mesh->transformXYZ.y));
		rapidxml::xml_attribute<>* tZAttribute = doc->allocate_attribute("z", floatToChar(mesh->transformXYZ.z));
		//Append the attributes
		transformNode->append_attribute(tXAttribute);
		transformNode->append_attribute(tYAttribute);
		transformNode->append_attribute(tZAttribute);

		//rotation node
		rapidxml::xml_node<>* rotationNode = doc->allocate_node(rapidxml::node_element, "Rotation");
		modelNode->append_node(rotationNode);

		//Transform Attribute
		rapidxml::xml_attribute<>* rXAttribute = doc->allocate_attribute("x", floatToChar(mesh->rotationXYZ.x));
		rapidxml::xml_attribute<>* rYAttribute = doc->allocate_attribute("y", floatToChar(mesh->rotationXYZ.y));
		rapidxml::xml_attribute<>* rZAttribute = doc->allocate_attribute("z", floatToChar(mesh->rotationXYZ.z));
		//Append the attributes
		rotationNode->append_attribute(rXAttribute);
		rotationNode->append_attribute(rYAttribute);
		rotationNode->append_attribute(rZAttribute);

		//scale node
		rapidxml::xml_node<>* scaleNode = doc->allocate_node(rapidxml::node_element, "Scale");
		modelNode->append_node(scaleNode);

		//scale attribute
		rapidxml::xml_attribute<>* scaleAttribute = doc->allocate_attribute("scale", floatToChar(mesh->scale));
		scaleNode->append_attribute(scaleAttribute);

		//Textures node
		rapidxml::xml_node<>* TexturesNode = doc->allocate_node(rapidxml::node_element, "Textures");
		modelNode->append_node(TexturesNode);

		//Textures loop
		for (int i = 0; i < mesh->MAX_TEXTURES; i++)
		{
			//Skip if its empty
			if (mesh->textureNames[i] == "")
			{
				continue;
			}
			//create a new texture node
			rapidxml::xml_node<>* TextureNode = doc->allocate_node(rapidxml::node_element, "Texture");
			TexturesNode->append_node(TextureNode);

			rapidxml::xml_attribute<>* textureNameAttr = doc->allocate_attribute("name", mesh->textureNames[i].c_str());
			rapidxml::xml_attribute<>* textureRatioAttr = doc->allocate_attribute("ratio", floatToChar(mesh->textureRatios[i]));
			rapidxml::xml_attribute<>* textureUnitAttr = doc->allocate_attribute("textureUnit", intToChar(i));

			//Append the attributes
			TextureNode->append_attribute(textureNameAttr);
			TextureNode->append_attribute(textureRatioAttr);
			TextureNode->append_attribute(textureUnitAttr);
		}
	}
	
	//Turn the doc into a string using the built in function
	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), *doc);

	//save to the prefs file
	std::ofstream fileToWrite(CONFIG_DIR + sceneName);
	fileToWrite << xml_as_string;
	//clean up
	fileToWrite.close();
	doc->clear();

	return true;
}

char* SceneManager::floatToChar(float value)
{
	char val[64];
	sprintf_s(val, "%f", value);
	return doc->allocate_string(val);
}

char* SceneManager::intToChar(int value)
{
	char val[64];
	sprintf_s(val, "%d", value);
	return doc->allocate_string(val);
}