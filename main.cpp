#include "gamepanel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GamePanel w;
    //斗地主开始
    w.show();
    return a.exec();
}
