/**
 * @file Keeper.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Definition for Keeper class metods
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Keeper.h"

Keeper::Keeper(string robotID, MQTTClient2 *client, Controller *controller)
    : Robot(robotID, client, controller)
{
    basePosition = {-4.2f * sign, 0, 90.0f * sign};
    outPosition = {-4.5f * sign, -4.0f, 0};
}
/**
 * @brief Updates Keeper robot
 *
 */
void Keeper::updateRobot()
{
    positionGK();
}
/**
 * @brief Positions Keeper in a way such that it stops the goal
 *
 */
    void Keeper ::positionGK()
{
    if (controller->referee == pauseGame)
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
    else
    {
        shot = false;
        Vector2 ball2D = {controller->ball.position.x, controller->ball.position.y};

        // Define the angles between the ball and the goal
        Vector2 ballTo1st = Vector2Subtract({goal1_1.x * sign, goal1_1.y}, ball2D);
        float angleTo1st = angleBetweenVectors2(ballTo1st, {(-1.0f) * sign, 0});
        Vector2 ballTo2nd = Vector2Subtract({goal1_2.x * sign, goal1_2.y}, ball2D);
        float angleTo2nd = angleBetweenVectors2(ballTo2nd, {(-1.0f) * sign, 0});

        // Define the angle of the ball trajectory
        Vector2 ballSpeed2D = {controller->ball.speed.x, controller->ball.speed.y};
        float shotAngle = angleBetweenVectors2(ballSpeed2D, {(-1.0f) * sign, 0});

        if (sign * shotAngle <= sign * (angleTo1st - 5) &&
            ((sign * shotAngle) >= sign * (angleTo2nd + 5)) &&
            (Vector2Length(ballSpeed2D) > SHOT_SPEED))
        {
            shot = true;
        }

        Setpoint keeperSetPoint;
        if (shot)
        {
            // Define the line that follows the trajectory of the ball when shot
            float shotSlope = tan(-shotAngle * PI / 180);
            // Define the diving trajectory of the keeper (at a right angle with ball trajectory)
            Vector3 diveVector = Vector3Scale(Vector3CrossProduct(controller->ball.speed,
                                                                  {0, 0, -1}),TEAM_SIGN);

            Vector2 diveVector2D = {diveVector.x, diveVector.y};
            float diveAngle = angleBetweenVectors2(diveVector2D, {TEAM_SIGN * 1, 0}) * PI / 180;
            float diveSlope = tan(diveAngle);

            float x = (coordinates.y - diveSlope * coordinates.x - ball2D.y + shotSlope * ball2D.x)
                     / (shotSlope - diveSlope);
            float y = ball2D.y + shotSlope * (x - ball2D.x);
            keeperSetPoint = {{x, y}, sign * 90.0f};

            moveRobot(keeperSetPoint, MAX_SPEED);
        }
        else if (sign == 1 && (ball2D.x <= -3.5f) && (ball2D.x >= -4.5f) && (ball2D.y >= -1.0f) && 
                (ball2D.y <= 1.0f))
        {
            ballInGKArea();
        }
        else if (sign == -1 && (ball2D.x >= 3.5f) && (ball2D.x <= 4.5f) && (ball2D.y >= -1.0f) &&
         (ball2D.y <= 1.0f))
        {
            ballInGKArea();
        }
        else
        {

            float bisector = (angleTo2nd + angleTo1st) * 0.5f;
            bisector *= PI / 180.0f;
            float slope = tan(-bisector);
            float forwardStep = FORWARD_STEP(ball2D.x);
            if (forwardStep <= 0)
                forwardStep = 0;
            float x = sign * (-4.5f + forwardStep);
            float y = ball2D.y + slope * (x - ball2D.x);
            moveRobot({{x, y}, sign * 90.0f}, MAX_SPEED);
        }
    }
}

/**
 * @brief Makes the Keeper take the ball out of the area
 *
 */

void Keeper::ballInGKArea()
{
    moveRobot({controller->ball.position.x, controller->ball.position.y}, MAX_SPEED);
}