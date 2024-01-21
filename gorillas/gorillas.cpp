#include <duels/gorillas/game.h>
#include <cmath>

using namespace duels::gorillas;

class GorillaAI
{
public:
  // Parameters of PID control
  double K_p;
  double K_i;
  double K_d;
  double ang;

  // constructor
  GorillaAI(double proportionalGain, double integralGain, double derivativeGain, double angle)
  {
    K_p = proportionalGain;
    K_i = integralGain;
    K_d = derivativeGain;

    ang = angle;
  }

  Input computeFrom(const Feedback &feedback)
  {
    Input nextInput;

    // Calculate the error between the X coordinate of the opponent's gorilla and that of the thrown banana
    double horizontalError = feedback.opponent.x - feedback.banana.x;
    double verticalError = feedback.opponent.y - feedback.banana.y;

    // Static variables for integral and differential terms
    static double integralTerm = 0.0;
    static double hol_prevError = 0.0;
    static double ver_prevError = 0.0;

    // Updating the integral term
    // integralTerm += horizontalError;
    integralTerm = integralTerm + horizontalError + verticalError;
    // Calculation of Differential Terms
    double derivativeTerm = (verticalError - ver_prevError) / (horizontalError - hol_prevError);

    // Calculate inputs for PID control
    double pidControl = K_p * horizontalError + K_i * integralTerm + K_d * derivativeTerm;

    // Adjust speed using PID control. Wind effects are also taken into account.
    nextInput.vel += pidControl - feedback.wind;

    // Update previous error for differential term
    hol_prevError = horizontalError;
    ver_prevError = verticalError;

    // Temporarily fixes the throwing angle
    nextInput.angle = ang;

    return nextInput;
  }
};

int main(int argc, char **argv)
{
  Game game(argc, argv, "So ONISHI", 3); // difficulty from 0 to 3

  // PID Tuning
  GorillaAI ai(0.25, 0.001, 0.01, 60); // proportionalGain, integralGain, derivativeGain, angle
  Input input;
  Feedback feedback;

  // don't need this
  // const auto timeout = game.timeout_ms();

  while (game.get(feedback))
  {
    input = ai.computeFrom(feedback);

    game.send(input);
  }
}
