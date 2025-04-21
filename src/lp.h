#ifndef LP_H
#define LP_H

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

#include "tsumFile.h"

class LPWindow : public QWidget{
    Q_OBJECT
    public:
        explicit LPWindow(QWidget* parent=0);
    private:
        TSUMFile* ts_file;
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

        DataSeries* water_surface;
        DataSeries* initial;
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