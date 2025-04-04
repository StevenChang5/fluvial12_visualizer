#ifndef SYD_H
#define SYD_H

#include <QWidget>
#include <QPushButton>
#include <QFormLayout>

class SYDWindow : public QWidget{
    Q_OBJECT
    public:
        explicit SYDWindow(QWidget *parent = 0);
    private:
        int m_counter;
        QPushButton *m_button;
        QString fileName;
        QPushButton* getFileButton;
        QFormLayout *formGroupLayout;
    signals:
        void counterReached();
    private slots:
        void slotButtonClicked(bool checked);
        void getFileButtonClicked();
};

#endif