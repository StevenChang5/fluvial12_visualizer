#ifndef SYD_H
#define SYD_H

#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QPushButton>
#include <QFormLayout>

class SYDWindow : public QWidget{
    Q_OBJECT
    public:
        explicit SYDWindow(QWidget *parent = 0);
    private:
        int m_counter;
        QPushButton *m_button;
        QString fileName;
        QPushButton* getFileButton;
        QFormLayout *formGroupLayout;
        QLineSeries* graphData;
        QChartView* chartView;
    signals:
        void counterReached();
    private slots:
        void slotButtonClicked(bool checked);
        void getFileButtonClicked();
};

#endif