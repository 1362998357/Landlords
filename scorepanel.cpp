#include "scorepanel.h"
#include "ui_scorepanel.h"

scorePanel::scorePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scorePanel)
{
    ui->setupUi(this);
}

void scorePanel::setScores(int left, int right, int user)
{
    ui->lefttRobotScore->setText(QString::number(left));
    ui->rightRobotScore->setText(QString::number(right));
    ui->userScore->setText(QString::number(user));
}

scorePanel::~scorePanel()
{
    delete ui;
}
