#include "RightWing.h"

RightWing::RightWing(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
    basePosition = {-0.7f * sign, -1.5f * sign, 90.0f * sign};
}

void RightWing::updateRobot()
{

    switch (controller->referee)
    {
    case preKickOff1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);  
        }
        else
        {
            direction = runUpDestination({controller->homeTeam[4]->coordinates.x, 
                                          controller->homeTeam[4]->coordinates.y});
            Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
            readyToKick = moveRobot(newPath, MAX_SPEED);
        }
        break;

    case preKickOff2:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);  
        }
        else
        {
        }
        break;

    case kickOff:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
            if(passToRobot(5))
            {
                controller->referee = playing;
            }
        }
        break;

    case preFreeKick1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case preFreeKick2:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case freeKick1:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case freeKick2:
        if (team == 2)
        {
            moveRobot(basePosition, PAUSE_SPEED);
        }
        else
        {
        }
        break;
    case prePenaltyKick1:
        break;
    case prePenaltyKick2:
        break;
    case penaltyKick1:
        break;
    case penaltyKick2:
        break;
    case continueGame:
        break;
    case removeRobot1:
        break;
    case removeRobot2:
        break;
    case addRobot1:
        break;
    case addRobot2:
        break;
    }
    // if (!readyToKick)
    // {
    //     if (Vector3Length(controller->ball.speed) < BALL_SPEED_ZERO)
    //     {
    //         direction = runUpDestination();
    //         Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
    //         readyToKick = moveRobot(newPath, MAX_SPEED);
    //     }
    // }

    // else if (readyToKick && !kicked)
    // {
    //     direction = kickDestination();
    //     moveRobot(direction, MAX_SPEED);
    //     if (Vector2Distance({controller->ball.position.x, controller->ball.position.y},
    //                         {coordinates.x, coordinates.y}) < (BALL_RADIUS + ROBOT_KICKER_RADIUS))
    //     {
    //         kick(kickPower);
    //         kicked = true;
    //     }
    // }
    // else
    // {
    //     readyToKick = false;
    //     kicked = false;
    // }
}