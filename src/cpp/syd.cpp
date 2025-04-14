#include "float.h"
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
     * Chart initialization 
    ********************************************************/
    chart_view = new QChartView(this);
    chart_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    axis_x = new QValueAxis();
    axis_y = new QValueAxis();

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

    // Combo Box
    cs_selector = new QComboBox(this);
    cs_selector->setEnabled(false);

    // Upload and save buttons
    QGroupBox* load_group = new QGroupBox(tr("Upload/Save Files"), this);
    upload_button = new QPushButton("Upload File", this);
    connect(upload_button, SIGNAL(clicked()), this, SLOT(getFileButtonClicked()));
    save_button = new QPushButton("Save Graph", this);
    connect(save_button, SIGNAL(clicked()), this, SLOT(saveFileButtonClicked()));
    upload_scour_button = new QPushButton("Upload TZMIN.OUT");
    connect(upload_scour_button, SIGNAL(clicked()), this, SLOT(getScourFileButtonClicked()));
    upload_scour_button->setEnabled(false);

    QVBoxLayout* load_group_layout = new QVBoxLayout(this);
    load_group_layout->addWidget(upload_button);
    load_group_layout->addWidget(upload_scour_button);
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

    chart_view->chart()->addAxis(axis_x, Qt::AlignBottom);
    chart_view->chart()->addAxis(axis_y, Qt::AlignLeft);

    output_init = new DataSeries("Initial", chart_view, axis_x, axis_y, view_group_layout);
    output_peak = new DataSeries("Peak", chart_view, axis_x, axis_y, view_group_layout);
    output_end = new DataSeries("End", chart_view, axis_x, axis_y, view_group_layout);
    output_ws = new DataSeries("W.S. at Peak", chart_view, axis_x, axis_y, view_group_layout);
    syd_peak = new DataSeries("Syd Peak", chart_view, axis_x, axis_y, view_group_layout);
    syd_end = new DataSeries("Syd End", chart_view, axis_x, axis_y, view_group_layout);
    scour = new DataSeries("Scour", chart_view, axis_x, axis_y, view_group_layout);
    scour->setVisible(false);
    scour_loaded = false;
    syd_peak->setVisible(false);
    syd_end->setVisible(false);

    view_group_layout->addStretch();
    view_group_layout->addWidget(cs_selector);
    control_layout->addWidget(view_group);
    control_layout->addWidget(load_group);

    connect(this, SIGNAL(fileUploaded()), this, SLOT(getFileUploaded()));
    connect(cs_selector, SIGNAL(currentTextChanged(const QString&)), this, SLOT(csSelectorChanged(const QString&)));
    connect(cs_radio, SIGNAL(toggled(bool)), this, SLOT(sydToCs()));
    connect(syd_radio,SIGNAL(toggled(bool)), this, SLOT(csToSyd()));
    connect(this, SIGNAL(displayScour()), this, SLOT(displayScourReceived()));

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
        upload_scour_button->setEnabled(true);
    }
}

void SYDWindow::getScourFileButtonClicked(){
    fname = QFileDialog::getOpenFileName(this,"Select TZMIN.OUT","","Text Files (*.OUT);;All Files (*)");
    if(!fname.isEmpty()){
        hfile->uploadScourFile(fname.toStdString());
        scour_loaded = true;
        emit displayScour();
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
        cs_selector->addItem(QString::fromStdString(std::format("ID: {}, Name: {}",temp->getId(), temp->getName())));
    }
}

void SYDWindow::csSelectorChanged(const QString& text){
    std::string data = text.toStdString();
    int idx_left = data.find(' ')+1;
    int idx_right = data.find(',');
    int id = stoi(data.substr(idx_left, idx_right-idx_left));

    Crosssection* cs = hfile->sections[id];
    output_init->line_series->clear();
    output_peak->line_series->clear();
    output_end->line_series->clear();
    output_ws->line_series->clear();
    auto coor_initial = cs->get_coor("0");
    auto coor_peak = cs->get_coor(hfile->getApproxPeak());
    auto coor_end = cs->get_coor(hfile->getApproxEnd());
    min_y = FLT_MAX;
    max_y = FLT_MIN;
    min_x = FLT_MAX;
    max_x = FLT_MIN;
    for(int i = 0; i < coor_initial.size(); i++){
        float x_initial = std::get<0>(coor_initial[i]);
        float y_initial = std::get<1>(coor_initial[i]);
        min_x = std::min(x_initial, min_x);
        max_x = std::max(x_initial, max_x);
        min_y = std::min(y_initial, min_y);
        max_y = std::max(y_initial, max_y);
        output_init->line_series->append(x_initial, y_initial);
        
    }
    for(int i = 0; i < coor_peak.size(); i++){
        float x_peak = std::get<0>(coor_peak[i]);
        float y_peak = std::get<1>(coor_peak[i]);
        min_x = std::min(x_peak, min_x);
        max_x = std::max(x_peak, max_x);
        min_y = std::min(y_peak, min_y);
        max_y = std::max(y_peak, max_y);
        output_peak->line_series->append(x_peak, y_peak);
    }
    for(int i = 0; i < coor_end.size(); i++){
        float x_end = std::get<0>(coor_end[i]);
        float y_end = std::get<1>(coor_end[i]);
        min_x = std::min(x_end, min_x);
        max_x = std::max(x_end, max_x);
        min_y = std::min(y_end, min_y);
        max_y = std::max(y_end, max_y);
        output_end->line_series->append(x_end, y_end);
    }
    output_ws->line_series->append(min_x, cs->getWsElev(hfile->getApproxPeak()));
    output_ws->line_series->append(max_x, cs->getWsElev(hfile->getApproxPeak()));
    if(scour_loaded){
        emit displayScour();
    }

    axis_x->setRange(min_x, max_x);
    axis_y->setRange(min_y, max_y);
}

void SYDWindow::sydSelectorChanged(const QString& text){
    std::string data = text.toStdString();
}

void SYDWindow::csToSyd(){
    output_init->setVisible(false);
    output_peak->setVisible(false);
    output_end->setVisible(false);
    output_ws->setVisible(false);
    syd_peak->setVisible(true);
    syd_end->setVisible(true);

    cs_selector->setEnabled(false);
    syd_peak->line_series->clear();
    syd_end->line_series->clear();
    float y = INT_MIN;
    for(int id = 1; id < hfile->sections.size()+1; id++){
        Crosssection* cs = hfile->sections[id];
        float peak = cs->getSyd(hfile->getApproxPeak());
        float end = cs->getSyd(hfile->getApproxEnd());
        syd_peak->line_series->append(stof(cs->getName()),peak);
        syd_end->line_series->append(stof(cs->getName()),end);
        y = std::max(y, std::max(peak, end));
    }
    axis_x->setRange(stof(hfile->sections[1]->getName()), stof(hfile->sections[hfile->sections.size()]->getName()));
    axis_y->setRange(0, y);
}

void SYDWindow::sydToCs(){
    cs_selector->setEnabled(true);

    output_init->setVisible(true);
    output_peak->setVisible(true);
    output_end->setVisible(true);
    output_ws->setVisible(true);
    syd_peak->setVisible(false);
    syd_end->setVisible(false);
    
    axis_x->setRange(min_x, max_x);
    axis_y->setRange(min_y, max_y);
}

void SYDWindow::displayScourReceived(){
    std::string data = cs_selector->currentText().toStdString();
    int idx_left = data.find(' ')+1;
    int idx_right = data.find(',');
    int id = stoi(data.substr(idx_left, idx_right-idx_left));

    Crosssection* cs = hfile->sections[id];
    scour->line_series->clear();
    float x;
    float y;
    for(int i = 0; i < cs->getScour().size(); i++){
        x = std::get<0>(cs->getScour()[i]);
        y = std::get<1>(cs->getScour()[i]);
        min_x = std::min(x, min_x);
        max_x = std::max(x, max_x);
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
        scour->line_series->append(x,y);
    }
    axis_x->setRange(min_x, max_x);
    axis_y->setRange(min_y, max_y);
    scour->setVisible(true);
}

SYDWindow::DataSeries::DataSeries(std::string name, QChartView* chart_view, QValueAxis* x, QValueAxis* y, QBoxLayout* layout, bool isVisible, QWidget* parent){
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

void SYDWindow::DataSeries::setVisible(bool isVisible){
    line_series->setVisible(isVisible);
    check_box->setVisible(isVisible);
}