#include "ProjectileManager.h"

//Load the xml file
bool CannonLoader::LoadCannonFromXML(std::string filePath)
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

	result = ParseCannon(doc->first_node("Cannon"), currentCannon);

	delete xmlFile;
	delete doc;

	return result;
}

bool CannonLoader::ParseCannon(rapidxml::xml_node<>* valueIn, sCannon& cannon)
{
	if (!valueIn)
	{
		std::cout << "Failed at Cannon" << std::endl;
		return false;
	}
	bool result = true;
	result &= ParseYaw(valueIn->first_node("Yaw"), cannon);
	result &= ParsePitch(valueIn->first_node("Pitch"), cannon);
	return result;
}

//parse the Projectile node
bool CannonLoader::ParseYaw(rapidxml::xml_node<>* valueIn, sCannon& cannon)
{
	if (!valueIn)
	{
		std::cout << "Failed at Yaw" << std::endl;
		return false;
	}
	std::cout << "Printing Yaw" << std::endl;
	bool result = true;
	result &= SetValue(valueIn->first_attribute("Min"), cannon.yawMin);
	result &= SetValue(valueIn->first_attribute("Max"), cannon.yawMax);
	return result;
}
//parse Entry node
bool CannonLoader::ParsePitch(rapidxml::xml_node<>* valueIn, sCannon& cannon)
{
	if (!valueIn)
	{
		std::cout << "Failed at Pitch" << std::endl;
		return false;
	}
	std::cout << "Printing Pitch" << std::endl;
	bool result = true;
	result &= SetValue(valueIn->first_attribute("Min"), cannon.pitchMin);
	result &= SetValue(valueIn->first_attribute("Max"), cannon.pitchMax);
	return result;
}
//sets the value of value out to the attribute for a float
bool CannonLoader::SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		std::cout << "Failed at attribute node float" << std::endl;
		return false;
	}
	valueOut = std::stof(valueIn->value());
	return true;
}