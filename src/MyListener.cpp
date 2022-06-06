/**
 * @file MyListener.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief MQTTListener sub-class definition
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>

#include "MyListener.h"

using namespace std;

MyListener::MyListener(Controller* controller)       
{
    this->controller = controller;
}

/**
 * @brief 
 * 
 * @param topic 
 * @param payload 
 */
void MyListener::onMessage(string topic, vector<char> payload)
{
    vector<float> decodedMessage;
    if (topic.find("robot") != -1)
    {
        decodedMessage = decode(payload);

        int robotTeam = topic[5] - '0';
        
        int robotIndex = topic[7] - '1';
       
        controller->assignRobotMessage(robotTeam, robotIndex, decodedMessage, topic); 
    }
    else if (topic.find("ball") != -1)
    {
        decodedMessage = decode(payload);
        controller->updateBall(decodedMessage);
        controller->updateController();
    }
    else if (topic.find("edacup") != -1)
    {
        uint8_t contenido;
        if(payload.size() > 0)
            contenido = decodeByte(payload);
        string endOfTopic;
        endOfTopic = topic.substr(6, topic.size() - 6);
        if(endOfTopic == "/preKickOff")
        {
            controller->referee = (GameStates)(preKickOff1 + contenido - 1);
        }
        else if(endOfTopic == "/kickOff")
        {
            controller->referee = (GameStates)(kickOff1 + contenido - 1);
        }
        else if(endOfTopic == "/preFreeKick")
        {
            controller->referee = (GameStates)(preFreeKick1 + contenido - 1);
        }
        else if(endOfTopic == "/freeKick")
        {
            controller->referee = (GameStates)(freeKick1 + contenido - 1);
        }
        else if(endOfTopic == "/prePenaltyKick")
        {
            controller->referee = (GameStates)(prePenaltyKick1 + contenido - 1);
        }
        else if(endOfTopic == "/penaltyKick")
        {
            controller->referee = (GameStates)(penaltyKick1 + contenido - 1);
        }
        else if(endOfTopic == "/pause")
        {
            controller->referee = (GameStates)(pauseGame);
        }
        else if(endOfTopic == "/continue")
        {
            controller->referee = (GameStates)(continueGame);
        }
        else if(endOfTopic == "/removeRobot")
        {
            controller->removedPlayers ++;
            switch (controller->removedPlayers)
            {
                case 1:
                    controller->homeTeam[1]->removed = true;
                    break;
                case 2:
                    controller->homeTeam[2]->removed = true;
                    break;
                case 3:
                    controller->homeTeam[3]->removed = true;
                    break;
                case 4:
                    controller->homeTeam[0]->removed = true;
                    break;
                case 5:
                    controller->homeTeam[4]->removed = true;
                    break;
                case 6:
                    controller->homeTeam[5]->removed = true;
                    break;
            }
        }
        else if(endOfTopic == "/addRobot")
        {
            controller->removedPlayers --;
            //controller->referee = (GameStates)(addRobot1 + contenido - 1);
        }
    }
}

/**
 * @brief Decodes a message from a char vector to a float vector
 * 
 * @param vecChar 
 * @return vector<float> 
 */
vector<float> MyListener::decode(vector<char> vecChar)
{
    vector<float> vecFloat;
    float flaux;
    char* ptrAuxChar = (char*)&flaux;

    for (int i = 0; i < (vecChar.size() / 4); i++)
    {
        for (int j = 0; j < 4; j++)
            //cambio btb el float auxiliar
            ptrAuxChar[j] = vecChar[(i * 4) + j]; 

        vecFloat.push_back(flaux);
    }
    return vecFloat;
}

/**
 * @brief Decodes a message from a char vector to a byte
 * 
 * @param vecChar 
 * @return uint8_t
 */
uint8_t MyListener::decodeByte(vector<char> vecChar)
{
    return (uint8_t)vecChar[0];
}