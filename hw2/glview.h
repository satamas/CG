#ifndef GLVIEW_H
#define GLVIEW_H

#include <QWidget>

class GLView : public QWidget
{
    Q_OBJECT

public:
    GLView(QWidget *parent = 0);
    ~GLView();
};

#endif // GLVIEW_H
