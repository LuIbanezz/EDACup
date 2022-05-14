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

using namespace std;

class Robot
{
public:
    Robot(string robotID);
    void assignMessage(vector<float>& message, string& topic);

protected:
	Vector3 coordinates;
	Vector3 speed;
	Vector3 rotation;
	Vector3 angularSpeed;
    string robotID;

};

#endif //_ROBOT_H