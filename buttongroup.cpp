#include "buttongroup.h"
#include "ui_buttongroup.h"
ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

void ButtonGroup::initButtons()
{
    // 开始游戏
    ui->startBut->setImage(":/images/start-1.png", ":/images/start-3.png", ":/images/start-2.png");
    // 出牌
    ui->sendHandCardBut->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    ui->sendHandCard->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");

    // 不要
    ui->JmpCard->setImage(":/images/pass_btn-1.png", ":/images/pass_btn-3.png", ":/images/pass_btn-2.png");
    // 不抢
    ui->giveUp->setImage(":/images/buqiang-1.png", ":/images/buqiang-3.png", ":/images/buqiang-2.png");
    // 1,2,3分
    ui->scoreOne->setImage(":/images/1fen-1.png", ":/images/1fen-3.png", ":/images/1fen-2.png");
    ui->scoreTwo->setImage(":/images/2fen-1.png", ":/images/2fen-3.png", ":/images/2fen-2.png");
    ui->scoreThree->setImage(":/images/3fen-1.png", ":/images/3fen-3.png", ":/images/3fen-2.png");

    // 设置按钮的大小
    QVector<startButton*>buts;
    buts<<ui->startBut<<ui->JmpCard<<ui->sendHandCard<<ui->sendHandCardBut<<ui->giveUp<<ui->scoreOne<<ui->scoreThree<<ui->scoreTwo;
    for(auto &ve:buts)
    {
        ve->setFixedSize(85,45);
    }

    //链接每个按钮的处理函数
    connect(ui->startBut, &startButton::clicked, this,&ButtonGroup::startGame);
    connect(ui->sendHandCardBut, &startButton::clicked, this, &ButtonGroup::playHandSendCard);
    connect(ui->sendHandCard, &startButton::clicked, this, &ButtonGroup::playHandSendCard);
    connect(ui->JmpCard, &startButton::clicked, this, &ButtonGroup::passSendCard);
    connect(ui->giveUp, &startButton::clicked, this, [=]()
            {
                emit betPoint(0);
            });
    connect(ui->scoreOne, &startButton::clicked, this, [=]()
            {
                emit betPoint(1);
            });
    connect(ui->scoreTwo, &startButton::clicked, this, [=]()
            {
                emit betPoint(2);
            });
    connect(ui->scoreThree, &startButton::clicked, this, [=]()
            {
                emit betPoint(3);
            });
}


ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::selectPanel(Panel type, int bet)
{
    // enum Panel{Start, PlayCard, PassOrPlay, CallLord, Empty};
    ui->stackedWidget->setCurrentIndex(type);
    //不是叫地主只选择页面不用下注
    if(type != CallLord)
    {
        return;
    }
    if(bet == 0)
    {
        ui->scoreOne->setVisible(true);
        ui->scoreTwo->setVisible(true);
        ui->scoreThree->setVisible(true);
    }
    else if(bet == 1)
    {
        ui->scoreOne->setVisible(false);
        ui->scoreTwo->setVisible(true);
        ui->scoreThree->setVisible(true);
    }
    else if(bet == 2)
    {
        ui->scoreOne->setVisible(false);
        ui->scoreTwo->setVisible(true);
        ui->scoreThree->setVisible(true);
    }
}
