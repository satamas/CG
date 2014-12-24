#include <QtWidgets/qcheckbox.h>
#include "MainWidget.h"

MainWidget::MainWidget() :
        _glWidget(new GLWidget()),
        layout(new QHBoxLayout()),
        formLayout(new QFormLayout()),
        spinBox(new QDoubleSpinBox()),
        checkBox(new QCheckBox()) {

    spinBox->setValue(1);
    QObject::connect(spinBox, SIGNAL(valueChanged(double)), _glWidget, SLOT(setTextureFactor(double)));
    spinBox->setSingleStep(0.1);
    spinBox->setMinimum(0);
    formLayout->addRow("Texture factor: ", spinBox);

    formLayout->addRow("Filter textures ", checkBox);
    QObject::connect(checkBox, SIGNAL(stateChanged(int)), _glWidget, SLOT(setTextureFiltration(int)));

    layout->addWidget(_glWidget);
    layout->addLayout(formLayout);
    setLayout(layout);
}

MainWidget::~MainWidget() {
    delete spinBox;
    spinBox = 0;
    delete layout;
    layout = 0;
}