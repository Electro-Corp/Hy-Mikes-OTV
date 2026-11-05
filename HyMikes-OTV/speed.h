// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------
#ifndef SPEED_H
#define SPEED_H

static void setMotorSpeed(motor &motor, int speed)
{
  motor.speed = constrain(speed, 0, 255); // Force values between 0-255 (PWM)
}

static void minMotorSpeed(motor &motor)
{
  setMotorSpeed(motor, 0);
}

static void maxMotorSpeed(motor &motor)
{
  setMotorSpeed(motor, 255);
}

static byte getMotorSpeed(motor &motor)
{
  return motor.speed;
}

#endif