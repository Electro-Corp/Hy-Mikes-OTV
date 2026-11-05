// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------
#ifndef DIRECTION_H
#define DIRECTION_H

static void unsetMotorDirection(motor &motor)
{
  motor.direction.input1 = LOW;
  motor.direction.input2 = LOW;
}

static void setMotorDirectionForward(motor &motor)
{
  motor.direction.input1 = HIGH;
  motor.direction.input2 = LOW;
}

static void setMotorDirectionBackward(motor &motor)
{
  motor.direction.input1 = LOW;
  motor.direction.input2 = HIGH;
}

static bool isMotorDirectionUnset(motor &motor)
{
  return motor.direction.input1 == LOW && motor.direction.input2 == LOW;
}

static bool isMotorDirectionForward(motor &motor)
{
  return motor.direction.input1 == HIGH && motor.direction.input2 == LOW;
}

static bool isMotorDirectionBackward(motor &motor)
{
  return motor.direction.input1 == LOW && motor.direction.input2 == HIGH;
}

#endif
