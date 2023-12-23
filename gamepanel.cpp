#include "gamepanel.h"
#include "ui_gamepanel.h"
#include <QRandomGenerator>
#include <QPainter>
GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);
    // 1. 背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(num+1);
    m_bkImage.load(path);

    // 2. 窗口的标题的大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000, 650);

    // 3. 实例化游戏控制类对象
    gameControlInit();

    // 4. 玩家得分(更新)
    updatePlayerScore();
    //初始化按钮组
    ui->butGroup->initButtons();
    ui->butGroup->selectPanel(ButtonGroup::Start);
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameControlInit()
{

}

void GamePanel::updatePlayerScore()
{

}

void GamePanel::initCardMap()
{

}

void GamePanel::cropImage(QPixmap &pix, int x, int y, Card &c)
{

}

void GamePanel::initPlayerContext()
{

}

void GamePanel::initGameScene()
{

}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{

}

void GamePanel::paintEvent(QPaintEvent *ev)
{
        Q_UNUSED(ev)
        QPainter p(this);
        p.drawPixmap(rect(), m_bkImage);
}

