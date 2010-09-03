/********************************************************************************
** Form generated from reading UI file 'modeleditor.ui'
**
** Created: Fri Sep 3 10:10:49 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELEDITOR_H
#define UI_MODELEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "klineedit.h"
#include "ktabwidget.h"

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
    QWidget *m_modelViewWidget;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    KTabWidget *ktabwidget;
    QWidget *modelTab;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    KLineEdit *klineedit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_2;
    QWidget *animationsTab;
    QWidget *texturesTab;

    void setupUi(QWidget *ModelEditor)
    {
        if (ModelEditor->objectName().isEmpty())
            ModelEditor->setObjectName(QString::fromUtf8("ModelEditor"));
        ModelEditor->resize(955, 1070);
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

        m_modelViewWidget = new QWidget(ModelEditor);
        m_modelViewWidget->setObjectName(QString::fromUtf8("m_modelViewWidget"));
        m_modelViewWidget->setMinimumSize(QSize(400, 800));

        gridLayout->addWidget(m_modelViewWidget, 0, 2, 3, 1);

        groupBox_2 = new QGroupBox(ModelEditor);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        ktabwidget = new KTabWidget(groupBox_2);
        ktabwidget->setObjectName(QString::fromUtf8("ktabwidget"));
        ktabwidget->setMovable(true);
        ktabwidget->setTabReorderingEnabled(false);
        modelTab = new QWidget();
        modelTab->setObjectName(QString::fromUtf8("modelTab"));
        verticalLayoutWidget = new QWidget(modelTab);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 20, 181, 151));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        klineedit = new KLineEdit(verticalLayoutWidget);
        klineedit->setObjectName(QString::fromUtf8("klineedit"));
        klineedit->setMaxLength(336);

        horizontalLayout->addWidget(klineedit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        doubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));

        horizontalLayout_2->addWidget(doubleSpinBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        doubleSpinBox_2 = new QDoubleSpinBox(verticalLayoutWidget);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));

        horizontalLayout_3->addWidget(doubleSpinBox_2);


        verticalLayout->addLayout(horizontalLayout_3);

        ktabwidget->addTab(modelTab, QString());
        animationsTab = new QWidget();
        animationsTab->setObjectName(QString::fromUtf8("animationsTab"));
        ktabwidget->addTab(animationsTab, QString());
        texturesTab = new QWidget();
        texturesTab->setObjectName(QString::fromUtf8("texturesTab"));
        ktabwidget->addTab(texturesTab, QString());

        gridLayout_3->addWidget(ktabwidget, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 3, 3, 1);


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
        label->setText(QApplication::translate("ModelEditor", "Name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ModelEditor", "Unknown:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ModelEditor", "Blend time:", 0, QApplication::UnicodeUTF8));
        ktabwidget->setTabText(ktabwidget->indexOf(modelTab), QApplication::translate("ModelEditor", "Model", 0, QApplication::UnicodeUTF8));
        ktabwidget->setTabText(ktabwidget->indexOf(animationsTab), QApplication::translate("ModelEditor", "Animations", 0, QApplication::UnicodeUTF8));
        ktabwidget->setTabText(ktabwidget->indexOf(texturesTab), QApplication::translate("ModelEditor", "Textures", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ModelEditor: public Ui_ModelEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELEDITOR_H
