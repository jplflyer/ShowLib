CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17
CONFIG += sdk_no_version_chec

INCLUDEPATH += include/ /usr/local/include

SOURCES += \
    showlib/FileUtilities.cpp \
    showlib/JSONSerializable.cpp \
    showlib/OptionHandler.cpp \
    showlib/RESTServer.cpp \
    showlib/RequestHandlerFactory.cpp \
    showlib/Router.cpp \
    showlib/StringUtils.cpp \
    showlib/StringVector.cpp \
    src/BCrypt-Password.cpp \
    tests/TestJSON.cpp \
    tests/TestStrings.cpp \
    tests/Whatever.cpp \
    tests/main-test.cpp

HEADERS += \
    showlib/Base64.h \
    showlib/FileUtilities.h \
    showlib/JSONSerializable.h \
    showlib/OptionHandler.h \
    showlib/RESTServer.h \
    showlib/Ranges.h \
    showlib/RequestHandlerFactory.h \
    showlib/Router.h \
    showlib/StringUtils.h \
    showlib/StringVector.h \
    tests/TestJSON.h \
    tests/TestStrings.h \
    tests/UnitTesting.h \
    tests/main-test.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
