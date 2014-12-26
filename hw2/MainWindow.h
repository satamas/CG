#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWindow>
#include <QSlider>
#include <QLayout>
#include "GLWidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow();

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent * event) override;

private:
    GLWidget * _glWidget;
    QSlider * _frequencySlider;
    QSlider * _waveVectorSlider;

    QLayout * _defaultLayout;
    QLayout * _layoutWithSLiders;

    QSlider * createSlider();
};

#endif // MAINWINDOW_H
