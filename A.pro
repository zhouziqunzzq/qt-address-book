#-------------------------------------------------
#
# Project created by QtCreator 2016-12-20T08:48:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = A
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp \
    emailgroups.cpp \
    telephonegroups.cpp \
    persongroups.cpp \
    email.cpp \
    date.cpp \
    address.cpp \
    telephone.cpp \
    persons.cpp \
    settings.cpp \
    iohelper.cpp \
    tinyxml2.cpp \
    stringhelper.cpp \
    passwordvalidatedialog.cpp \
    personinfodialog.cpp \
    telephonegroupsdialog.cpp \
    emailgroupsdialog.cpp \
    telephoneinfodialog.cpp \
    emailinfodialog.cpp \
    telephonegroupinfo.cpp \
    emailgroupinfo.cpp

HEADERS  += mainwindow.h \
    list.h \
    listNode.h \
    elements.h \
    group.h \
    persons.h \
    person.h \
    persongroup.h \
    telephonegroup.h \
    emailgroup.h \
    emailgroups.h \
    telephonegroups.h \
    persongroups.h \
    email.h \
    date.h \
    address.h \
    telephone.h \
    settings.h \
    iohelper.h \
    tinyxml2.h \
    stringhelper.h \
    passwordvalidatedialog.h \
    personinfodialog.h \
    telephonegroupsdialog.h \
    emailgroupsdialog.h \
    telephoneinfodialog.h \
    emailinfodialog.h \
    telephonegroupinfo.h \
    emailgroupinfo.h

FORMS    += mainwindow.ui \
    passwordvalidatedialog.ui \
    personinfodialog.ui \
    telephonegroupsdialog.ui \
    emailgroupsdialog.ui \
    telephoneinfodialog.ui \
    emailinfodialog.ui \
    telephonegroupinfo.ui \
    emailgroupinfo.ui
