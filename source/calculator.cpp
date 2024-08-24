#include "calculator.h"

Calculator::Calculator(QObject* parent) : QObject(parent)
{
}

double Calculator::add(double a, double b)
{
    return a + b;
}

double Calculator::subtract(double a, double b)
{
    return a - b;
}

double Calculator::multiply(double a, double b)
{
    return a * b;
}

double Calculator::divide(double a, double b)
{
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}
