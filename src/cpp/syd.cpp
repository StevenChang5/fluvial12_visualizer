#include "syd.h"

#include <filesystem>
#include <format>
#include <string>

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QSizePolicy>

#include <QDebug>

SYDWindow::SYDWindow(QWidget *parent) : QWidget(parent){

    QVBoxLayout *main_layout = new QVBoxLayout(this);

    /********************************************************
     * Viewer data initialization for cs and syd functions
     * Creates line series for intial, peak, and end for cs
     * Creates line series for peak, and for syd
    ********************************************************/
    // cs function
    chart_data_initial = new QLineSeries();
    chart_data_initial->setName(QString("Initial"));
    chart_data_initial->setPointsVisible(true);
    chart_data_peak = new QLineSeries();
    chart_data_peak->setName(QString("Peak"));
    chart_data_peak->setPointsVisible(true);
    chart_data_end = new QLineSeries();
    chart_data_end->setName(QString("End"));
    chart_data_end->setPointsVisible(true);

    // syd function
    chart_syd_peak = new QLineSeries();
    chart_syd_peak->setName(QString("SYD Peak"));
    chart_syd_peak->setVisible(false);
    chart_syd_end = new QLineSeries();
    chart_syd_end->setName(QString("SYD End"));
    chart_syd_end->setVisible(false);

    /********************************************************
     * Chart initialization 
    ********************************************************/
    chart_view = new QChartView(this);
    chart_view->chart()->addSeries(chart_data_initial);
    chart_view->chart()->addSeries(chart_data_peak);
    chart_view->chart()->addSeries(chart_data_end);
    chart_view->chart()->addSeries(chart_syd_peak);
    chart_view->chart()->addSeries(chart_syd_end);
    chart_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    axis_x = new QValueAxis();
    axis_y = new QValueAxis();
    chart_view->chart()->addAxis(axis_x, Qt::AlignBottom);
    chart_data_initial->attachAxis(axis_x);
    chart_data_peak->attachAxis(axis_x);
    chart_data_end->attachAxis(axis_x);
    chart_syd_peak->attachAxis(axis_x);
    chart_syd_end->attachAxis(axis_x);
    chart_view->chart()->addAxis(axis_y, Qt::AlignLeft);
    chart_data_initial->attachAxis(axis_y);
    chart_data_peak->attachAxis(axis_y);
    chart_data_end->attachAxis(axis_y);
    chart_syd_peak->attachAxis(axis_y);
    chart_syd_end->attachAxis(axis_y);

    /********************************************************
     * Control initialization
     * Creates radio buttons to switch between cs and syd
     * Creates check boxes to toggle viewing of init/peak/end
     * Creates combo box to select different crosssections
     * Creates upload/save file buttons
    ********************************************************/

    // Radio buttons
    cs_radio = new QRadioButton("Crosssection",this);
    syd_radio = new QRadioButton("Sediment Yield Tons",this);

    // Check boxes
    check_data_initial = new QCheckBox("Initial",this);
    check_data_peak = new QCheckBox("Peak",this);
    check_data_end = new QCheckBox("End",this);
    check_syd_peak = new QCheckBox("Peak",this);
    check_syd_end = new QCheckBox("End",this);
    check_syd_peak->setVisible(false);
    check_syd_end->setVisible(false);
    check_data_initial->setChecked(true);
    check_data_peak->setChecked(true);
    check_data_end->setChecked(true);
    check_syd_peak->setChecked(true);
    check_syd_end->setChecked(true);

    // Combo Box
    cs_selector = new QComboBox(this);
    cs_selector->setEnabled(false);

    // Upload and save buttons
    QGroupBox* load_group = new QGroupBox(tr("Upload/Save Files"), this);
    upload_button = new QPushButton("Upload File", this);
    connect(upload_button, SIGNAL(clicked()), this, SLOT(getFileButtonClicked()));
    save_button = new QPushButton("Save File", this);
    connect(save_button, SIGNAL(clicked()), this, SLOT(saveFileButtonClicked()));

    QVBoxLayout* load_group_layout = new QVBoxLayout(this);
    load_group_layout->addWidget(upload_button);
    load_group_layout->addWidget(save_button);
    load_group->setLayout(load_group_layout);
    
    // Layout
    QVBoxLayout* control_layout = new QVBoxLayout(this);
    view_group = new QGroupBox(tr("Viewer Controls"), this);
    view_group_layout = new QVBoxLayout(this);
    view_group->setLayout(view_group_layout);
    view_group->setEnabled(false);
    view_group_layout->addWidget(new QLabel("Functions:",this));
    view_group_layout->addWidget(cs_radio);
    view_group_layout->addWidget(syd_radio);
    view_group_layout->addWidget(new QLabel("View:", this));
    view_group_layout->addWidget(check_data_initial);
    view_group_layout->addWidget(check_data_peak);
    view_group_layout->addWidget(check_data_end);
    view_group_layout->addWidget(check_syd_peak);
    view_group_layout->addWidget(check_syd_end);
    view_group_layout->addStretch();
    view_group_layout->addWidget(cs_selector);
    control_layout->addWidget(view_group);
    control_layout->addWidget(load_group);

    connect(this, SIGNAL(fileUploaded()), this, SLOT(getFileUploaded()));
    connect(cs_selector, SIGNAL(currentTextChanged(const QString&)), this, SLOT(csSelectorChanged(const QString&)));
    connect(cs_radio, SIGNAL(toggled(bool)), this, SLOT(sydToCs()));
    connect(syd_radio,SIGNAL(toggled(bool)), this, SLOT(csToSyd()));
    connect(check_data_initial, &QCheckBox::toggled, this, [this](bool t){this->chart_data_initial->setVisible(t); });
    connect(check_data_peak, &QCheckBox::toggled, this, [this](bool t){this->chart_data_peak->setVisible(t); });
    connect(check_data_end, &QCheckBox::toggled, this, [this](bool t){this->chart_data_end->setVisible(t); });
    connect(check_syd_peak, &QCheckBox::toggled, this, [this](bool t){this->chart_syd_peak->setVisible(t); });
    connect(check_syd_end, &QCheckBox::toggled, this, [this](bool t){this->chart_syd_end->setVisible(t); });

    QHBoxLayout* viewer_layout = new QHBoxLayout(this);
    viewer_layout->addWidget(chart_view);
    viewer_layout->addLayout(control_layout);

    main_layout->addLayout(viewer_layout);
}

void SYDWindow::getFileButtonClicked(){
    fname = QFileDialog::getOpenFileName(this,"Select a file", "","Text Files (*.txt);;All Files (*)" );
    if(!fname.isEmpty()){
        hfile = new HydrographFile(fname.toStdString());
        emit fileUploaded();
        cs_radio->toggle();
        view_group->setEnabled(true);
    }
}

void SYDWindow::saveFileButtonClicked(){
    save_path = QFileDialog::getExistingDirectory(this,"Select a save directory");
    save_path += "/graph";
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
    min_y = INT_MAX;
    max_y = INT_MIN;
    min_x = INT_MAX;
    max_x = INT_MIN;
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

void SYDWindow::sydSelectorChanged(const QString& text){
    std::string data = text.toStdString();
}

void SYDWindow::csToSyd(){
    cs_selector->setEnabled(false);
    check_data_initial->setVisible(false);
    check_data_peak->setVisible(false);
    check_data_end->setVisible(false);
    check_syd_peak->setVisible(true);
    check_syd_end->setVisible(true);
    check_syd_peak->setChecked(true);
    check_syd_end->setChecked(true);

    chart_data_initial->setVisible(false);
    chart_data_peak->setVisible(false);
    chart_data_end->setVisible(false);
    chart_syd_peak->setVisible(true);
    chart_syd_end->setVisible(true);
    chart_syd_peak->setPointsVisible(true);
    chart_syd_end->setPointsVisible(true);

    chart_syd_peak->clear();
    chart_syd_end->clear();
    float y = INT_MIN;
    for(int id = 1; id < hfile->sections.size()+1; id++){
        Crosssection* cs = hfile->sections[id];
        float peak = cs->getSyd(hfile->get_approx_peak());
        float end = cs->getSyd(hfile->get_approx_end());
        chart_syd_peak->append(stof(cs->get_name()),peak);
        chart_syd_end->append(stof(cs->get_name()),end);
        y = std::max(y, std::max(peak, end));
    }
    axis_x->setRange(stof(hfile->sections[1]->get_name()), stof(hfile->sections[hfile->sections.size()]->get_name()));
    axis_y->setRange(0, y);
}

void SYDWindow::sydToCs(){
    cs_selector->setEnabled(true);
    check_data_initial->setVisible(true);
    check_data_peak->setVisible(true);
    check_data_end->setVisible(true);
    check_syd_peak->setVisible(false);
    check_syd_end->setVisible(false);
    check_data_initial->setChecked(true);
    check_data_peak->setChecked(true);
    check_data_end->setChecked(true);

    chart_data_initial->setVisible(true);
    chart_data_peak->setVisible(true);
    chart_data_end->setVisible(true);
    chart_syd_peak->setVisible(false);
    chart_syd_end->setVisible(false);
    chart_syd_peak->setPointsVisible(false);
    chart_syd_end->setPointsVisible(false);
    axis_x->setRange(min_x, max_x);
    axis_y->setRange(min_y, max_y);
}