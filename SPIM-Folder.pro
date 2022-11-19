QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering


SOURCES += main.cpp \
    #Core/stb_image.cpp \
    Core/materiallist.cpp \
    Core/material.cpp \
    Core/Audio.cpp \
    Core/Camera.cpp \
    Core/CollisionShapes.cpp \
    Core/Vertex.cpp \
    Core/VisualObject.cpp \
    Core/XYZ.cpp \
    Core/dr_wav.cpp \
    Core/inputmanager.cpp \
    Core/quadtree.cpp \
    Core/shader.cpp \
    Core/texture.cpp \
    Light/Light.cpp \
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
    Mesh/line.cpp \
    Scenes/Scene.cpp \
    Scenes/Scene0.cpp \
    Scenes/Scene1.cpp \
    Materials/materialcubemap.cpp \
    Materials/materialphong.cpp \
    Materials/materialplain.cpp \
    Materials/materialtexture.cpp \
    Materials/materialbillboard.cpp \
    Scenes/SceneSwitcher.cpp \
    Terrain/terrainbasechunk.cpp \
    logger.cpp \
    Lua_files/luafunctiontest.cpp \
    mainwindow.cpp \
    Terrain/proceduralterrain.cpp \
    renderwindow.cpp

HEADERS += \
    #Core/stb_image.h \
    Core/materiallist.h \
    Core/material.h \
    Core/Audio.h \
    Core/Camera.h \
    Core/CollisionShapes.h \
    Core/Vertex.h \
    Core/VisualObject.h \
    Core/XYZ.h \
    Core/inputmanager.h \
    Core/quadtree.h \
    Core/shader.h \
    Core/texture.h \
    Light/Light.h \
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
    Mesh/line.h \
    Scenes/Scene.h \
    Scenes/Scene0.h \
    Scenes/Scene1.h \
    Scenes/SceneSwitcher.h \
    Materials/materialcubemap.h \
    Materials/materialphong.h \
    Materials/materialplain.h \
    Materials/materialtexture.h \
    Materials/materialbillboard.h \
    Terrain/terrainbasechunk.h \
    library_includes/dr_lib/dr_wav.h \
    logger.h \
    Lua_files/luafunctiontest.h \
    mainwindow.h \
    Terrain/proceduralterrain.h \
    renderwindow.h \
# external Lua files
    library_includes/Lua/lua.hpp \
    library_includes/Lua/lua.h \
    library_includes/Lua/luaconf.h \
    library_includes/Lua/lualib.h \
    library_includes/Lua/lauxlib.h \
    Lua_files/funksjoner.h \
    Lua_files/test0.lua \
    Lua_files/test1.out \
    Lua_files/test1.lua \
    Lua_files/test2.lua \
    Lua_files/test3.lua \
    Lua_files/test4.lua \
    #Noise
    library_includes/Noise/FastNoiseLite.h \
    #image loader
    stb_image/stb_image.h \
    stb_image/stb_image.cpp\
    glm/glm.hpp

FORMS += \
    mainwindow.ui

DISTFILES += \
    #images
    Assets/Texture/heightmap.bmp \
    Assets/Texture/hund.bmp \
    Assets/Texture/linus.bmp \
    #skybox
    Assets/Texture/skybox/right.jpg \
    Assets/Texture/skybox/left.jpg \
    Assets/Texture/skybox/top.jpg \
    Assets/Texture/skybox/bottom.jpg \
    Assets/Texture/skybox/front.jpg \
    Assets/Texture/skybox/back.jpg \
    #shaders
    Shaders/phongshader.frag \
    Shaders/phongshader.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/plainshaderChad.frag \
    Shaders/plainshaderChad.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert \
    Shaders/skyboxshader.frag \
    Shaders/skyboxshader.vert

#INCLUDEPATH += $(LUA_HOME)\\include
#LIBS *= $(LUA_HOME)\\libs\\lua5.4.3-static.lib

win32: LIBS += -L$$PWD/build_libraries/debug/OpenAL/ -lOpenAL32

INCLUDEPATH += $$PWD/build_libraries/debug/OpenAL
DEPENDPATH += $$PWD/build_libraries/debug/OpenAL

INCLUDEPATH += $$PWD/library_includes

#Lua
win32: LIBS += -L$$PWD/library_includes/Lua/ -llua5.4.3-static

INCLUDEPATH += $$PWD/library_includes/Lua
DEPENDPATH += $$PWD/library_includes/Lua

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/library_includes/Lua/lua5.4.3-static.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/library_includes/Lua/liblua5.4.3-static.a

#Luac
win32: LIBS += -L$$PWD/library_includes/Lua/ -lluac

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/library_includes/Lua/luac.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/library_includes/Lua/libluac.a
