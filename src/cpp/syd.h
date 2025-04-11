#ifndef SYD_H
#define SYD_H

#include <QWidget>
#include <QChartView>
#include <QChart>
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

        // Upload/Download Group
        QFormLayout* load_group_layout;
        QPushButton* upload_button;
        QPushButton* save_button;
        
        // Viewer Group
        QHBoxLayout* viewer_layout;
        QValueAxis* axis_x;
        QValueAxis* axis_y;
        int min_y;
        int max_y;
        int min_x;
        int max_x;
        QLineSeries* chart_data_initial;
        QLineSeries* chart_data_peak;
        QLineSeries* chart_data_end;
        QLineSeries* chart_syd_peak;
        QLineSeries* chart_syd_end;
        QChartView* chart_view;
        QChart* chart;
        QCheckBox* check_data_initial;
        QCheckBox* check_data_peak;
        QCheckBox* check_data_end;
        QCheckBox* check_syd_peak;
        QCheckBox* check_syd_end;
        QVBoxLayout* control_layout;
    
        
        // Control Group
        QComboBox* cs_selector;
        QRadioButton* cs_radio;
        QRadioButton* syd_radio;
        
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