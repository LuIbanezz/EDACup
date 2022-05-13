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
	Vector3 coordinates;
	Vector3 speed;
	Vector3 rotation;
	Vector3 angularSpeed;
    string robotID;
    void assign(vector<float>& message, string topic);
private:

};

#endif//_ROBOT_H