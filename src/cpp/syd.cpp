#include "syd.h"
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

    getFileButton = new QPushButton("Open File", this);
    connect(getFileButton, SIGNAL(clicked()), this, SLOT(getFileButtonClicked()));
    

    QGroupBox* formGroupBox = new QGroupBox(tr("Upload/Save Files"), this);
    QLineEdit* getFileLineEdit = new QLineEdit(this);
    getFileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    formGroupLayout = new QFormLayout(this);
    formGroupLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formGroupLayout->addRow(getFileButton, getFileLineEdit);
    formGroupLayout->addRow(new QLabel(tr("Save File:")), new QLineEdit);
    formGroupBox->setLayout(formGroupLayout);

    main_layout->addStretch();
    main_layout->addWidget(formGroupBox);

}

void SYDWindow::slotButtonClicked(bool checked){
    if(checked){
        m_button->setText("Checked");
    }else{
        m_button->setText("Hello World");
    }
    m_counter++;
    if(m_counter == 10){
        emit counterReached();
    }
}

void SYDWindow::getFileButtonClicked(){
    fileName = QFileDialog::getOpenFileName(this,"Select a file", "","Text Files (*.txt);;All Files (*)" );
    if(!fileName.isEmpty()){
        QLayoutItem* lineEdit = formGroupLayout->itemAt(0,QFormLayout::FieldRole);
        QLineEdit* edit = qobject_cast<QLineEdit*>(lineEdit->widget());
        edit->setText(fileName);
    }
}