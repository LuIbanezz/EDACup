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

// TODO: completar constructor de Robot
Robot::Robot(string robotID, MQTTClient2 *client, Controller *controller)
{
    this->robotID = robotID;
    mqttClient2 = client;
    this->controller = controller; 
}

void Robot::assignMessage(vector<float> &message, string &topic)
{
    if (topic.find("motion") != -1)
    {
        // TODO: Arreglar plsssss //pero si está bien
        coordinates.x = message[0];
        coordinates.y = message[2];
        coordinates.z = message[1];
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

void Robot::updateRobot(vector<float> &ballInfo)
{
    if(!readyToKick)
    {
        Setpoint destination = runUpDestination(ballInfo);
        readyToKick = moveRobot(destination, MAXSPEED);
    }
    
    else if(readyToKick && !kicked)
    {
        Setpoint destination = kickDestination(ballInfo);
        moveRobot(destination, MAXSPEED);
        if(Vector2Distance({ballInfo[0], ballInfo[2]}, {coordinates.x, coordinates.y}) < 
        (BALLRADIUS + ROBOTRADIUS))
        {
            kick(1.0f);
            kicked = true;
        }
        
    }
    else
    {
        readyToKick = false;
        kicked = false;
    }
    
}

bool Robot::moveRobot(Setpoint destination, float speed)
{
    bool arrived = false;
    Vector2 directorVector;
    directorVector.x = destination.position.x - coordinates.x;
    directorVector.y = destination.position.y - coordinates.y;

    float rotationAngle = 90.0f - Vector2Angle({0,0}, directorVector);

    if (Vector2Length(directorVector) > (speed * DELTATIME))
    {
        directorVector = Vector2Add({coordinates.x, coordinates.y},
                        Vector2Scale(Vector2Normalize(directorVector), DELTATIME * speed));

        Setpoint setPoint = {directorVector, rotationAngle};
        setSetpoint(setPoint);
    }
    else if(Vector2Distance({destination.position.x, destination.position.y},
    {coordinates.x, coordinates.y}) > 0.006f)
    {
        setSetpoint(destination);
    }
    else
    {
        arrived = true;
    }
    return arrived;
}

void Robot::setSetpoint(Setpoint setpoint) // ESTA ES LA FUNCIÓN PARA PUBLICAR EL SETPOINT
{
    vector<char> payload(12);

    *((float *)&payload[0]) = setpoint.position.x;
    *((float *)&payload[4]) = setpoint.position.y;
    *((float *)&payload[8]) = setpoint.rotation;

    mqttClient2->publish(robotID + "/pid/setpoint/set", payload);
}

Setpoint Robot::runUpDestination(vector<float> &ballInfo)
{
    Vector2 goalToBall = {ballInfo[0] - GOAL1X, ballInfo[2] - GOAL1Y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                Vector2Normalize(goalToBall), BALLRADIUS + ROBOTRADIUS + RUN_UP),
                                {ballInfo[0], ballInfo[2]})),
                            90.0f - Vector2Angle(goalToBall,{0,0})};
    return destination;
}

Setpoint Robot::kickDestination(vector<float> &ballInfo)
{
    Vector2 goalToBall = {ballInfo[0] - GOAL1X, ballInfo[2] - GOAL1Y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                Vector2Normalize(goalToBall), -0.01f),
                                {ballInfo[0], ballInfo[2]})),
                            90.0f - Vector2Angle(goalToBall,{0,0})};
    return destination;
}

void Robot::kick(float strength)
{
    vector<char> payload(4);

    *((float *)&payload[0]) = strength;
    mqttClient2->publish(robotID + "/kicker/kick/cmd", payload);

    *((float *)&payload[0]) = 249.0f;
    mqttClient2->publish(robotID + "/kicker/chargeVoltage/set", payload);
}

void Robot::startRobot()
{
    vector<char> payload(4);
    *((float *)&payload[0]) = 249.0f;
    mqttClient2->publish(robotID + "/kicker/chargeVoltage/set", payload);

    readyToKick = false;
    kicked = false;
}