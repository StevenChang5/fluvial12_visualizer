#include <QApplication>
#include <QTabWidget>
#include <QMainWindow>
#include "syd.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);

    QMainWindow window = QMainWindow();
    window.setMinimumSize(800, 500);

    QTabWidget* tasks = new QTabWidget(&window);
    SYDWindow* syd_window = new SYDWindow(tasks);
    QString syd("Soild Yield Deposition");
    tasks->addTab(syd_window, syd);

    window.setCentralWidget(tasks);

    window.show();
    
    return app.exec();
}