#include <QHBoxLayout>
#include <QKeyEvent>
#include "mainwindow.h"

MainWindow::MainWindow() :
    QWidget()
{
    _glWidget = new GLWidget();

    _defaultLayout = new QHBoxLayout;
    _defaultLayout->addWidget(_glWidget);


    _frequencySlider = createSlider();
    _frequencySlider->hide();
    QObject::connect(_frequencySlider, SIGNAL(valueChanged(int)), _glWidget, SLOT(setFrequency(int)));
    _waveVectorSlider = createSlider();
    _waveVectorSlider->hide();
    QObject::connect(_waveVectorSlider, SIGNAL(valueChanged(int)), _glWidget, SLOT(setWaveVector(int)));

    _defaultLayout->addWidget(_frequencySlider);
    _defaultLayout->addWidget(_waveVectorSlider);
    setLayout(_defaultLayout);
}

QSlider * MainWindow::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 500);
    slider->setSingleStep(1);
    slider->setPageStep(50);
    slider->setTickInterval(50);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_F1 || event->key() == Qt::Key_F3){
        _frequencySlider->hide();
        _waveVectorSlider->hide();
    } else if(event->key() == Qt::Key_F4){
        _frequencySlider->show();
        _waveVectorSlider->show();
    }
    _glWidget->onKeyPress(event);
}
