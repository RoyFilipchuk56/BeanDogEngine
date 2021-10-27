#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_utils.hpp>
#include "cProjectile.h"

struct sProjectile
{
	std::string name;
	std::string modelFileName;
	std::map<std::string, std::string> attributes;
};

class ProjectileLoader
{
public:
	//Load the xml file
	bool LoadProjectilesFromXML(std::string filePath, std::map<std::string, sProjectile>& listOfProjectiles);
	//parse the Projectile node
	bool ParseType(rapidxml::xml_node<>* valueIn, std::map<std::string, sProjectile>& listOfProjectiles);
	//parse Entry node
	bool ParseEntry(rapidxml::xml_node<>* valueIn, sProjectile& valueOut);
	//sets the value of value out to the attribute for a float
	bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut);
	//sets the value of value out to the node for a float
	bool SetValue(rapidxml::xml_node<>* valueIn, float& valueOut);
	//sets the value of value out to the attribute for a string
	bool SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut);
	//sets the value of value out to the node for a string
	bool SetValue(rapidxml::xml_node<>* valueIn, std::string& valueOut);
};

class ProjectileManager
{
public:
	std::map<std::string, sProjectile> projectileList;
	ProjectileLoader loader;

	ProjectileManager(std::string filePath);

	bool GetProjectile(std::string name, sProjectile& projectile);
};

struct sCannon
{
	float yawMin;
	float yawMax;
	float pitchMin;
	float pitchMax;
};

class CannonLoader
{
public:
	sCannon currentCannon;

	//Load the xml file
	bool LoadCannonFromXML(std::string filePath);

	bool ParseCannon(rapidxml::xml_node<>* valueIn, sCannon& cannon);
	//parse the Projectile node
	bool ParseYaw(rapidxml::xml_node<>* valueIn, sCannon& cannon);
	//parse Entry node
	bool ParsePitch(rapidxml::xml_node<>* valueIn, sCannon& cannon);
	//sets the value of value out to the attribute for a float
	bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut);
};