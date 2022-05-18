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

static Vector2 projection (Vector2 vector, Vector2 projector);

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
        speed.y = message[5];
        speed.z = message[4];
        rotation.x = message[6];
        rotation.y = message[8];
        rotation.z = message[7];
        angularSpeed.x = message[9];
        angularSpeed.y = message[11];
        angularSpeed.z = message[10];
    }
}

void Robot::updateRobot()
{
    if(!readyToKick)
    {
        if(Vector3Length(controller->ball.speed) < 0.1f)
        {
            direction = runUpDestination();
            Setpoint newPath = getPath(BALLRADIUS + ROBOTREALRADIUS + 0.1f);
            readyToKick = moveRobot(newPath, MAXSPEED);
        }
    }
    
    else if(readyToKick && !kicked)
    {
        direction = kickDestination();
        moveRobot(direction, MAXSPEED);
        if(Vector2Distance({controller->ball.position.x, controller->ball.position.y}, 
            {coordinates.x, coordinates.y}) < (BALLRADIUS + ROBOTRADIUS))
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

Setpoint Robot::runUpDestination()
{
    Vector2 goalToBall = {controller->ball.position.x - GOAL1X,
                        controller->ball.position.y - GOAL1Y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                Vector2Normalize(goalToBall), BALLRADIUS + ROBOTRADIUS + RUN_UP),
                                {controller->ball.position.x, controller->ball.position.y})),
                            90.0f - Vector2Angle(goalToBall,{0,0})};
    return destination;
}

Setpoint Robot::kickDestination()
{
    Vector2 goalToBall = {controller->ball.position.x - GOAL1X,
                            controller->ball.position.y - GOAL1Y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                Vector2Normalize(goalToBall), -0.01f),
                                {controller->ball.position.x, controller->ball.position.y})),
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

static Vector2 projection (Vector2 direc, Vector2 projector)
{
    Vector2 unitaryProjector = Vector2Normalize(projector);
    Vector2 result = Vector2Scale(unitaryProjector, Vector2DotProduct(direc, unitaryProjector));
    return result;
}

/**
    @brief Calcula el "vector distancia" entre v1 y v2
*/
Setpoint Robot::getPath (float minDistance)
{
    Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
    Vector2 vToBall = Vector2Subtract(ballPosition, {coordinates.x, coordinates.y});
    Vector2 vToFinal = Vector2Subtract(direction.position, {coordinates.x, coordinates.y});

    Vector2 project = projection(vToBall,vToFinal);

    Vector2 distanceDirector = Vector2Subtract(project, vToBall);
    
    Vector2 resultDirection;
    Setpoint result;
    float distance = Vector2Length(distanceDirector);
    if(distance < minDistance)
    {
        resultDirection = Vector2Add(Vector2Scale(Vector2Normalize(distanceDirector),minDistance),
                            ballPosition);

        result = {{resultDirection.x, resultDirection.y},
                  90.0f - Vector2Angle({0,0}, resultDirection)};
    }
    else
    {
        result = direction;
    }
    
    return result;
}