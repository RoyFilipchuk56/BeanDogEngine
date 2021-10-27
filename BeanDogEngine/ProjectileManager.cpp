#include "ProjectileManager.h"

//Projectile manager loader
ProjectileManager::ProjectileManager(std::string filePath)
{
	loader.LoadProjectilesFromXML(filePath, projectileList);
}

bool ProjectileManager::GetProjectile(std::string name, sProjectile& projectile)
{
	std::map<std::string, sProjectile>::iterator it = projectileList.find(name);
	if (it == projectileList.end())
	{
		std::cout << "projectile not found" << std::endl;
		return false;
	}
	projectile = it->second;
	return true;
}


//Projectile loader class
//Load the xml file
bool ProjectileLoader::LoadProjectilesFromXML(std::string filePath, std::map<std::string, sProjectile>& listOfProjectiles)
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

	// load the file
	file<>* xmlFile = new file<>(filePath.c_str());
	xml_document<>* doc = new xml_document<>;    // character type defaults to char
	doc->parse<0>(xmlFile->data());// 0 means default parse flags

	result = ParseType(doc->first_node("Projectiles"), listOfProjectiles);

	delete xmlFile;
	delete doc;
	f.close();

	return result;
}

//parse the Projectile node
bool ProjectileLoader::ParseType(rapidxml::xml_node<>* valueIn, std::map<std::string, sProjectile>& listOfProjectiles)
{
	if (!valueIn)
	{
		std::cout << "Failed at Type" << std::endl;
		return false;
	}
	bool result = true;
	for (rapidxml::xml_node<>* child = valueIn->first_node("Type"); child; child = child->next_sibling())
	{
		sProjectile tempProjectile;
		result &= SetValue(child->first_attribute("Name"), tempProjectile.name);
		result &= SetValue(child->first_attribute("ModelFile"), tempProjectile.modelFileName);
		result &= ParseEntry(child, tempProjectile);

		listOfProjectiles[tempProjectile.name] = tempProjectile;
	}
	return result;
}

//parse Entry node
bool ProjectileLoader::ParseEntry(rapidxml::xml_node<>* valueIn, sProjectile& valueOut)
{
	if (!valueIn)
	{
		std::cout << "Failed at Entry" << std::endl;
		return false;
	}
	bool result = true;
	for (rapidxml::xml_node<>* child = valueIn->first_node("Entry"); child; child = child->next_sibling())
	{
		std::string key;
		std::string value;
		result &= SetValue(child->first_attribute("Key"), key);
		result &= SetValue(child, value);

		valueOut.attributes[key] = value;
	}
	return result;
}

//sets the value of value out to the attribute for a float
bool ProjectileLoader::SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		std::cout << "Failed at attribute sttribute int" << std::endl;
		return false;
	}
	valueOut = std::stoi(valueIn->value());
	return true;
}

//sets the value of value out to the node for a float
bool ProjectileLoader::SetValue(rapidxml::xml_node<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		std::cout << "Failed at attribute node string" << std::endl;
		return false;
	}
	valueOut = std::stoi(valueIn->value());
	return true;
}

//sets the value of value out to the attribute for a string
bool ProjectileLoader::SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut)
{
	if (!valueIn)
	{
		std::cout << "Failed at value attribute string" << std::endl;
		return false;
	}
	valueOut = valueIn->value();
	return true;
}

//sets the value of value out to the node for a string
bool ProjectileLoader::SetValue(rapidxml::xml_node<>* valueIn, std::string& valueOut)
{
	if (!valueIn)
	{
		std::cout << "Failed at attribute node string" << std::endl;
		return false;
	}
	valueOut = valueIn->value();
	return true;
}