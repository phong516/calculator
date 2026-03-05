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
    Button* clearButton = createButton("Clear", &Calculator::digitClicked);
    Button* clearAllButton = createButton("Clear All", &Calculator::digitClicked);

    Button* memoryClearButton = createButton("MC", &Calculator::digitClicked);
    Button* memoryReadButton = createButton("MR", &Calculator::digitClicked);
    Button* memorySetButton = createButton("MS", &Calculator::digitClicked);
    Button* memoryAddButton = createButton("M+", &Calculator::digitClicked);

    Button* pointButton = createButton(".", &Calculator::digitClicked);
    Button* changeSignButton = createButton("\302\261", &Calculator::digitClicked);
    Button* addButton = createButton("+", &Calculator::digitClicked);
    Button* minusButton = createButton("-", &Calculator::digitClicked);
    Button* multiplyButton = createButton("x", &Calculator::digitClicked);
    Button* divideButton = createButton("/", &Calculator::digitClicked);

    Button* squareRootButton = createButton("sqrt", &Calculator::digitClicked);
    Button* squareButton = createButton("\302\262", &Calculator::digitClicked);
    Button* reciprocalButton = createButton("1/x", &Calculator::digitClicked);
    Button* equalButton = createButton("=", &Calculator::digitClicked);


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
    display->setText(button->text());
}

