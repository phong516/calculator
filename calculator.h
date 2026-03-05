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
    QLineEdit *display;
    template<class PointerToMemberFunction>
    Button* createButton(const QString& text, const PointerToMemberFunction &member);

private slots:
    void digitClicked();
};
#endif // CALCULATOR_H
