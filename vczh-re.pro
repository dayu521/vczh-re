######################################################################
# Automatically generated by qmake (3.1) Wed Aug 26 21:27:29 2020
######################################################################

TEMPLATE = app
TARGET = vczh-re
INCLUDEPATH += .
CONFIG += c++2a

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

msvc {
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
}

# Input
HEADERS += CharClass.h \
           Dfa.h \
           DfaMatrix.h \
           EpsilonNfa.h \
           GrammarTree.h \
           Link.h \
           LytWString.h \
           Matrix.h \
           Nfa.h \
           OrderedLink.h \
           Regex.h \
           Set.h \
           SomeDataType.h \
           StatusEdge.h \
           Tree.h
SOURCES += CharClass.cpp \
           Dfa.cpp \
           DfaMatrix.cpp \
           EpsilonNfa.cpp \
           GrammarTree.cpp \
           LytWString.cpp \
           main.cpp \
           Nfa.cpp \
           StatusEdge.cpp
