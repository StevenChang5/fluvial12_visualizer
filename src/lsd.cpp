#include "float.h"
#include "lsd.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>

LSDWindow::LSDWindow(QWidget* parent) : QWidget(parent){
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    chart_view = new QChartView(this);
    chart_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    axis_x = new QValueAxis();
    axis_x->setTitleText("Station (ft)");
    axis_x->setTickCount(10);
    axis_y = new QValueAxis();
    axis_y->setTickCount(10);
    axis_y->setTitleText("Elevation (ft)");

    QGroupBox* load_group = new QGroupBox(tr("Upload/Save Files"), this);
    upload_button = new QPushButton("Upload TYY", this);
    connect(upload_button, SIGNAL(clicked()), this, SLOT(getFileButtonClicked()));
    save_button = new QPushButton("Save Graph", this);
    connect(save_button, SIGNAL(clicked()), this, SLOT(saveFileButtonClicked()));
    save_button->setEnabled(false);

    QVBoxLayout* load_group_layout = new QVBoxLayout(this);
    load_group_layout->addWidget(upload_button);
    load_group_layout->addWidget(save_button);
    load_group->setLayout(load_group_layout);

    QVBoxLayout* control_layout = new QVBoxLayout(this);
    view_group = new QGroupBox(tr("Viewer Controls"), this);
    view_group_layout = new QVBoxLayout(this);
    view_group->setLayout(view_group_layout);
    view_group->setEnabled(false);
    view_group_layout->addWidget(new QLabel("View:", this));
    control_layout->addWidget(view_group);
    control_layout->addWidget(load_group);

    chart_view->chart()->addAxis(axis_x, Qt::AlignBottom);
    chart_view->chart()->addAxis(axis_y, Qt::AlignLeft);
    chart_view->chart()->setTitle(QString::fromStdString("Sediment Delivery"));

    peak = new DataSeries("Peak", chart_view, axis_x, axis_y, view_group_layout);
    end = new DataSeries("End", chart_view, axis_x, axis_y, view_group_layout);

    view_group_layout->addStretch();

    QHBoxLayout* viewer_layout = new QHBoxLayout(this);
    viewer_layout->addWidget(chart_view);
    viewer_layout->addLayout(control_layout);

    main_layout->addLayout(viewer_layout);
    return;
}

void LSDWindow::getFileButtonClicked(){
    fname = QFileDialog::getOpenFileName(this,"Select a file", "","TYY File (*TYY);;All Files (*)" );
    min_x = FLT_MAX;
    max_x = FLT_MIN;
    min_y = FLT_MAX;
    max_y = FLT_MIN;
    if(!fname.isEmpty()){
        ty_file = new TYYFile(fname.toStdString());
        std::vector<std::tuple<float,float,float>> coors = ty_file->getCoordinates();
        for(int i = 0; i < coors.size(); i++){
            float x = std::get<0>(coors[i]);
            float y_peak = std::get<1>(coors[i]);
            float y_end = std::get<2>(coors[i]);
            min_x = std::min(x, min_x);
            max_x = std::max(x, max_x);
            min_y = std::min(y_peak, min_y);
            max_y = std::max(y_peak, max_y);
            min_y = std::min(y_end, min_y);
            max_y = std::max(y_end, max_y);
            peak->line_series->append(x, y_peak);
            end->line_series->append(x, y_end);
        }
        view_group->setEnabled(true);
        save_button->setEnabled(true);
    }
    axis_x->setRange(min_x, max_x);
    axis_y->setRange(min_y, max_y);
}

void LSDWindow::saveFileButtonClicked(){
    save_path = QFileDialog::getExistingDirectory(this,"Select a save directory");
    save_path += "/sediment_delivery";
    QString new_save_path = save_path + ".png";
    int copy = 1;
    if(!save_path.isEmpty()){
        while(std::filesystem::exists(new_save_path.toStdString())){
            new_save_path = save_path + QString::fromStdString(std::format("({})",copy)) + ".png";
            copy++;
        }
        chart_view->grab().save(new_save_path);
        return;
    }
}

LSDWindow::DataSeries::DataSeries(std::string name, QChartView* chart_view, QValueAxis* x, QValueAxis* y, QBoxLayout* layout, bool isVisible, QWidget* parent){
    line_series = new QLineSeries();
    line_series->setName(QString::fromStdString(name));
    chart_view->chart()->addSeries(line_series);
    
    line_series->attachAxis(x);
    line_series->attachAxis(y);
    line_series->setPointsVisible(isVisible);
    line_series->setVisible(isVisible);

    check_box = new QCheckBox(QString::fromStdString(name),parent);
    check_box->setVisible(isVisible);
    check_box->setChecked(true);
    layout->addWidget(check_box);

    connect(check_box, &QCheckBox::toggled, line_series, [this](bool t){line_series->setVisible(t);});
}

void LSDWindow::DataSeries::setVisible(bool isVisible){
    line_series->setVisible(isVisible);
    check_box->setVisible(isVisible);
    check_box->setChecked(isVisible);
}