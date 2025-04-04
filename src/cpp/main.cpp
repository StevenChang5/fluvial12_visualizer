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

    // QTTabWidget tasks;
    // QString syd("Soil Yield Deposition"); 
    // Window* window = new Window(&tabs);
    // tabs.addTab(window, tab1);
    // tabs.show();
    // QString tab2("Test 2");
    // QWidget* window2 = new QWidget(&tabs);
    // window2->setFixedSize(1000,1000);
    // tabs.addTab(window2, tab2);
    
    return app.exec();
}