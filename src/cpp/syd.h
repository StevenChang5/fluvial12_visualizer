#ifndef SYD_H
#define SYD_H

#include <QWidget>
#include <QGroupBox>
#include <QChartView>
#include <QCheckBox>
#include <QComboBox>
#include <QLineSeries>
#include <QPushButton>
#include <QRadioButton>
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
        QString save_path;

        QGroupBox* view_group;
        QVBoxLayout* view_group_layout;
        
        // Chart
        QValueAxis* axis_x;
        QValueAxis* axis_y;
        QLineSeries* chart_data_initial;
        QLineSeries* chart_data_peak;
        QLineSeries* chart_data_end;
        QLineSeries* chart_data_ws;
        QLineSeries* chart_syd_peak;
        QLineSeries* chart_syd_end;
        QChartView* chart_view;
        int min_y;
        int max_y;
        int min_x;
        int max_x;
        
        // Control Group
        QComboBox* cs_selector;
        QRadioButton* cs_radio;
        QRadioButton* syd_radio;
        QCheckBox* check_data_initial;
        QCheckBox* check_data_peak;
        QCheckBox* check_data_end;
        QCheckBox* check_data_ws;
        QCheckBox* check_syd_peak;
        QCheckBox* check_syd_end;

        // Upload/Download Group
        QPushButton* upload_button;
        QPushButton* save_button;
        
    signals:
        void counterReached();
        void fileUploaded();
        
    private slots:
        void getFileButtonClicked();
        void saveFileButtonClicked();
        void getFileUploaded();
        void csSelectorChanged(const QString& text);
        void sydSelectorChanged(const QString& text);
        void csToSyd();
        void sydToCs();
};

#endif