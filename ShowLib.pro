CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++14
CONFIG += sdk_no_version_chec

INCLUDEPATH += include/ /usr/local/include

SOURCES += \
    showlib/JSONSerializable.cpp \
    tests/TestJSON.cpp \
    tests/main-test.cpp

HEADERS += \
    showlib/JSONSerializable.h \
    tests/TestJSON.h \
    tests/UnitTesting.h \
    tests/main-test.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
