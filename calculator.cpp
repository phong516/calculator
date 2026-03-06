#include "calculator.h"
#include <QLineEdit>
#include <QGridLayout>
Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    display = new QLineEdit("0");
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);
    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);

    enum {NumOfDigitButtons = 10};
    Button* digitButton[NumOfDigitButtons];

    for (int i = 0; i < NumOfDigitButtons; ++i)
    {
        digitButton[i] = createButton(QString::number(i), &Calculator::digitClicked);
    }
    Button* backspaceButton = createButton("Backspace", &Calculator::digitClicked);
    Button* clearButton = createButton("Clear", &Calculator::clear);
    Button* clearAllButton = createButton("Clear All", &Calculator::clearAll);

    Button* memoryClearButton = createButton("MC", &Calculator::digitClicked);
    Button* memoryReadButton = createButton("MR", &Calculator::digitClicked);
    Button* memorySetButton = createButton("MS", &Calculator::digitClicked);
    Button* memoryAddButton = createButton("M+", &Calculator::digitClicked);

    Button* pointButton = createButton(".", &Calculator::digitClicked);
    Button* changeSignButton = createButton("\302\261", &Calculator::digitClicked);
    Button* addButton = createButton("+", &Calculator::addictiveOperatorClicked);
    Button* minusButton = createButton("-", &Calculator::addictiveOperatorClicked);
    Button* multiplyButton = createButton("x", &Calculator::multiplicativeOperatorClicked);
    Button* divideButton = createButton("/", &Calculator::multiplicativeOperatorClicked);

    Button* squareRootButton = createButton("sqrt", &Calculator::digitClicked);
    Button* squareButton = createButton("\302\262", &Calculator::digitClicked);
    Button* reciprocalButton = createButton("1/x", &Calculator::digitClicked);
    Button* equalButton = createButton("=", &Calculator::equalClicked);


    mainLayout->addWidget(display, 0, 0, 1, 6);
    mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
    mainLayout->addWidget(clearButton, 1, 2, 1, 2);
    mainLayout->addWidget(clearAllButton, 1, 4, 1, 2);
    mainLayout->addWidget(memoryClearButton, 2, 0, 1, 1);
    mainLayout->addWidget(memoryReadButton, 3, 0, 1, 1);
    mainLayout->addWidget(memorySetButton, 4, 0, 1, 1);
    mainLayout->addWidget(memoryAddButton, 5, 0, 1, 1);

    mainLayout->addWidget(digitButton[0], 5, 1, 1, 1);
    mainLayout->addWidget(pointButton, 5, 2, 1, 1);
    mainLayout->addWidget(changeSignButton, 5, 3, 1, 1);

    mainLayout->addWidget(divideButton, 2, 4, 1, 1);
    mainLayout->addWidget(multiplyButton, 3, 4, 1, 1);
    mainLayout->addWidget(minusButton, 4, 4, 1, 1);
    mainLayout->addWidget(addButton, 5, 4, 1, 1);

    mainLayout->addWidget(squareRootButton, 2, 5, 1, 1);
    mainLayout->addWidget(squareButton, 3, 5, 1, 1);
    mainLayout->addWidget(reciprocalButton, 4, 5, 1, 1);
    mainLayout->addWidget(equalButton, 5, 5, 1, 1);

    for (int i = 1; i < NumOfDigitButtons; ++i)
    {
        int row = ((9 - i) / 3) + 2;
        int col = ((i - 1) % 3) + 1;
        mainLayout->addWidget(digitButton[i], row, col, 1, 1);
    }

    setLayout(mainLayout);

}

Calculator::~Calculator() {}

bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == "+")
        sumSoFar += rightOperand;
    else if (pendingOperator == "-")
        sumSoFar -= rightOperand;
    else if (pendingOperator == "x")
        factorSoFar *= rightOperand;
    else if (pendingOperator == "/")
    {
        if (rightOperand == 0)
            return false;
        factorSoFar /= rightOperand;
    }
    return true;
}

void Calculator::abortOperation()
{
    clearAll();
    display->setText("XXX");
}

void Calculator::clear()
{
    // no clear after click a operator
    // only clear just-input operand
    if (waitingForOperand)
        return;
    display->setText("0");
    waitingForOperand = true;
}

void Calculator::clearAll()
{
    sumSoFar = 0;
    factorSoFar = 0;
    pendingAddictiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    waitingForOperand = true;
    display->setText("0");
}

template<class PointerToMemberFunction>
Button *Calculator::createButton(const QString &text, const PointerToMemberFunction &member)
{
    Button* button = new Button(text);
    connect(button, &Button::clicked, this, member);
    return button;
}

void Calculator::digitClicked()
{
    Button* button = qobject_cast<Button*>(sender());
    int digitValue = button->text().toInt();

    // prevent "00" display. "0.00" is still valid
    if (digitValue == 0 && display->text() == "0")
        return;

    if (waitingForOperand)
    {
        display->clear();
        waitingForOperand = false;
    }

    display->setText(display->text() + button->text());
}

void Calculator::addictiveOperatorClicked()
{
    Button* button = qobject_cast<Button*>(sender());

    QString clickedOperator = button->text();
    double operand = display->text().toDouble();

    if (pendingMultiplicativeOperator.isEmpty() == false) // "*/" is waiting
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0;
        pendingMultiplicativeOperator.clear();
    }

    if (pendingAddictiveOperator.isEmpty() == false) // "+-" is waiting
    {
        if (!calculate(operand, pendingAddictiveOperator))
        {
            abortOperation();
            return;
        }
        display->setText(QString::number(sumSoFar));
    }
    else
    {
        sumSoFar = operand;
    }

    pendingAddictiveOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::multiplicativeOperatorClicked()
{
    Button* button = qobject_cast<Button*>(sender());

    QString clickedOperator = button->text();
    double operand = display->text().toDouble();

    if (pendingMultiplicativeOperator.isEmpty() == false) // "*/" is waiting
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            abortOperation();
            return;
        }
        display->setText(QString::number(factorSoFar));
        pendingMultiplicativeOperator.clear();
    }
    else
    {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;

}

void Calculator::equalClicked()
{
    double operand = display->text().toDouble();
    if (!pendingMultiplicativeOperator.isEmpty())
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAddictiveOperator.isEmpty())
    {
        if (!calculate(operand, pendingAddictiveOperator))
        {
            abortOperation();
            return;
        }
        pendingAddictiveOperator.clear();
    }
    else
    {
        sumSoFar = operand;
    }
    display->setText(QString::number(sumSoFar));
    sumSoFar = 0;
    waitingForOperand = true;
}
