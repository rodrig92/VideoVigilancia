/********************************************************************************
** Form generated from reading UI file 'servidor.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVIDOR_H
#define UI_SERVIDOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Servidor
{
public:
    QWidget *centralWidget;
    QPushButton *Escuchar;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Servidor)
    {
        if (Servidor->objectName().isEmpty())
            Servidor->setObjectName(QStringLiteral("Servidor"));
        Servidor->resize(400, 300);
        centralWidget = new QWidget(Servidor);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Escuchar = new QPushButton(centralWidget);
        Escuchar->setObjectName(QStringLiteral("Escuchar"));
        Escuchar->setGeometry(QRect(70, 210, 80, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 40, 311, 141));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setScaledContents(true);
        Servidor->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Servidor);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 20));
        Servidor->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Servidor);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Servidor->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Servidor);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Servidor->setStatusBar(statusBar);

        retranslateUi(Servidor);

        QMetaObject::connectSlotsByName(Servidor);
    } // setupUi

    void retranslateUi(QMainWindow *Servidor)
    {
        Servidor->setWindowTitle(QApplication::translate("Servidor", "Servidor", 0));
        Escuchar->setText(QApplication::translate("Servidor", "Escuchar", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Servidor: public Ui_Servidor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVIDOR_H
