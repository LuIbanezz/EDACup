#include "P_RightWing.h"

RightWing::RightWing(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
}

void RightWing::updateRobot()
{
    if (!readyToKick)
    {
        if (Vector3Length(controller->ball.speed) < BALL_SPEED_ZERO)
        {
            direction = runUpDestination();
            Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
            readyToKick = moveRobot(newPath, MAX_SPEED);
        }
    }

    else if (readyToKick && !kicked)
    {
        direction = kickDestination();
        moveRobot(direction, MAX_SPEED);
        if (Vector2Distance({controller->ball.position.x, controller->ball.position.y},
                            {coordinates.x, coordinates.y}) < (BALL_RADIUS + ROBOT_KICKER_RADIUS))
        {
            kick(kickPower);
            kicked = true;
        }
    }
    else
    {
        readyToKick = false;
        kicked = false;
    }
}