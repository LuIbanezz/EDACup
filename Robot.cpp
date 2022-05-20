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
#include <math.h>
#include <cstring>

using namespace std;

static Vector2 projection (Vector2 vector, Vector2 projector);


Robot::Robot(string robotID, MQTTClient2 *client, Controller *controller)
{
    this->robotID = robotID;
    mqttClient2 = client;
    this->controller = controller;
    dressRobot(robotID[7] - '0');

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
    if(!readyToKick)
    {
        if(Vector3Length(controller->ball.speed) < BALL_SPEED_ZERO)
        {
            direction = runUpDestination();
            Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
            readyToKick = moveRobot(newPath, MAX_SPEED);
        }
    }
    
    else if(readyToKick && !kicked)
    {
        direction = kickDestination();
        moveRobot(direction, MAX_SPEED);
        if(Vector2Distance({controller->ball.position.x, controller->ball.position.y}, 
            {coordinates.x, coordinates.y}) < (BALL_RADIUS + ROBOT_KICKER_RADIUS))
        {
            kick(kickPower);
            kicked = true;
        }
        
    }
    else
    {
        readyToKick = false;
        kicked = false;
    }
    
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

    float rotationAngle = 90.0f - Vector2Angle({0,0}, directorVector);

    if (Vector2Length(directorVector) > (speed * DELTA_TIME))
    {
        directorVector = Vector2Add({coordinates.x, coordinates.y},
                        Vector2Scale(Vector2Normalize(directorVector), DELTA_TIME * speed));

        Setpoint setPoint = {directorVector, rotationAngle};
        setSetpoint(setPoint);
    }
    else if(Vector2Distance({destination.position.x, destination.position.y},
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
    Vector2 goalToBall = {controller->ball.position.x - GOAL1X,
                        controller->ball.position.y - GOAL1Y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                    Vector2Normalize(goalToBall),
                                    BALL_RADIUS + ROBOT_KICKER_RADIUS + RUN_UP_DISTANCE),
                                {controller->ball.position.x, controller->ball.position.y})),
                            90.0f - Vector2Angle(goalToBall,{0,0})};
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
    Vector2 goalToBall = {controller->ball.position.x - GOAL1X,
                            controller->ball.position.y - GOAL1Y};
    Setpoint destination = {(Vector2Add(
                                Vector2Scale(
                                Vector2Normalize(goalToBall), -0.01f),
                                {controller->ball.position.x, controller->ball.position.y})),
                            90.0f - Vector2Angle(goalToBall,{0,0})};
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
static Vector2 projection (Vector2 direc, Vector2 projector)
{
    Vector2 unitaryProjector = Vector2Normalize(projector);
    Vector2 result = Vector2Scale(unitaryProjector, Vector2DotProduct(direc, unitaryProjector));
    return result;
}

/**
 * @brief Calculates the angle between two vectors
 * 
 * @param v1 
 * @param v2 
 * @return float 
 */
static float angleBetweenVectors(Vector2 v1, Vector2 v2)
{
    float angle;
    angle = acos(Vector2DotProduct(v1,v2) / (Vector2Length(v1) * Vector2Length(v2))) * (180 / PI);
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
Setpoint Robot::getPath (float minDistance)
{
    Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
    Vector2 vToBall = Vector2Subtract(ballPosition, {coordinates.x, coordinates.y});
    Vector2 vToFinal = Vector2Subtract(direction.position, {coordinates.x, coordinates.y});
    Vector2 vBallToFinal = Vector2Subtract(vToFinal, vToBall);

    Vector2 project = projection(vToBall,vToFinal);

    Vector2 distanceDirector = Vector2Subtract(project, vToBall);
    
    Vector2 resultDirection;
    Setpoint result;
    float distance = Vector2Length(distanceDirector);
    float angle = angleBetweenVectors(vBallToFinal, Vector2Scale(vToBall, -1) );

    if(distance < minDistance && angle > 90.0f)
    {
        if( abs(angle - 180.f) < 0.1f) //POSIBLE CASO DE DISTANCEDIRECTOR = 0
        {
            Vector2 perpendicularVector = 
                            Vector2Normalize({-distanceDirector.y, distanceDirector.x});

            resultDirection =  Vector2Add(Vector2Scale
                        (Vector2Normalize(perpendicularVector), minDistance), ballPosition);
        }
        else
        {
            resultDirection = Vector2Add(Vector2Scale
                        (Vector2Normalize(distanceDirector), minDistance), ballPosition);
  
        }
        result = {{resultDirection.x, resultDirection.y},
                  90.0f - Vector2Angle({0,0}, resultDirection)};
        
    }
    else
    {
        result = direction;
    }
    
    return result;
}


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
    switch (robotNumber)
    {
        case 1:
            shirt = LoadImage("../../resources/1.png");
            break;
        
        case 2:
            shirt = LoadImage("../../resources/2.png");
            break;

        case 3:
            shirt = LoadImage("../../resources/3.png");
            break;
        
        case 4:
            shirt = LoadImage("../../resources/4.png");
            break;

        case 5:
            shirt = LoadImage("../../resources/5.png");
            break;

        case 6:
            shirt = LoadImage("../../resources/6.png");
            break;
    }
    ImageFormat(&shirt, PIXELFORMAT_UNCOMPRESSED_R8G8B8);
    setShirt();
}
// string topic = "robot"; topic += TEAM; topic += "."; topic += i + '0';

// for (char c = '1'; c < ('1' + 4); c++)
//         {
//             mqttClient2.subscribe(topic + "/motor" + c + "/current");
//             mqttClient2.subscribe(topic + "/motor" + c + "/rpm");
//             mqttClient2.subscribe(topic + "/motor" + c + "/temperature");
//         }