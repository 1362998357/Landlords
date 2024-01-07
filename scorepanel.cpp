#include "scorepanel.h"
#include "ui_scorepanel.h"
const QString MYCOLOR[] = {"black", "white", "red", "blue", "green"};
scorePanel::scorePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scorePanel)
{
    ui->setupUi(this);
    m_list << ui->userScore << ui->lefttRobotScore << ui->rightRobotScore
           << ui->meTitle << ui->leftTitle << ui->rightTiltle
           << ui->score1 << ui->score2 << ui->score3;
}

void scorePanel::setScores(int left, int right, int user)
{
    ui->lefttRobotScore->setText(QString::number(left));
    ui->rightRobotScore->setText(QString::number(right));
    ui->userScore->setText(QString::number(user));
}

void scorePanel::setMyFontSize(int point)
{
    QFont font("微软雅黑", point, QFont::Bold);
    for(int i=0; i<m_list.size(); ++i)
    {
        m_list[i]->setFont(font);
    }
}

void scorePanel::setMyFontColor(FontColor color)
{
    QString style = QString("QLabel{color:%1}").arg(MYCOLOR[color]);
    for(int i=0; i<m_list.size(); ++i)
    {
        m_list[i]->setStyleSheet(style);
    }
}
scorePanel::~scorePanel()
{
    delete ui;
}
