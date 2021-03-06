CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17
CONFIG += sdk_no_version_chec
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

INCLUDEPATH += include/ /usr/local/include

SOURCES += \
    showlib/ArgumentVector.cpp \
    showlib/FileUtilities.cpp \
    showlib/JSONSerializable.cpp \
    showlib/OptionHandler.cpp \
    showlib/RESTServer.cpp \
    showlib/RequestHandlerFactory.cpp \
    showlib/Router.cpp \
    showlib/SSHConfiguration.cpp \
    showlib/StringUtils.cpp \
    showlib/StringVector.cpp \
    showlib/URI.cpp \
    src/BCrypt-Password.cpp \
    tests/RunSSHConfig.cpp \
    tests/TestJSON.cpp \
    tests/TestStrings.cpp \
    tests/TestVectorUtilities.cpp \
    tests/Whatever.cpp \
    tests/main-test.cpp

HEADERS += \
    showlib/ArgumentVector.h \
    showlib/Base64.h \
    showlib/FileUtilities.h \
    showlib/JSONSerializable.h \
    showlib/NumericOperators.h \
    showlib/OptionHandler.h \
    showlib/RESTServer.h \
    showlib/Ranges.h \
    showlib/RequestHandlerFactory.h \
    showlib/Router.h \
    showlib/SSHConfiguration.h \
    showlib/StringUtils.h \
    showlib/StringVector.h \
    showlib/URI.h \
    showlib/VectorUtilities.h \
    tests/TestJSON.h \
    tests/TestStrings.h \
    tests/TestVectorUtilities.h \
    tests/UnitTesting.h \
    tests/main-test.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
