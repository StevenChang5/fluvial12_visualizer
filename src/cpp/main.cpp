#include <QApplication>
#include <QTabWidget>
#include <QMainWindow>
#include "syd.h"
#include "lsd.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);

    QMainWindow window = QMainWindow();
    window.setMinimumSize(800, 500);

    QTabWidget* tasks = new QTabWidget(&window);
    SYDWindow* syd_window = new SYDWindow(tasks);
    QString syd("Soild Yield Deposition");
    tasks->addTab(syd_window, syd);

    LSDWindow* sd_window = new LSDWindow(tasks);
    QString sd("Sediment Delivery");
    tasks->addTab(sd_window, sd);

    window.setCentralWidget(tasks);

    window.show();
    
    return app.exec();
}