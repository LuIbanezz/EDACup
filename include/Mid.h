#ifndef MID_H
#define MID_H

#include "Robot.h"

using namespace std;


class Mid : public Robot
{
    public:
    Mid(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();
};

#endif