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

        class DataSeries{
            public:
                DataSeries(std::string name, QChartView* chart_view, QValueAxis* x, QValueAxis* y, QBoxLayout* layout, bool isVisible=true, QWidget* parent=0);
                void setVisible(bool isVisible);

                QLineSeries* line_series;
                QCheckBox* check_box;
        };

        DataSeries* output_init;
        DataSeries* output_peak;
        DataSeries* output_end;
        DataSeries* output_ws;
        DataSeries* syd_peak;
        DataSeries* syd_end;
        
        // Chart
        QValueAxis* axis_x;
        QValueAxis* axis_y;
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
        QCheckBox* check_data_scour;
        QCheckBox* check_syd_peak;
        QCheckBox* check_syd_end;

        // Upload/Download Group
        QPushButton* upload_button;
        QPushButton* save_button;
        QPushButton* upload_scour_button;
        
    signals:
        void counterReached();
        void fileUploaded();
        
    private slots:
        void getFileButtonClicked();
        void getScourFileButtonClicked();
        void saveFileButtonClicked();
        void getFileUploaded();
        void csSelectorChanged(const QString& text);
        void sydSelectorChanged(const QString& text);
        void csToSyd();
        void sydToCs();
};

#endif