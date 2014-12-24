#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qformlayout.h>
#include "GLWidget.h"

class QBasicTimer;
class QGLShaderProgram;

class MainWidget : public QWidget
{
Q_OBJECT
public:
    explicit MainWidget();
    virtual ~MainWidget();

protected:
    void keyPressEvent(QKeyEvent * e){
        _glWidget->onKeyPress(e);
    }
private:
    GLWidget * _glWidget;
    QBoxLayout * layout;
    QCheckBox * checkBox;
    QFormLayout * formLayout;
    QDoubleSpinBox * spinBox;
};

#endif // MAINWIDGET_H