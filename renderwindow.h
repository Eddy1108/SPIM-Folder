#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include "Core/texture.h"

#include "Scenes/SceneSwitcher.h"
#include "Scenes/Scene.h"
#include "Scenes/Scene0.h"
#include "Scenes/Scene1.h"
#include "Core/Camera.h"

#include "Light/Light.h"
#include "mesh/Kube.h"

#include "Shaders/PlainShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/PhongShader.h"
#include "Shaders/SkyBoxShader.h"
#include "Shaders/BillboardShader.h"

#include <chrono>

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }
    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    std::vector<Scene*> Scenes;
    int activeScene{ 0 };

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    inline static glm::vec3 triangle[3];

    inline static float mDeltaTime{ 0 };

private slots:
    void render();          //the actual render - function

private:
    void init();            //initialize things we need before rendering

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    //Shader Storage
    std::unordered_map<std::string, Shader*> mShaderPrograms;

    Camera* mCurrentCamera{ nullptr };

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QMatrix4x4* mPmatrix{ nullptr };
    QMatrix4x4* mVmatrix{ nullptr };

    glm::mat4 mPmatrix1{ 1.0f };
    glm::mat4 mVMatrix{ 1.0f };
    glm::mat4 mMVPmatrix{1.0f};

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application


    void DrawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color);


    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

protected:

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
