TARGET = Gammaray
TEMPLATE = lib

# DEFINES += 

QT += network

# Gammaray files

SOURCES += \
    gammarayplugin.cpp \
    gammaraytool.cpp \
    gammarayengine.cpp \
    gammaraysettings.cpp \
    gammarayconfigwidget.cpp

HEADERS += \
    gammarayplugin.h \
    gammaraytool.h \
    gammarayengine.h \
    gammarayconstants.h \
    gammaraysettings.h \
    gammarayconfigwidget.h

FORMS += \
    gammarayconfigwidget.ui \
    gammarayversion.ui

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/d/dev/qt-creator/src

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/d/dev/qt-creator/build-Debug

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\Nokia\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/Nokia/qtcreator" or "~/.local/share/Nokia/qtcreator" on Linux
##    "~/Library/Application Support/Nokia/Qt Creator" on Mac
USE_USER_DESTDIR = yes

PROVIDER = KDAB

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/analyzerbase/analyzerbase.pri)
include($$QTCREATOR_SOURCES/src/plugins/coreplugin/coreplugin.pri)

LIBS += -L$$IDE_PLUGIN_PATH/Nokia
