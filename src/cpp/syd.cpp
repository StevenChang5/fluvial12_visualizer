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
    
    chart_data = new QLineSeries();
    chart_view = new QChartView(this);
    chart = new QChart();
    chart->addSeries(chart_data);
    chart_view->setChart(chart);
    chart_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    axis_x = new QValueAxis();
    axis_y = new QValueAxis();
    chart->addAxis(axis_x, Qt::AlignBottom);
    chart_data->attachAxis(axis_x);
    chart->addAxis(axis_y, Qt::AlignLeft);
    chart_data->attachAxis(axis_y);
    
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

    chart_data->clear();
    auto coor = cs->get_coor("0");
    int min_y = INT_MAX;
    int max_y = INT_MIN;
    int min_x = INT_MAX;
    int max_x = INT_MIN;
    for(int i = 0; i < coor.size(); i++){
        int x = std::get<0>(coor[i]);
        int y = std::get<1>(coor[i]);
        min_x = std::min(x, min_x);
        max_x = std::max(x, max_x);
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
        chart_data->append(x, y);
    }
    axis_x->setRange(min_x, max_x);
    axis_y->setRange(min_y, max_y);
    
    chart->legend()->hide();
}
