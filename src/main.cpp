#include <QApplication>
#include <QTabWidget>
#include <QMainWindow>
#include "cs.h"
#include "lp.h"
#include "lsd.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);

    QMainWindow window = QMainWindow();
    window.setMinimumSize(800, 500);

    QTabWidget* tasks = new QTabWidget(&window);
    CSWindow* cs_window = new CSWindow(tasks);
    QString cs("Cross Section");
    tasks->addTab(cs_window, cs);

    LPWindow* lp_window = new LPWindow(tasks);
    QString lp("Longitudinal Profile");
    tasks->addTab(lp_window, lp);

    LSDWindow* sd_window = new LSDWindow(tasks);
    QString sd("Sediment Delivery");
    tasks->addTab(sd_window, sd);

    window.setCentralWidget(tasks);

    window.show();
    
    return app.exec();
}