#include "ObjectReader.h"
#include <iostream>

bool ObjectLoader::LoadObejctsFromXML(std::string filePath)
{
	using namespace rapidxml;

	bool result = true;


	//Check to see if file exists
	std::ifstream f(filePath.c_str());
	//If it doesnt than create one with the default values
	if (!f.good())
	{
		return false;
	}
	f.close();

	// load the file
	file<>* xmlFile = new file<>(filePath.c_str());
	xml_document<>* doc = new xml_document<>;    // character type defaults to char
	doc->parse<0>(xmlFile->data());// 0 means default parse flags

	result = ParseObjects(doc->first_node("Objects"));

	delete xmlFile;
	delete doc;

	return result;
}

bool ObjectLoader::ParseObjects(rapidxml::xml_node<>* valueIn)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;

	result &= ParseCannon(valueIn->first_node("Cannon"), curObjectInfo.cannonInfo);
	result &= ParseWater(valueIn->first_node("Water"), curObjectInfo.waterInfo);
	return result;
}

bool ObjectLoader::ParseCannon(rapidxml::xml_node<>* valueIn, sCannon& cannon)
{
	if (!valueIn)
	{
		std::cout << "Failed at Cannon" << std::endl;
		return false;
	}
	bool result = true;
	result &= ParseYaw(valueIn->first_node("Yaw"), cannon);
	result &= ParsePitch(valueIn->first_node("Pitch"), cannon);
	result &= ParseSpeed(valueIn->first_node("Speed"), cannon);
	return result;
}

bool ObjectLoader::ParseYaw(rapidxml::xml_node<>* valueIn, sCannon& cannon)
{
	if (!valueIn)
	{
		std::cout << "Failed at Yaw" << std::endl;
		return false;
	}
	bool result = true;
	result &= SetValue(valueIn->first_attribute("Min"), cannon.yawMin);
	result &= SetValue(valueIn->first_attribute("Max"), cannon.yawMax);
	return result;
}

bool ObjectLoader::ParsePitch(rapidxml::xml_node<>* valueIn, sCannon& cannon)
{
	if (!valueIn)
	{
		std::cout << "Failed at Pitch" << std::endl;
		return false;
	}
	bool result = true;
	result &= SetValue(valueIn->first_attribute("Min"), cannon.pitchMin);
	result &= SetValue(valueIn->first_attribute("Max"), cannon.pitchMax);
	return result;
}

bool ObjectLoader::ParseSpeed(rapidxml::xml_node<>* valueIn, sCannon& cannon)
{
	if (!valueIn)
	{
		std::cout << "Failed at speed" << std::endl;
		return false;
	}
	bool result = true;
	result &= SetValue(valueIn->first_attribute("speed"), cannon.movementSpeed);
	return result;
}

bool ObjectLoader::ParseWater(rapidxml::xml_node<>* valueIn, sWater& water)
{
	if (!valueIn)
	{
		std::cout << "Failed at Pitch" << std::endl;
		return false;
	}
	bool result = true;
	result &= SetValue(valueIn->first_attribute("height"), water.height);
	return result;
}

bool ObjectLoader::SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		std::cout << "Failed at attribute node float" << std::endl;
		return false;
	}
	valueOut = std::stof(valueIn->value());
	return true;
}
