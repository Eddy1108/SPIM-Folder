QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    Core/Camera.cpp \
    Core/CollisionShapes.cpp \
    Core/Vertex.cpp \
    Core/VisualObject.cpp \
    Core/XYZ.cpp \
    Core/quadtree.cpp \
    Core/shader.cpp \
    Core/texture.cpp \
    Light/Light.cpp \
    Light/Sun.cpp \
    Math/Barycentric.cpp \
    Math/BezierCurve.cpp \
    Math/SortCoords.cpp \
    Mesh/Billboard.cpp \
    Mesh/Disc.cpp \
    Mesh/Equidistance.cpp \
    Mesh/Heightmap.cpp \
    Mesh/Kube.cpp \
    Mesh/LAZSurface.cpp \
    Mesh/ObjLoader.cpp \
    Mesh/OctahedronBall.cpp \
    Mesh/Player/InteractiveObject.cpp \
    Mesh/Player/Player.cpp \
    Mesh/PointCloud.cpp \
    Mesh/SkyBox.cpp \
    Mesh/Triangle.cpp \
    Mesh/TriangleSurface.cpp \
    Mesh/ballspawner.cpp \
    Mesh/line.cpp \
    Mesh/rollingball.cpp \
    Scenes/Scene.cpp \
    Scenes/Scene0.cpp \
    Scenes/Scene1.cpp \
    Scenes/SceneSwitcher.cpp \
    Shaders/PhongShader.cpp \
    Shaders/PlainShader.cpp \
    Shaders/SkyBoxShader.cpp \
    Shaders/TextureShader.cpp \
    logger.cpp \
    mainwindow.cpp \
    renderwindow.cpp

HEADERS += \
    Core/Camera.h \
    Core/CollisionShapes.h \
    Core/Vertex.h \
    Core/VisualObject.h \
    Core/XYZ.h \
    Core/quadtree.h \
    Core/shader.h \
    Core/texture.h \
    Light/Light.h \
    Light/Sun.h \
    Math/Barycentric.h \
    Math/BezierCurve.h \
    Math/SortCoords.h \
    Mesh/Billboard.h \
    Mesh/Disc.h \
    Mesh/Equidistance.h \
    Mesh/Heightmap.h \
    Mesh/Kube.h \
    Mesh/LAZSurface.h \
    Mesh/ObjLoader.h \
    Mesh/OctahedronBall.h \
    Mesh/Player/InteractiveObject.h \
    Mesh/Player/Player.h \
    Mesh/PointCloud.h \
    Mesh/SkyBox.h \
    Mesh/Triangle.h \
    Mesh/TriangleSurface.h \
    Mesh/ballspawner.h \
    Mesh/line.h \
    Mesh/rollingball.h \
    Scenes/Scene.h \
    Scenes/Scene0.h \
    Scenes/Scene1.h \
    Scenes/SceneSwitcher.h \
    Shaders/PhongShader.h \
    Shaders/PlainShader.h \
    Shaders/SkyBoxShader.h \
    Shaders/TextureShader.h \
    logger.h \
    mainwindow.h \
    renderwindow.h \
# external libs
#    stb_image/stb_image.h \
    glm/glm.hpp

FORMS += \
    mainwindow.ui

DISTFILES += \
    Assets/tex/heightmap.bmp \
    Assets/tex/hund.bmp \
    Assets/tex/linus.bmp \
    Shaders/phongshader.frag \
    Shaders/phongshader.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/plainshaderChad.frag \
    Shaders/plainshaderChad.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert
