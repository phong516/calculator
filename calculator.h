#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

#include "button.h"

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
private:
    bool waitingForOperand {true};
    double sumSoFar {0};
    double factorSoFar {0};
    QLineEdit *display;

    QString pendingAddictiveOperator {""};
    QString pendingMultiplicativeOperator {""};

    template<class PointerToMemberFunction>
    Button* createButton(const QString& text, const PointerToMemberFunction &member);

    bool calculate(double rightOperand, const QString &pendingOperator);;
    void abortOperation();
    void clear();
    void clearAll();

private slots:
    void digitClicked();
    void addictiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalClicked();

};
#endif // CALCULATOR_H
