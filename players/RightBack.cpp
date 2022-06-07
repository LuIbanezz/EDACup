/**
 * @file RightBack.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Definition for RightBack class methods
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "RightBack.h"

/**
 * @brief Construct a new Right Back:: Right Back object. Positions it to a base position
 *
 * @param robotID
 * @param client
 * @param controller
 */

RightBack::RightBack(string robotID, MQTTClient2 *client, Controller *controller)
    : Robot(robotID, client, controller)
{
    basePosition = {-2.5f * sign, -1.0f * sign, 90.0f * sign};
    outPosition = {-3.5f * sign, -4.0f, 0};
}
/**
 * @brief Updates Left Back robot
 *
 */
void RightBack::updateRobot()
{
    if (!removed)
    {
        switch (controller->referee)
        {
        case preKickOff1:
            withBall = false;
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case preKickOff2:
            withBall = false;
            moveRobot(basePosition, PAUSE_SPEED);
            break;
        case kickOff1:
            break;

        case kickOff2:
            break;
        case preFreeKick1:
        {
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                  basePosition.position.y - ballPosition.y};

            if (Vector2Length(baseToBall) < 0.6f)
            {
                Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                            basePosition.position.y},
                                            Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
        }
        break;

        case preFreeKick2:
        {
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                  basePosition.position.y - ballPosition.y};

            if (Vector2Length(baseToBall) < 0.6f)
            {
                Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                            basePosition.position.y},
                                            Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
        }
        break;
        case freeKick1:
            break;
        case prePenaltyKick1:
            if (team == 2)
            {
                moveRobot({-4, 1.5, 90}, PAUSE_SPEED);
            }
            else
            {
                moveRobot({-4, -1.5, 90}, PAUSE_SPEED);
            }
            break;
        case prePenaltyKick2:
            if (team == 2)
            {
                moveRobot({4, 1, 90}, PAUSE_SPEED);
            }
            else
            {
                moveRobot({4, -1, 90}, PAUSE_SPEED);
            }
            break;
        case penaltyKick1:
            break;
        case penaltyKick2:
            break;
        case continueGame:
            break;
        case pauseGame:
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                  basePosition.position.y - ballPosition.y};

            if (Vector2Length(baseToBall) < 0.6f)
            {
                Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                            basePosition.position.y},
                                            Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
            break;
        }
    }
    else if (controller->removedPlayers < RightBackRemoval)
    {
        removed = false;
        returnRobot();
    }
    else if (controller->removedPlayers >= RightBackRemoval)
    {
        removed = true;
        removeRobot();
    }
}