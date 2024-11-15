#include "Visualizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QIcon appIcon("Resources/icon.png");
    a.setWindowIcon(appIcon); 

    Visualizer w;
    w.show();
    w.setWindowState(Qt::WindowMaximized);

    return a.exec();
}
