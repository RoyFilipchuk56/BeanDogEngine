#pragma once

// This is the message interface for the mediator and 
//	is also used for the factories, etc. 

#include <string>
#include <vector>
#include <glm/vec4.hpp>

struct sMessage
{
	sMessage() {};
	std::string command;
	std::vector< std::string > vec_sData;
	std::vector< glm::vec4 > vec_fData;
	std::vector<int> iData;
	std::vector< bool> vec_bData;
};


class iMessage
{
public:
	// Make a virtual destructor for anything with virtual
	virtual ~iMessage() {}

	// Asyncronous (don't return)
	virtual bool SetReciever(iMessage* reciever) = 0;
	virtual bool RecieveMessage(sMessage message) = 0;

	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage message, sMessage& messageReply) = 0;

};