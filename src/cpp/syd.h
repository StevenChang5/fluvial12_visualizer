#ifndef SYD_H
#define SYD_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QComboBox>
#include <QLineSeries>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QString>
#include <QValueAxis>

#include "hydrograph.h"

class SYDWindow : public QWidget{
    Q_OBJECT
    public:
        explicit SYDWindow(QWidget *parent = 0);
    private:
        int m_counter;
        HydrographFile* hfile;
        QPushButton* m_button;
        QString fileName;
        QPushButton* getFileButton;
        QFormLayout* formGroupLayout;
        QHBoxLayout* viewer_layout;
        QVBoxLayout* control_layout;
        QLineSeries* chart_data;
        QChartView* chart_view;
        QChart* chart;
        QComboBox* cs_selector;
        QValueAxis* axis_x;
        QValueAxis* axis_y;
    signals:
        void counterReached();
        void fileUploaded();
    private slots:
        void getFileButtonClicked();
        void getFileUploaded();
        void csSelectorChanged(const QString& text);
};

#endif