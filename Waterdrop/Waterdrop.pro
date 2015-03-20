SOURCES += \
    main.cpp \
    mesh.cpp \
    window.cpp

INCLUDEPATH += \
    ../VCG/vcglib

HEADERS += \
    mesh.h \
    drop.h \
    window.h

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target
