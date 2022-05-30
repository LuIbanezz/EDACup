#ifndef KEEPER_H
#define KEEPER_H

#include "Robot.h"

using namespace std;

class Keeper : public Robot
{
    public:
    Keeper(string robotID, MQTTClient2 *client, Controller *controller);
    void updateRobot();

    private:
    void positionGK();
    void ballInGKArea();

    bool shot;

};

#endif