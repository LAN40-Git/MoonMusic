#include "playercontrol.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerControl newPlayer;
    newPlayer.show();
    return a.exec();
}
