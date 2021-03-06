/**
 * @file Robot.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief   Definition for Robot methods
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Robot.h"
#include <math.h>
#include <cstring>

using namespace std;

static Vector2 projection(Vector2 vector, Vector2 projector);

/**
 * @brief Construct a new Robot:: Robot object
 * 
 * @param robotID 
 * @param client 
 * @param controller 
 */
Robot::Robot(string robotID, MQTTClient2 *client, Controller *controller)
{
    this->robotID = robotID;
    mqttClient2 = client;
    this->controller = controller;
    dressRobot(robotID[7] - '0');
    team=robotID[5]-'0';
}

/**
 * @brief Initializes the robot. To be used whenever the robots need to be restarted.
 * 
 */
void Robot::startRobot()
{
    vector<char> payload(4);
    *((float *)&payload[0]) = 249.0f;
    mqttClient2->publish(robotID + "/kicker/chargeVoltage/set", payload);

    readyToKick = false;
    kicked = false;
    kickPower = MAX_KICK_POWER;
}

/**
 * @brief Assigns the message with the information of the robot to the robot.
 * 
 * @param message 
 * @param topic 
 */
void Robot::assignMessage(vector<float> &message, string &topic)
{

    if (topic.find("motion") != -1)
    {
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

/**
 * @brief Calculates the path to the position to hit the ball, moves the robot and decides when
 * to kick the ball.
 * 
 */
void Robot::updateRobot()
{
}

/**
 * @brief Moves the robot to the destination with the rotation indicated, with the speed passed. 
 * 
 * @param destination 
 * @param speed 
 * @return true if the robot arrived to the destination
 * @return false otherwise.
 */
bool Robot::moveRobot(Setpoint destination, float speed)
{
    bool arrived = false;
    Vector2 directorVector;
    directorVector.x = destination.position.x - coordinates.x;
    directorVector.y = destination.position.y - coordinates.y;

    float rotationAngle = 90.0f - Vector2Angle({0, 0}, directorVector);

    if (Vector2Length(directorVector) > (speed * DELTA_TIME))
    {
        directorVector = Vector2Add({coordinates.x, coordinates.y},
                                    Vector2Scale(Vector2Normalize(directorVector), DELTA_TIME * speed));

        Setpoint setPoint = {directorVector, rotationAngle};
        setSetpoint(setPoint);
    }
    else if (Vector2Distance({destination.position.x, destination.position.y},
                             {coordinates.x, coordinates.y}) > ARRIVED_MIN_DISTANCE)
    {
        setSetpoint(destination);
    }
    else
    {
        arrived = true;
    }

    return arrived;
}

/**
 * @brief Publishes the setPoint
 * 
 * @copyright Marc S. Ressl 
 * @param setpoint 
 */
void Robot::setSetpoint(Setpoint setpoint)
{
    vector<char> payload(12);

    *((float *)&payload[0]) = setpoint.position.x;
    *((float *)&payload[4]) = setpoint.position.y;
    *((float *)&payload[8]) = setpoint.rotation;

    mqttClient2->publish(robotID + "/pid/setpoint/set", payload);
}

/**
 * @brief Calculates the position behind the ball, in direction to the goal, to prepare for
 * hitting the ball.
 * 
 * @return Setpoint     returns the position and the rotation
 */
Setpoint Robot::runUpDestination()
{
    Vector2 goalToBall = {controller->ball.position.x - goal1.x,
                          controller->ball.position.y - goal1.y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                    Vector2Normalize(goalToBall),
                                    BALL_RADIUS + ROBOT_KICKER_RADIUS + RUN_UP_DISTANCE),
                                {controller->ball.position.x, controller->ball.position.y})),
                            90.0f - Vector2Angle(goalToBall, {0, 0})};
    return destination;
}

/**
 * @brief Calculates the position between the ball and the goal, just ahead of the ball.
 * To be used when the robot has to kick the ball
 * 
 * @return Setpoint 
 */
Setpoint Robot::kickDestination()
{
    Vector2 goalToBall = {controller->ball.position.x - goal1.x,
                          controller->ball.position.y - goal1.y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                    Vector2Normalize(goalToBall), -0.01f),
                                {controller->ball.position.x, controller->ball.position.y})),
                            90.0f - Vector2Angle(goalToBall, {0, 0})};
    return destination;
}

/**
 * @brief Kicks the ball with the power indicated and charges the capacitor again.
 * 
 * @param strength 
 */
void Robot::kick(float strength)
{
    vector<char> payload(4);

    *((float *)&payload[0]) = strength;
    mqttClient2->publish(robotID + "/kicker/kick/cmd", payload);

    *((float *)&payload[0]) = 249.0f;
    mqttClient2->publish(robotID + "/kicker/chargeVoltage/set", payload);
}

/**
 * @brief Calculates the projection of a Vector over another vector
 * 
 * @param direc Vector to be projected
 * @param projector Vector that direc will be projected over.
 * @return Vector2 
 */
static Vector2 projection(Vector2 direc, Vector2 projector)
{
    Vector2 unitaryProjector = Vector2Normalize(projector);
    Vector2 result = Vector2Scale(unitaryProjector, Vector2DotProduct(direc, unitaryProjector));
    return result;
}

/**
 * @brief Calculates the angle between two vectors. Start from v1, working clockwise
 * 
 * @param v1 
 * @param v2 
 * @note in counter clockwise direction, v1, then v2 (otherwise negative)
 * @return float 
 */
float Robot::angleBetweenVectors(Vector2 v1, Vector2 v2)
{
    float angleV1=Vector2Angle({0,0},v1);
    if (angleV1>=180)
        angleV1=angleV1-360;

    float angleV2=Vector2Angle({0,0},v2);
    if (angleV2>=180)
        angleV2=angleV1-360;

    float angle;
    if (angleV2>=angleV1)
        angle= angleV2-angleV1;
    else 
        angle= angleV1-angleV2;

    if (angle>=180)
        angle=angle-360;
    
    return angle;
}

/**
 * @brief Calculates the path to the run_up position depending if the ball is in the
 *   middle of the way. In such case, the robot will dodge the ball.
 * 
 * @param minDistance The minimun distance between the ball and the current path, such that
 * the robot does not hit the ball.
 * 
 * @return Setpoint 
 */
Setpoint Robot::getPath(float minDistance)
{
    Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
    Vector2 vToBall = Vector2Subtract(ballPosition, {coordinates.x, coordinates.y});
    Vector2 vToFinal = Vector2Subtract(direction.position, {coordinates.x, coordinates.y});
    Vector2 vBallToFinal = Vector2Subtract(vToFinal, vToBall);

    Vector2 project = projection(vToBall, vToFinal);

    Vector2 distanceDirector = Vector2Subtract(project, vToBall);

    Vector2 resultDirection;
    Setpoint result;
    float distance = Vector2Length(distanceDirector);
    float angle = angleBetweenVectors(vBallToFinal, Vector2Scale(vToBall, -1));

    if (distance < minDistance && angle > 90.0f)
    {
        if (abs(angle - 180.f) < 0.1f) //POSIBLE CASO DE DISTANCEDIRECTOR = 0
        {
            Vector2 perpendicularVector =
                Vector2Normalize({-distanceDirector.y, distanceDirector.x});

            resultDirection = Vector2Add(Vector2Scale(Vector2Normalize(perpendicularVector), minDistance), ballPosition);
        }
        else
        {
            resultDirection = Vector2Add(Vector2Scale(Vector2Normalize(distanceDirector), minDistance), ballPosition);
        }
        result = {{resultDirection.x, resultDirection.y},
                  90.0f - Vector2Angle({0, 0}, resultDirection)};
    }
    else
    {
        result = direction;
    }

    return result;
}

/**
 * @brief Selects an image from a larger image
 * @copyright Marc S. Ressl - EDA 22.08
 */
void Robot::setShirt()
{
    Rectangle selectRectangle = {16.0F * 0, 0, 16, 16};
    Image selectedShirt = ImageFromImage(shirt, selectRectangle);

    const int dataSize = 16 * 16 * 3;
    vector<char> payload(dataSize);
    memcpy(payload.data(), selectedShirt.data, dataSize);

    UnloadImage(selectedShirt);

    mqttClient2->publish(robotID + "/display/lcd/set", payload);
}

/**
 * @brief Sets the robots T-Shirt
 * 
 * @param robotNumber 
 */
void Robot::dressRobot(int robotNumber)
{
    string path = RESOURCES_PATH;
    switch (robotNumber)
    {
    case 1:
        path += "1.png";
        shirt = LoadImage(path.c_str());
        break;

    case 2:
        path += "2.png";
        shirt = LoadImage(path.c_str());
        break;

    case 3:
        path += "3.png";
        shirt = LoadImage(path.c_str());
        break;

    case 4:
        path += "4.png";
        shirt = LoadImage(path.c_str());
        break;

    case 5:
        path += "5.png";
        shirt = LoadImage(path.c_str());
        break;

    case 6:
        path += "6.png";
        shirt = LoadImage(path.c_str());
        break;
    }
    ImageFormat(&shirt, PIXELFORMAT_UNCOMPRESSED_R8G8B8);
    setShirt();
}
