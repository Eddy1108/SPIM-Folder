#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scenes/Scene0.h"
#include "Scenes/Scene1.h"

#include "Core/shader.h"
#include "mainwindow.h"
#include "logger.h"

#include <AL/al.h>

RenderWindow::RenderWindow(const QSurfaceFormat& format, MainWindow* mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }


    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{

    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    if (!mInitialized)
        mInitialized = true;

    initializeOpenGLFunctions();

    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
        std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
        std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);


    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    glDepthMask(GL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

    //Set up Shaders
    mShaderPrograms.insert(std::pair<std::string, Shader*>("plain", new PlainShader("../SPIM-Folder/Shaders/plainshader.vert", "../SPIM-Folder/Shaders/plainshader.frag")));
    mShaderPrograms.insert(std::pair<std::string, Shader*>("textured", new TextureShader("../SPIM-Folder/Shaders/textureshader.vert", "../SPIM-Folder/Shaders/textureshader.frag")));
    mShaderPrograms.insert(std::pair<std::string, Shader*>("phong", new PhongShader("../SPIM-Folder/Shaders/phongshader.vert", "../SPIM-Folder/Shaders/phongshader.frag")));
    mShaderPrograms.insert(std::pair<std::string, Shader*>("skybox", new SkyBoxShader("../SPIM-Folder/Shaders/skyboxShader.vert", "../SPIM-Folder/Shaders/skyboxShader.frag")));

    //Set up Scene(s)
    Scenes.push_back(new Scene0(mShaderPrograms));


    ///Initialize all the scenes
    for (int i = 0; i < Scenes.size(); i++)
    {
        Scenes[i]->init();
    }

    glBindVertexArray(0);       //unbinds any VertexArray - good practice


    ///How to use textures;
    //mTexture[0] = new Texture();
    //mTexture[1] = new Texture("../3Dprog22/Assets/tex/hund.bmp");
    ////Set the textures loaded to a texture slot
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());
}


// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    ///Used in Deta time calculation
    auto start = std::chrono::system_clock::now();
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCurrentCamera = Scenes[activeScene]->mCamera;

    ///Draw active Scene
    Scenes[activeScene]->draw();
    

     /// Other Things
    calculateFramerate();
    checkForGLerrors();
    mContext->swapBuffers(this);

    ///Calculates Delta time
    auto end = std::chrono::system_clock::now();
    auto ElapsedSeconds = end - start;
    mDeltaTime = std::chrono::duration<double>(ElapsedSeconds).count();
    //std::cout << "DELTATIME:" << mDeltaTime << std::endl;
}

void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop == the render()-function
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//Debug method
void RenderWindow::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
{
    static uint32_t VAO{};
    static uint32_t VBO{};
    glm::vec3 data[] = { start, end };
    if (VAO == 0)
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
    glUseProgram(mShaderPrograms[0]->getProgram());
    auto location = glGetUniformLocation(mShaderPrograms["plain"]->getProgram(), "colorIn");
    glUniform4f(location, color.x, color.y, color.z, color.w);

    static constexpr glm::mat4 identity(1.f);

    location = glGetUniformLocation(mShaderPrograms["plain"]->getProgram(), "model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(identity));

    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}


void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

void RenderWindow::mousePressEvent(QMouseEvent* event)
{
    if(Scenes[activeScene])
        Scenes[activeScene]->mousePressEvent(event);

}

void RenderWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if(Scenes[activeScene])
        Scenes[activeScene]->mouseReleaseEvent(event);

}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(Scenes[activeScene])
        Scenes[activeScene]->mouseMoveEvent(event);
}


void RenderWindow::keyPressEvent(QKeyEvent* event)
{
    //Exit application
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }

    if(Scenes[activeScene])
        Scenes[activeScene]->keyPressEvent(event);
}

void RenderWindow::keyReleaseEvent(QKeyEvent* event)
{
    if(Scenes[activeScene])
        Scenes[activeScene]->keyReleaseEvent(event);
}

void RenderWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(Scenes[activeScene])
        Scenes[activeScene]->mouseDoubleClickEvent(event);
}
