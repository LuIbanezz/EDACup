/**
 * @file Robot.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
 * @version 0.1
 * @date 2022-05-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Robot.h"

using namespace std;

//TODO: completar constructor de Robot
Robot::Robot(string robotID)
{
    this->robotID = robotID;
}

void Robot::assignMessage(vector<float> &message, string& topic)
{
    if (topic.find("motion") != -1)
    {
        //TODO: Arreglar plsssss
        coordinates.x = message[0];
        coordinates.y = message[1];
        coordinates.z = message[2];
        speed.x = message[3];
        speed.y = message[4];
        speed.z = message[5];
        rotation.x = message[6];
        rotation.y = message[7];
        rotation.z = message[8];
        angularSpeed.x = message[9];
        angularSpeed.y = message[10];
        angularSpeed.z = message[11];
    } 
}