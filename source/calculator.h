#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QString>

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject* parent = nullptr);

    Q_INVOKABLE double add(double a, double b);
    Q_INVOKABLE double subtract(double a, double b);
    Q_INVOKABLE double multiply(double a, double b);
    Q_INVOKABLE double divide(double a, double b);
};

#endif // CALCULATOR_H
