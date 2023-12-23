#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>
/*
 * 进入游戏后下方显示的各个按钮
*/
namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
