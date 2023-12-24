#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

namespace Ui {
class scorePanel;
}

class scorePanel : public QWidget
{
    Q_OBJECT

public:
    explicit scorePanel(QWidget *parent = nullptr);
    void setScores(int left,int right,int user);
    ~scorePanel();

private:
    Ui::scorePanel *ui;
};

#endif // SCOREPANEL_H
