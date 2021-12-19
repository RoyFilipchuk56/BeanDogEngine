#include "CommandFactory.h"
#include "GameObject.h"
#include "EntitySystem.h"
#include "CmdMoveToPosition.h"
#include "CmdOrientTo.h"
#include "CmdWaypoint.h"
#include "CmdFollowObject.h"
#include "CommandGroup.h"
#include <iostream>

CommandFactory::~CommandFactory()
{
}

void CommandFactory::MakeCommandGroup(sMessage message)
{
    unsigned int stringCount = message.vec_sData.size();

    if (stringCount < 2)
        return;


    GameObject* entity = nGameObject::GetGameObjectByFriendlyName(message.vec_sData[1]);
    if (entity == nullptr)
    {
        std::cout << "Couldn't find an entity with friendlyname: " << message.vec_sData[1] << std::endl;
    }

    CommandGroup* group = new CommandGroup(message.iData[0]);
    group->SetFriendlyName(message.vec_sData[0]);
    entity->GetCommandController()->AddCommandGroup(group);

    unsigned int sDataPosition = 2;  // 2 is the position of the first command name
    unsigned int fDataPosition = 0;
    unsigned int iDataPosition = 1;
    // Loop through each command that needs to be created
    while (sDataPosition < stringCount)
    {
        sDataPosition++;

        // Move to a world position
        if (message.vec_sData[sDataPosition - 1] == "moveToLocation")
        {
            bool serial = (message.vec_sData[sDataPosition++] == "serial");
            bool moveByTime = (message.vec_sData[sDataPosition++] == "seconds");
            float value = message.vec_fData[fDataPosition].x;
            float toX = message.vec_fData[fDataPosition].y;
            float toY = message.vec_fData[fDataPosition].z;
            float toZ = message.vec_fData[fDataPosition].w;
            fDataPosition++;

            CmdMoveToPosition* cmd = new CmdMoveToPosition(moveByTime, value, glm::vec3(toX, toY, toZ), entity->GetPosition(), false);
            cmd->SetEntity(entity);
            if (serial)
                group->AddCommandSerial(cmd);
            else
                group->AddCommandParallel(cmd);
        }

        // Move to a position relative to itself
        else if (message.vec_sData[sDataPosition - 1] == "moveToDistance")
        {
            bool serial = (message.vec_sData[sDataPosition++] == "serial");
            bool moveByTime = (message.vec_sData[sDataPosition++] == "seconds");
            glm::vec3 pos = entity->GetPosition();
            float value = message.vec_fData[fDataPosition].x;
            float toX = message.vec_fData[fDataPosition].y;
            float toY = message.vec_fData[fDataPosition].z;
            float toZ = message.vec_fData[fDataPosition].w;
            fDataPosition++;

            CmdMoveToPosition* cmd = new CmdMoveToPosition(moveByTime, value, glm::vec3(toX, toY, toZ), entity->GetPosition(), true);
            cmd->SetEntity(entity);
            if (serial)
                group->AddCommandSerial(cmd);
            else
                group->AddCommandParallel(cmd);
        }

        // Rotate to a position
        else if (message.vec_sData[sDataPosition - 1] == "orientToLocation")
        {
            bool serial = (message.vec_sData[sDataPosition++] == "serial");
            bool moveByTime = (message.vec_sData[sDataPosition++] == "seconds");
            glm::vec3 pos = entity->GetPosition();
            float value = message.vec_fData[fDataPosition].x;
            float toX = message.vec_fData[fDataPosition].y;
            float toY = message.vec_fData[fDataPosition].z;
            float toZ = message.vec_fData[fDataPosition].w;
            fDataPosition++;
            bool lookAtPosition = (message.vec_sData[sDataPosition++] == "point");

            CmdOrientTo* cmd;

            // Orient towards a world position
            if (lookAtPosition)
            {
                cmd = new CmdOrientTo(moveByTime, value, glm::vec3(toX, toY, toZ), false);
            }
            // Orient towards an entity
            else
            {
                cmd = new CmdOrientTo(moveByTime, value, false);
                GameObject* lookAt = nGameObject::GetGameObjectByFriendlyName(message.vec_sData[sDataPosition++]);
                if (lookAt == NULL)
                {
                    std::cout << "Could not find entity " << message.vec_sData[sDataPosition - 1] << " for orientTo command." << std::endl;
                    delete cmd;
                    return;
                }
                cmd->SetLookAtEntity(lookAt);
            }
            cmd->SetEntity(entity);

            if (serial)
                group->AddCommandSerial(cmd);
            else
                group->AddCommandParallel(cmd);
        }

        // Rotate to a relative position
        else if (message.vec_sData[sDataPosition - 1] == "orientToRelative")
        {
            bool serial = (message.vec_sData[sDataPosition++] == "serial");
            bool moveByTime = (message.vec_sData[sDataPosition++] == "seconds");
            glm::vec3 pos = entity->GetPosition();
            float value = message.vec_fData[fDataPosition].x;
            float toX = message.vec_fData[fDataPosition].y;
            float toY = message.vec_fData[fDataPosition].z;
            float toZ = message.vec_fData[fDataPosition].w;
            fDataPosition++;

            CmdOrientTo* cmd;

            cmd = new CmdOrientTo(moveByTime, value, glm::vec3(toX, toY, toZ), true);
            cmd->SetEntity(entity);

            if (serial)
                group->AddCommandSerial(cmd);
            else
                group->AddCommandParallel(cmd);
        }
        
        //Traverse through the waypoints
        else if (message.vec_sData[sDataPosition - 1] == "followWaypoint")
        {
            bool serial = (message.vec_sData[sDataPosition++] == "serial");
            bool moveByTime = (message.vec_sData[sDataPosition++] == "seconds");
            float value = message.vec_fData[fDataPosition].x;
            float waypointCount = message.vec_fData[fDataPosition].y;
            fDataPosition++;

            std::vector<glm::vec3> waypoints;
            //Set each waypoints xyz
            for (int i = 0; i < waypointCount; i++)
            {
                glm::vec3 tempWaypoint(message.vec_fData[fDataPosition].x,
                    message.vec_fData[fDataPosition].y,
                    message.vec_fData[fDataPosition].z);
                waypoints.push_back(tempWaypoint);
                fDataPosition++;
            }

            CmdWaypoint* cmd = new CmdWaypoint(moveByTime, value, waypoints);

            cmd->SetEntity(entity);
            if (serial)
                group->AddCommandSerial(cmd);
            else
                group->AddCommandParallel(cmd);
        }

        // Follow an entity at a specified speed and distance
        else if (message.vec_sData[sDataPosition - 1] == "followObject")
        {
            bool serial = (message.vec_sData[sDataPosition++] == "serial");
            float distance = message.vec_fData[fDataPosition].x;
            glm::vec3 offset = glm::vec3(message.vec_fData[fDataPosition].y, message.vec_fData[fDataPosition].z, message.vec_fData[fDataPosition].w);
            fDataPosition++;
            float speed = message.vec_fData[fDataPosition].x;
            float timeLimit = message.vec_fData[fDataPosition].y;
            fDataPosition++;

            GameObject* follow = nGameObject::GetGameObjectByFriendlyName(message.vec_sData[sDataPosition++]);
            if (follow == NULL)
            {
                std::cout << "Could not find entity " << message.vec_sData[sDataPosition - 1] << " for orientTo command." << std::endl;
                return;
            }

            CmdFollowObject* cmd = new CmdFollowObject(offset, speed, distance, timeLimit, follow);

            cmd->SetEntity(entity);
            if (serial)
                group->AddCommandSerial(cmd);
            else
                group->AddCommandParallel(cmd);
        }
    }
}

bool CommandFactory::SetReciever(iMessage* receiver)
{
    mediator = receiver;

    return true;
}

bool CommandFactory::RecieveMessage(sMessage message)
{
    if (!message.vec_sData.empty())
    {
        if (message.command == "Make Command Group")
        {
            MakeCommandGroup(message);
        }
    }


    return false;
}

bool CommandFactory::RecieveMessage(sMessage message, sMessage& reply)
{
    return false;
}
