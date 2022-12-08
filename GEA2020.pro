QT          += core gui widgets qml

TEMPLATE    = app
CONFIG      += c++17

TARGET      = GameEngineArchitecture

INCLUDEPATH += .
INCLUDEPATH += ./include

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    Math/vector.cpp \
    Objects/ball.cpp \
    Objects/cube.cpp \
    Objects/heightmapterrain.cpp \
    Shaders/particleshader.cpp \
    Shaders/phongshader.cpp \
    Shaders/plainshader.cpp \
    Shaders/shader.cpp \
    Shaders/skyboxshader.cpp \
    Shaders/textureshader.cpp \
    boxcolliderwidget.cpp \
    camera.cpp \
    collisions.cpp \
    collisionwidget.cpp \
    entity.cpp \
    frustumclass.cpp \
    matrix3x3.cpp \
    Math/matrix4x4.cpp \
    Shaders/mousepickershader.cpp \
    movementsystem.cpp \
    particlesystem.cpp \
    rendersystem.cpp \
    renderwindow.cpp \
    mainwindow.cpp \
    Objects/triangle.cpp \
    Math/vector2d.cpp \
    Math/vector3d.cpp \
    scene.cpp \
    scriptingsystem.cpp \
    skybox.cpp \
    spherecolliderwidget.cpp \
    vertex.cpp \
    visualobject.cpp \
    Objects/xyz.cpp \
    Objects/grid_area.cpp \
    interactiveobject.cpp \
    light.cpp \
    texture.cpp \
    objmesh.cpp \
    Math/vector4d.cpp \
    wavfilehandler.cpp \
    resourcemanager.cpp \
    widgettransform.cpp
	

HEADERS += \
    Math/vector.h \
    Objects/ball.h \
    Objects/cube.h \
    Objects/heightmapterrain.h \
    Shaders/particleshader.h \
    Shaders/phongshader.h \
    Shaders/plainshader.h \
    Shaders/shader.h \
    Shaders/skyboxshader.h \
    Shaders/textureshader.h \
    boxcolliderwidget.h \
    camera.h \
    collisions.h \
    collisionwidget.h \
    components.h \
    constants.h \
    entity.h \
    frustumclass.h \
    gltypes.h \
    include/AL/al.h \
    include/AL/alc.h \
    include/AL/alext.h \
    include/AL/efx-creative.h \
    include/AL/efx-presets.h \
    include/AL/efx.h \
    Shaders/mousepickershader.h \
    movementsystem.h \
    particlesystem.h \
    scene.h \
    scriptingsystem.h \
    skybox.h \
    spherecolliderwidget.h \
    wavfilehandler.h \
    input.h \
    Math/math_constants.h \
    matrix3x3.h \
    Math/matrix4x4.h \
    rendersystem.h \
    renderwindow.h \
    mainwindow.h \
    Objects/triangle.h \
    Math/vector2d.h \
    Math/vector3d.h \
    vertex.h \
    visualobject.h \
    Objects/xyz.h \
    Objects/grid_area.h \
    interactiveobject.h \
    light.h \
    math_constants.h \
    texture.h \
    objmesh.h \
    Math/vector4d.h \
    resourcemanager.h \
    widgettransform.h

FORMS += \
    boxcolliderwidget.ui \
    collisionwidget.ui \
    mainwindow.ui \
    spherecolliderwidget.ui \
    widgettransform.ui

DISTFILES += \
    Shaders/mousepickershader.frag \
    Shaders/mousepickershader.vert \
    Shaders/particleshader.frag \
    Shaders/particleshader.vert \
    Shaders/phongshader.frag \
    Shaders/phongshader.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/skyboxshader.frag \
    Shaders/skyboxshader.vert \
    scriptfile.js \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert
