#include "Mediator.h"
#include <iostream>

Mediator::Mediator()
{

}
Mediator::~Mediator()
{
}

bool Mediator::RecieveMessage(sMessage message)
{
	/*
	if (message.command == "wall check")
	{
		//start at original position
		glm::vec2 spotCheck = glm::vec2(message.vec_fData[1].x, message.vec_fData[1].y);

		//For each spot in the distance for the length
		for (int i = 0; i < message.vec_fData[0].z; i++)
		{
			spotCheck.x += message.vec_fData[0].x;
			spotCheck.y += message.vec_fData[0].y;
			//std::cout << "Wall Check: " << spotCheck.x << " " << spotCheck.y << std::endl;

			//x and y are reversed
			if (myMap[spotCheck.y][spotCheck.x] == 'X')
			{
				return false;
			}

			for (iTank* tank : allTanks)
			{
				if (tank->GetPosition().x == spotCheck.x && tank->GetPosition().y == spotCheck.y)
				{
					return false;
				}
			}
		}
		return true;
	}

	if (message.command == "shoot check")
	{
		//start at original position
		glm::vec2 spotCheck = glm::vec2(message.vec_fData[1].x, message.vec_fData[1].y);

		//For each spot in the distance for the length
		for (int i = 0; i < message.vec_fData[0].z; i++)
		{
			spotCheck.x += message.vec_fData[0].x;
			spotCheck.y += message.vec_fData[0].y;
			//std::cout << "Wall Check: " << spotCheck.x << " " << spotCheck.y << std::endl;

			//x and y are reversed
			if (myMap[spotCheck.y][spotCheck.x] == 'X')
			{
				return false;
			}

			for (iTank* tank : allTanks)
			{
				if (tank->GetPosition().x == spotCheck.x && tank->GetPosition().y == spotCheck.y)
				{
					sMessage takeDamage;
					takeDamage.command = "take damage";
					takeDamage.vec_fData.push_back(glm::vec4(message.vec_fData[1].z, 0, 0, 0));
					tank->RecieveMessage(takeDamage);
					return false;
				}
			}
		}
		return true;
	}

	if (message.command == "tank check")
	{
		//start at original position
		glm::vec2 spotCheck = glm::vec2(message.vec_fData[1].x + message.vec_fData[0].x, message.vec_fData[1].y + message.vec_fData[0].y);

		//For each spot in the distance for the length
		//TODO: Use the max map value with direction and distance
		for (int i = 0; i < 102; i++)
		{
			spotCheck.x += message.vec_fData[0].x;
			spotCheck.y += message.vec_fData[0].y;
			//std::cout << "Wall Check: " << spotCheck.x << " " << spotCheck.y << std::endl;

			for (iTank* tank : allTanks)
			{
				if (tank->GetPosition().x == spotCheck.x && tank->GetPosition().y == spotCheck.y)
				{
					return true;
				}
			}
		}
		return false;
	}

	if (message.command == "laser check")
	{
		bool notWall = true;
		//start at original position
		glm::vec2 spotCheck = glm::vec2(message.vec_fData[1].x, message.vec_fData[1].y);

		//For each spot in the distance for the length
		for (int i = 0; i < 200; i++)
		{
			spotCheck.x += message.vec_fData[0].x;
			spotCheck.y += message.vec_fData[0].y;
			//std::cout << "Wall Check: " << spotCheck.x << " " << spotCheck.y << std::endl;

			//x and y are reversed
			if (myMap[spotCheck.y][spotCheck.x] == 'X')
			{
				return false;
			}

			for (iTank* tank : allTanks)
			{
				if (tank->GetPosition().x == spotCheck.x && tank->GetPosition().y == spotCheck.y)
				{
					sMessage takeDamage;
					takeDamage.command = "take damage";
					takeDamage.vec_fData.push_back(glm::vec4(message.vec_fData[1].z, 0, 0, 0));
					tank->RecieveMessage(takeDamage);
					return true;
				}
			}
		}
		return true;
	}
	*/
	return false;
}

bool Mediator::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}

bool Mediator::SetReciever(iMessage* reciever)
{
	return false;
}

void Mediator::setMap(std::vector<std::vector<char>> map)
{
	myMap = map;
}
