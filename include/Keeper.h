/**
 * @file Keeper.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Keeper class declaration
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
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