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

#define MAX_DISTANCE 0.1f

// TODO: completar constructor de Robot
Robot::Robot(string robotID, MQTTClient2 * client)
{
    this->robotID = robotID;
    mqttClient2 = client;
}

void Robot::assignMessage(vector<float> &message, string &topic)
{
    if (topic.find("motion") != -1)
    {
        // TODO: Arreglar plsssss //pero si está bien
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

void Robot::updateRobot(Vector3 position, float deltaTime, float speed)
{
    moveRobot(position, deltaTime, speed);
}

void Robot::moveRobot(Vector3 position, float deltaTime, float speed)
{
    Vector2 directorVector;
    directorVector.x = position.x - coordinates.x;
    directorVector.y = position.y - coordinates.y;

    if(Vector2Length(directorVector) > MAX_DISTANCE)
    {
        directorVector = Vector2Add({coordinates.x, coordinates.y},
                Vector2Scale(Vector2Normalize(directorVector), deltaTime * speed));

        Setpoint setPoint = {directorVector, position.z};
        setSetpoint(setPoint);
    }
    
    
}



void Robot::setSetpoint(Setpoint setpoint) // ESTA ES LA FUNCIÓN PARA PUBLICAR EL SETPOINT
{
    vector<char> payload(12);

    *((float *)&payload[0]) = setpoint.position.x;
    *((float *)&payload[4]) = setpoint.position.y;
    *((float *)&payload[8]) = setpoint.rotation;

    mqttClient2->publish(robotID + "/pid/setpoint/set", payload);
}