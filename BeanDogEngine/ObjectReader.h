#pragma once
#include <vector>
#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_utils.hpp>

struct sCannon
{
	float yawMin;
	float yawMax;
	float pitchMin;
	float pitchMax;
	float movementSpeed;
};

struct sWater
{
	float height;
};

struct sObjects
{
	sCannon cannonInfo;
	sWater waterInfo;
};

class ObjectLoader
{
public:
	sObjects curObjectInfo;

	//Load the object info from xml
	bool LoadObejctsFromXML(std::string filePath);

	//Parse the objects
	bool ParseObjects(rapidxml::xml_node<>* valueIn);
	//Parse the cannon node
	bool ParseCannon(rapidxml::xml_node<>* valueIn, sCannon& cannon);
	//parse the Projectile node
	bool ParseYaw(rapidxml::xml_node<>* valueIn, sCannon& cannon);
	//parse Entry node
	bool ParsePitch(rapidxml::xml_node<>* valueIn, sCannon& cannon);
	//parse the speed node
	bool ParseSpeed(rapidxml::xml_node<>* valueIn, sCannon& cannon);
	//parse the water
	bool ParseWater(rapidxml::xml_node<>* valueIn, sWater& water);
	//sets the value of value out to the attribute for a float
	bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut);
};