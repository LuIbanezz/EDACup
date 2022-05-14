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

using namespace std;

struct Setpoint
{
    Vector2 position;
    float rotation;
};


class Robot
{
public:
    Robot(string robotID, MQTTClient2 *client);
	
    void assignMessage(vector<float>& message, string& topic);
	void updateRobot(Vector3 position, float deltaTime, float speed);// qué hace esta???????
	
protected:


	Vector3 coordinates;
	Vector3 speed;
	Vector3 rotation;
	Vector3 angularSpeed;
    string robotID;
	MQTTClient2* mqttClient2;
	
	void setSetpoint(Setpoint setpoint);
	void moveRobot(Vector3 position, float deltaTime, float speed);//esta publica los setpoints??? si
	

};

#endif //_ROBOT_H