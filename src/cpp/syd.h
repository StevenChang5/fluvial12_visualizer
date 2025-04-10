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
        HydrographFile* hfile;
        QString fname;

        // Upload/Download Group
        QFormLayout* load_group_layout;
        QPushButton* upload_button;
        
        // Viewer Group
        QHBoxLayout* viewer_layout;
        QValueAxis* axis_x;
        QValueAxis* axis_y;
        QLineSeries* chart_data_initial;
        QLineSeries* chart_data_peak;
        QLineSeries* chart_data_end;
        QChartView* chart_view;
        QChart* chart;
        QVBoxLayout* control_layout;
        
        // Control Group
        QComboBox* cs_selector;
        
    signals:
        void counterReached();
        void fileUploaded();
    private slots:
        void getFileButtonClicked();
        void getFileUploaded();
        void csSelectorChanged(const QString& text);
};

#endif