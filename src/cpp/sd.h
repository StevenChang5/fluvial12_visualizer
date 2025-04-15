#ifndef SD_H
#define SD_H

#include <QWidget>
#include <QChartView>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineSeries>
#include <QPushButton>
#include <QValueAxis>
#include <QVBoxLayout>

#include "ty.h"

class SDWindow : public QWidget{
    Q_OBJECT
    public:
        explicit SDWindow(QWidget* parent=0);
    private:
        TYFile* ty_file;
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

        DataSeries* peak;
        DataSeries* end;

        // Chart
        QValueAxis* axis_x;
        QValueAxis* axis_y;
        QChartView* chart_view;
        float min_y;
        float max_y;
        float min_x;
        float max_x;

        // Upload/Download Group
        QPushButton* upload_button;
        QPushButton* save_button;
        QPushButton* upload_scour_button;

    private slots:
        void getFileButtonClicked();
        void saveFileButtonClicked();
};

#endif