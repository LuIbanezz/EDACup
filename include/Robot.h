/**
 * @file Robot.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Robot class declaration
 * @version 0.1
 * @date 2022-05-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _ROBOT_H
#define _ROBOT_H

#include <vector>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include "MQTTClient2.h"
#include <iostream>
#include "Controller.h"

#define DELTA_TIME 0.1f
#define MAX_SPEED 6.0f
#define PAUSE_SPEED 1.4f
#define MAX_KICK_POWER 0.6f
#define BALL_SPEED_ZERO 0.01f
#define SHOT_SPEED 1.5
#define ARRIVED_MIN_DISTANCE 0.01f
#define TEAM_SIGN (float)((team==1) ? -1 : 1)
#define RAD(x) ((x)*PI/180)

#define GOAL_LENGTH 1

#define BALL_RADIUS 0.0215f
#define ROBOT_KICKER_RADIUS 0.08f
#define ROBOT_RADIUS 0.09f
#define RUN_UP_DISTANCE 0.25f
#define FORWARD_STEP(x) (-0.3 * (-(x)-2) * (-(x)-2) + 0.5)

const Vector2 goal1{-4.5, 0};
const Vector2 goal1_1{-4.5, -0.5}; // Primer y segundo palo
const Vector2 goal1_2{-4.5, 0.5};

const Vector2 goal2{4.5, 0};
const Vector2 goal2_1{4.5, -0.5};
const Vector2 goal2_2{4.5, 0.5};

#define MAC 0

#if MAC
#define RESOURCES_PATH "../resources/"
#else
#define RESOURCES_PATH "../../resources/"
#endif

using namespace std;

class Controller;

struct Setpoint
{
    Vector2 position;
    float rotation;
};

class Robot
{
public:
    Robot(string robotID, MQTTClient2 *client, Controller *controller);

    void assignMessage(vector<float> &message, string &topic);
    virtual void updateRobot();
    bool moveRobot(Setpoint position, float speed);

    void startRobot();
    void startDribble();
    void stopDribble();

    Vector3 coordinates;
    Vector3 speed;
    Vector3 rotation;
    Vector3 angularSpeed;

protected:
    int team;
    int sign;
    string robotID;
    MQTTClient2 *mqttClient2;
    Controller *controller;
    Image shirt;

    Setpoint direction;
    Setpoint basePosition;
    
    float kickPower;

    float positioningTime;

    bool readyToKick;
    bool kicked;
    bool withBall;
    
    void dressRobot(int robotNumber);
    void setShirt();
    void kick(float strength);
    void setSetpoint(Setpoint setpoint);
    Setpoint runUpDestination(Vector2 shotTarget);
    Setpoint kickDestination(Vector2 shotTarget);
    Setpoint getPath(float minDistance);
    float angleBetweenVectors(Vector2 v1, Vector2 v2);
    bool passToRobot(int robotReceiver);
    bool receivePass();
    
};

#endif //_ROBOT_H