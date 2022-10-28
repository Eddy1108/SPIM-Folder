#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include "qevent.h"

class InputManager : protected QOpenGLFunctions_4_1_Core
{
public:
    InputManager();

    //Key Press Event
    virtual void mousePressEvent(QMouseEvent* event){};
    virtual void mouseReleaseEvent(QMouseEvent* event){};
    virtual void mouseMoveEvent(QMouseEvent* event){};
    virtual void keyPressEvent(QKeyEvent* event){};
    virtual void mouseDoubleClickEvent(QMouseEvent* event){};
    virtual void keyReleaseEvent(QKeyEvent* event){};
};

#endif // INPUTMANAGER_H
