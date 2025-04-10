#include "syd.h"

#include <string>

#include <QApplication>
#include <QProgressBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QSizePolicy>

#include <QDebug>

SYDWindow::SYDWindow(QWidget *parent) : QWidget(parent){

    QVBoxLayout *main_layout = new QVBoxLayout(this);

    // Upload and save file controls
    QGroupBox* load_group = new QGroupBox(tr("Upload/Save Files"), this);
    upload_button = new QPushButton("Upload File", this);
    connect(upload_button, SIGNAL(clicked()), this, SLOT(getFileButtonClicked()));
    QLineEdit* upload_line_edit = new QLineEdit(this);
    upload_line_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    load_group_layout = new QFormLayout(this);
    load_group_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    load_group_layout->addRow(upload_button, upload_line_edit);
    load_group_layout->addRow(new QLabel(tr("Save File:")), new QLineEdit);
    load_group->setLayout(load_group_layout);

    // Viewer and controls
    chart_data_initial = new QLineSeries();
    chart_data_peak = new QLineSeries();
    chart_data_end = new QLineSeries();
    chart_view = new QChartView(this);
    chart = new QChart();
    chart->addSeries(chart_data_initial);
    chart->addSeries(chart_data_peak);
    chart->addSeries(chart_data_end);
    chart_view->setChart(chart);
    chart_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    axis_x = new QValueAxis();
    axis_y = new QValueAxis();
    chart->addAxis(axis_x, Qt::AlignBottom);
    chart_data_initial->attachAxis(axis_x);
    chart_data_peak->attachAxis(axis_x);
    chart_data_end->attachAxis(axis_x);
    chart->addAxis(axis_y, Qt::AlignLeft);
    chart_data_initial->attachAxis(axis_y);
    chart_data_peak->attachAxis(axis_y);
    chart_data_end->attachAxis(axis_y);
    
    control_layout = new QVBoxLayout(this);
    cs_selector = new QComboBox(this);
    cs_selector->setEnabled(false);
    control_layout->addWidget(cs_selector);
    control_layout->addStretch();
    connect(this, SIGNAL(fileUploaded()), this, SLOT(getFileUploaded()));
    connect(cs_selector, SIGNAL(currentTextChanged(const QString&)), this, SLOT(csSelectorChanged(const QString&)));
    
    viewer_layout = new QHBoxLayout(this);
    viewer_layout->addWidget(chart_view);
    viewer_layout->addLayout(control_layout);

    main_layout->addLayout(viewer_layout);
    main_layout->addWidget(load_group);

}

void SYDWindow::getFileButtonClicked(){
    fname = QFileDialog::getOpenFileName(this,"Select a file", "","Text Files (*.txt);;All Files (*)" );
    if(!fname.isEmpty()){
        QLayoutItem* lineEdit = load_group_layout->itemAt(0,QFormLayout::FieldRole);
        QLineEdit* edit = qobject_cast<QLineEdit*>(lineEdit->widget());
        edit->setText(fname);
        hfile = new HydrographFile(fname.toStdString());
        emit fileUploaded();
    }
}

void SYDWindow::getFileUploaded(){
    cs_selector->setEnabled(true);
    for(int i = 1; i < hfile->sections.size(); i++){
        Crosssection* temp = hfile->sections[i];
        cs_selector->addItem(QString::fromStdString(std::format("ID: {}, Name: {}",temp->get_id(), temp->get_name())));
    }
}

void SYDWindow::csSelectorChanged(const QString& text){
    std::string data = text.toStdString();
    int idx_left = data.find(' ')+1;
    int idx_right = data.find(',');
    int id = stoi(data.substr(idx_left, idx_right-idx_left));

    Crosssection* cs = hfile->sections[id];
    chart_data_initial->clear();
    chart_data_peak->clear();
    chart_data_end->clear();
    auto coor_initial = cs->get_coor("0");
    auto coor_peak = cs->get_coor(hfile->get_approx_peak());
    auto coor_end = cs->get_coor(hfile->get_approx_end());
    int min_y = INT_MAX;
    int max_y = INT_MIN;
    int min_x = INT_MAX;
    int max_x = INT_MIN;
    qDebug() << hfile->get_approx_peak() << " " << hfile->get_approx_end();
    for(int i = 0; i < coor_initial.size(); i++){
        int x_initial = std::get<0>(coor_initial[i]);
        int y_initial = std::get<1>(coor_initial[i]);
        min_x = std::min(x_initial, min_x);
        max_x = std::max(x_initial, max_x);
        min_y = std::min(y_initial, min_y);
        max_y = std::max(y_initial, max_y);
        chart_data_initial->append(x_initial, y_initial);
        
    }
    for(int i = 0; i < coor_peak.size(); i++){
        int x_peak = std::get<0>(coor_peak[i]);
        int y_peak = std::get<1>(coor_peak[i]);
        min_x = std::min(x_peak, min_x);
        max_x = std::max(x_peak, max_x);
        min_y = std::min(y_peak, min_y);
        max_y = std::max(y_peak, max_y);
        chart_data_peak->append(x_peak, y_peak);
    }
    for(int i = 0; i < coor_end.size(); i++){
        int x_end = std::get<0>(coor_end[i]);
        int y_end = std::get<1>(coor_end[i]);
        min_x = std::min(x_end, min_x);
        max_x = std::max(x_end, max_x);
        min_y = std::min(y_end, min_y);
        max_y = std::max(y_end, max_y);
        chart_data_end->append(x_end, y_end);
    }
    axis_x->setRange(min_x, max_x);
    axis_y->setRange(min_y, max_y);
}
