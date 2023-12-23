#include "scorepanel.h"
#include "ui_scorepanel.h"

scorePanel::scorePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scorePanel)
{
    ui->setupUi(this);
}

scorePanel::~scorePanel()
{
    delete ui;
}
