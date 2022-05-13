/**
 * Robot base class.
 *
 * @copyright Copyright (C) 2022
 * @author Marc S. Ressl
 */

#include <cstring>  /* std::memcpy */
#include <iostream>
#include <vector>
#include "Robot.h"

using namespace std;

/**
 * @brief Construct a new Robot:: Robot object
 * 
 */
Robot::Robot()
{
    displayImages = LoadImage("../../RobotImages.png");     
}

/**
 * @brief Destroy the Robot:: Robot object
 * 
 */
Robot::~Robot()
{
    UnloadImage(displayImages);
}

/**
 * @brief Initializes the robot for a game.
 *
 */
void Robot::start(GameModel*, MQTTClient *mqttClient_)
{
}

/**
 * @brief Converts a setpoint to a tile position in maze coordinates.
 *
 * @param setpoint The setpoint
 * @return Vector2 The tile position
 */
Vector2 Robot::getTilePosition(Setpoint setpoint)
{
    Vector2 tilePosition;

    tilePosition.x = (10.0F * (1.4F + setpoint.position.x));
    tilePosition.y = (10.0F * (1.8F - setpoint.position.y));

    return tilePosition;
}

/**
 * @brief Converts a tile position in maze coordinates to a setpoint.
 *
 * @param tilePosition The tile position
 * @return setpoint The setpoint (using current robot rotation)
 */
Setpoint Robot::getSetpoint(Vector2 tilePosition)
{
    Setpoint setpoint;
    setpoint.position.x = -1.4F + 0.1F * tilePosition.x;
    setpoint.position.y = 1.8F - 0.1F * tilePosition.y;
    setpoint.rotation = this->setpoint.rotation;

    return setpoint;
}

/**
 * @brief Sets the robot controller setpoint.
 *
 * @param setpoint The setpoint
 */
void Robot::setSetpoint(Setpoint setpoint)
{
    this->setpoint = setpoint;

    vector<char> payload(12);

    *((float *)&payload[0]) = setpoint.position.x;
    *((float *)&payload[4]) = setpoint.position.y;
    *((float *)&payload[8]) = setpoint.rotation;

    mqttClient->publish(robotId + "/pid/setpoint/set", payload);
}

/**
 * @brief Lifts the robot to a destination coordinate
 *
 * @param destination The destination coordinate (x: left-right, y: up-down, z: forward-back)
 */
void Robot::liftTo(Vector3 destination)
{
    vector<char> payload(12);

    *((float *)&payload[0]) = destination.x;
    *((float *)&payload[4]) = destination.y;
    *((float *)&payload[8]) = destination.z;

    mqttClient->publish("hook/" + robotId + "/cmd", payload);
}

/**
 * @brief Sets image on the display
 *
 * @param imageIndex The index of the image (see RobotImages.png)
 */
void Robot::setDisplay(int imageIndex)
{
    Rectangle selectRectangle = {16.0F * imageIndex, 0, 16, 16};
    Image selectedImage = ImageFromImage(displayImages, selectRectangle);

    const int dataSize = 16 * 16 * 3;
    vector<char> payload(dataSize);
    memcpy(payload.data(), selectedImage.data, dataSize);

    UnloadImage(selectedImage);

    mqttClient->publish(robotId + "/display/lcd/set", payload);
}

/**
 * @brief Set robot eyes.
 *
 * @param leftEye Left eye color
 * @param rightEye Right eye color.
 */
void Robot::setEyes(Color leftEye, Color rightEye)
{
    vector<char> payload(3);
    payload[0] = leftEye.r;
    payload[1] = leftEye.g;
    payload[2] = leftEye.b;
    mqttClient->publish(robotId + "/display/leftEye/set", payload);

    payload[0] = rightEye.r;
    payload[1] = rightEye.g;
    payload[2] = rightEye.b;
    mqttClient->publish(robotId + "/display/rightEye/set", payload);
}

/**
 * @brief Updates setpoint 
 * 
 * @param velocity 
 * @param deltatime 
 */
void Robot::updateSetpoint(float velocity, float deltatime)
{
    float distance = velocity * deltatime;

    Setpoint destinationPoint = getSetpoint(tilePosition);

    Vector2 vectorDiference = Vector2Subtract(destinationPoint.position, setpoint.position);

    setpoint.position = Vector2Add(setpoint.position,
        Vector2Scale(Vector2Normalize(vectorDiference), distance));

    Vector2 actualTile = getTilePosition(setpoint);
    
    if((abs(tilePosition.x - actualTile.x) < MAX_DISTANCE) && 
        (abs(tilePosition.y - actualTile.y) < MAX_DISTANCE)) //
    {
        setpoint.rotation = (float) ((rotation - 1) * 90);
    }
}
/**
 * @brief Set the Robot Game Mode object
 * 
 */
void Robot::setRobotGameMode(GameMode _gameMode)
{
    gameMode = _gameMode;
    if(_gameMode == blue)
    {
        timeBlueStarted = gameModel->getGameStateTime();
    }
}



/**
 * @brief Getters
 * 
 */
Vector2 Robot::getRobotTilePosition()
{
    return tilePosition;
}

Direction Robot::getRobotDirection()
{
    return rotation;
}

Setpoint Robot::getRobotSetpoint()
{
    return setpoint;
}
GameMode Robot::getRobotGameMode()
{
    return gameMode;
}