#!/bin/sh
QT_VERSION=0.3.0
export QT_VERSION
QT_VER=0.3
export QT_VER
QT_VERSION_TAG=030
export QT_VERSION_TAG
QT_INSTALL_DOCS=/opt/Qt5.9.2/Docs/Qt-5.9.2
export QT_INSTALL_DOCS
BUILDDIR=/home/local/NPO/e.nikishin/Sources/QtXlsxWriter-master/src/xlsx
export BUILDDIR
exec /opt/Qt5.9.2/5.9.2/gcc_64/bin/qdoc "$@"
