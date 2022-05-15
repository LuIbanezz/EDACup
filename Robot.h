/**
 * @file Robot.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
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
#include "raylib.h"
#include <raymath.h>
#include "MQTTClient2.h"
#include <iostream>
#include "Controller.h"

#define DELTATIME 	0.1f
#define MAXSPEED	6.5f
#define GOAL1X 		4.5f
#define GOAL1Y		0.0f
#define BALLRADIUS	0.0215f
#define ROBOTRADIUS	0.08f

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
    Robot(string robotID, MQTTClient2 *client, Controller* controller);
	
    void assignMessage(vector<float>& message, string& topic);
	void updateRobot(vector<float>& ballInfo);// qué hace esta???????
	
protected:


	Vector3 coordinates;
	Vector3 speed;
	Vector3 rotation;
	Vector3 angularSpeed;
    string robotID;
	MQTTClient2* mqttClient2;
	Controller* controller;
	
	void kick(float strength);
	void setSetpoint(Setpoint setpoint);
	bool moveRobot(Setpoint position, float speed);//esta publica los setpoints??? si
	Setpoint setDestination(vector<float>& ballInfo);
	float positioningTime;

};

#endif //_ROBOT_H