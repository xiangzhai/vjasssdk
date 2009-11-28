/********************************************************************************
** Form generated from reading ui file 'modeleditor.ui'
**
** Created: Thu Nov 26 19:53:33 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MODELEDITOR_H
#define UI_MODELEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>
#include "ktabwidget.h"
#include "qogre/QOgreWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ModelEditor
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QTreeWidget *treeWidget;
    QTreeWidget *treeWidget_2;
    QTreeWidget *treeWidget_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    KTabWidget *ktabwidget;
    QWidget *animations;
    QWidget *textures;
    QOgreWdiget *rendererWidget;

    void setupUi(QWidget *ModelEditor)
    {
        if (ModelEditor->objectName().isEmpty())
            ModelEditor->setObjectName(QString::fromUtf8("ModelEditor"));
        ModelEditor->resize(805, 921);
        gridLayout = new QGridLayout(ModelEditor);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(ModelEditor);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        treeWidget = new QTreeWidget(groupBox);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        gridLayout_2->addWidget(treeWidget, 0, 0, 1, 1);

        treeWidget_2 = new QTreeWidget(groupBox);
        treeWidget_2->setObjectName(QString::fromUtf8("treeWidget_2"));

        gridLayout_2->addWidget(treeWidget_2, 1, 0, 1, 1);

        treeWidget_3 = new QTreeWidget(groupBox);
        treeWidget_3->setObjectName(QString::fromUtf8("treeWidget_3"));

        gridLayout_2->addWidget(treeWidget_3, 2, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 3, 2);

        groupBox_2 = new QGroupBox(ModelEditor);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        ktabwidget = new KTabWidget(groupBox_2);
        ktabwidget->setObjectName(QString::fromUtf8("ktabwidget"));
        ktabwidget->setMovable(true);
        ktabwidget->setTabReorderingEnabled(false);
        animations = new QWidget();
        animations->setObjectName(QString::fromUtf8("animations"));
        ktabwidget->addTab(animations, QString());
        textures = new QWidget();
        textures->setObjectName(QString::fromUtf8("textures"));
        ktabwidget->addTab(textures, QString());

        gridLayout_3->addWidget(ktabwidget, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 3, 3, 1);

        rendererWidget = new QOgreWdiget(ModelEditor);
        rendererWidget->setObjectName(QString::fromUtf8("rendererWidget"));
        rendererWidget->setMinimumSize(QSize(400, 800));

        gridLayout->addWidget(rendererWidget, 0, 2, 3, 1);


        retranslateUi(ModelEditor);

        ktabwidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ModelEditor);
    } // setupUi

    void retranslateUi(QWidget *ModelEditor)
    {
        ModelEditor->setWindowTitle(QApplication::translate("ModelEditor", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ModelEditor", "Models", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("ModelEditor", "Default/global", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget_2->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("ModelEditor", "Campaign/imported", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget_3->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("ModelEditor", "Map/imported", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ModelEditor", "Properties", 0, QApplication::UnicodeUTF8));
        ktabwidget->setTabText(ktabwidget->indexOf(animations), QApplication::translate("ModelEditor", "Animations", 0, QApplication::UnicodeUTF8));
        ktabwidget->setTabText(ktabwidget->indexOf(textures), QApplication::translate("ModelEditor", "Textures", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ModelEditor);
    } // retranslateUi

};

namespace Ui {
    class ModelEditor: public Ui_ModelEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELEDITOR_H
