/**
 * @file MyListener.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>

#include "MyListener.h"

#define ROBOT_INDEX topic[7] - '1'

using namespace std;

void MyListener::onMessage(string topic, vector<char> payload)
{
    vector<float> decodedMessage = decode(payload);
    if (topic.find("robot") != -1)
    {
                assign(decodedMessage, model->team[ROBOT_INDEX], topic);
    }
    cout << topic << endl;
}

void assign(vector<float>& message, Robot* robot, string topic)
{
    if (topic.find("motion") != -1)
    {
        //TODO: Arreglar plsssss
        robot->coordinates.x = message[0];
        robot->coordinates.y = message[1];
        robot->coordinates.z = message[2];
        robot->speed.x = message[3];
        robot->speed.y = message[4];
        robot->speed.z = message[5];
        robot->rotation.x = message[6];
        robot->rotation.y = message[7];
        robot->rotation.z = message[8];
        robot->angularSpeed.x = message[9];
        robot->angularSpeed.y = message[10];
        robot->angularSpeed.z = message[11];
    } 
}

//void Robot::setSetpoint(Setpoint setpoint)
//{
//    this->setpoint = setpoint;
//
//    vector<char> payload(12);
//
//    *((float*)&payload[0]) = setpoint.position.x;
//    *((float*)&payload[4]) = setpoint.position.y;
//    *((float*)&payload[8]) = setpoint.rotation;
//
//    mqttClient->publish(robotId + "/pid/setpoint/set", payload);
//}
vector<float> MyListener::decode(vector<char> vecChar)
{
    vector<float> vecFloat;
    float flaux;
    char* ptrAuxChar = (char*)&flaux;

    for (int i = 0; i < (vecChar.size() / 4); i++)
    {
        for (int j = 0; j < 4; j++)
            ptrAuxChar[j] = vecChar[(i * 4) + j]; //cambio btb el float auxiliar

        vecFloat.push_back(flaux);
    }
    return vecFloat;
}