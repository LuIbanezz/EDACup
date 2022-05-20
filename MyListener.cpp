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
    vector<float> decodedMessage = decode(payload);
    if (topic.find("robot") != -1)
    {
        
        int robotTeam = topic[5] - '0';
        
        int robotIndex = topic[7] - '1';
       
        controller->assignRobotMessage(robotTeam, robotIndex, decodedMessage, topic);
            
    }
    else
    {
        controller->updateBall(decodedMessage);
        controller->updateController();

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