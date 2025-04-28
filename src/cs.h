#ifndef CS_H
#define CS_H

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

#include "outputFile.h"

class CSWindow : public QWidget{
    Q_OBJECT
    public:
        explicit CSWindow(QWidget *parent = 0);
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
        DataSeries* scour;

        bool scour_loaded;
        
        // Chart
        QValueAxis* axis_x;
        QValueAxis* axis_y;
        QChartView* chart_view;
        float min_y;
        float max_y;
        float min_x;
        float max_x;
        
        // Control Group
        QComboBox* cs_selector;
        QRadioButton* cs_radio;
        QRadioButton* syd_radio;

        // Upload/Download Group
        QPushButton* upload_button;
        QPushButton* save_button;
        QPushButton* upload_scour_button;
        
    signals:
        void fileUploaded();
        void displayScour();
        
    private slots:
        void getFileButtonClicked();
        void getScourFileButtonClicked();
        void saveFileButtonClicked();
        void getFileUploaded();
        void csSelectorChanged(const QString& text);
        void sydSelectorChanged(const QString& text);
        void csToSyd();
        void sydToCs();
        void displayScourReceived();
};

#endif