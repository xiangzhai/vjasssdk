/********************************************************************************
** Form generated from reading ui file 'triggereditor.ui'
**
** Created: Thu Nov 26 19:50:41 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRIGGEREDITOR_H
#define UI_TRIGGEREDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>
#include "ktextbrowser.h"

QT_BEGIN_NAMESPACE

class Ui_TriggerEditor
{
public:
    QTreeWidget *treeWidget;
    KTextBrowser *ktextbrowser;
    QLabel *label;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QWidget *widget;

    void setupUi(QWidget *TriggerEditor)
    {
        if (TriggerEditor->objectName().isEmpty())
            TriggerEditor->setObjectName(QString::fromUtf8("TriggerEditor"));
        TriggerEditor->resize(468, 300);
        treeWidget = new QTreeWidget(TriggerEditor);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(10, 20, 121, 251));
        ktextbrowser = new KTextBrowser(TriggerEditor);
        ktextbrowser->setObjectName(QString::fromUtf8("ktextbrowser"));
        ktextbrowser->setGeometry(QRect(140, 50, 256, 121));
        label = new QLabel(TriggerEditor);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 20, 141, 18));
        checkBox = new QCheckBox(TriggerEditor);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(150, 180, 191, 23));
        checkBox_2 = new QCheckBox(TriggerEditor);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(350, 180, 98, 23));
        widget = new QWidget(TriggerEditor);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(150, 210, 251, 51));

        retranslateUi(TriggerEditor);

        QMetaObject::connectSlotsByName(TriggerEditor);
    } // setupUi

    void retranslateUi(QWidget *TriggerEditor)
    {
        TriggerEditor->setWindowTitle(QApplication::translate("TriggerEditor", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("TriggerEditor", "Triggers", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("TriggerEditor", "Map script", 0, QApplication::UnicodeUTF8));
        treeWidget->setSortingEnabled(__sortingEnabled);

        label->setText(QApplication::translate("TriggerEditor", "Trigger comment", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("TriggerEditor", "Run on map initialization", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("TriggerEditor", "Enabled", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TriggerEditor);
    } // retranslateUi

};

namespace Ui {
    class TriggerEditor: public Ui_TriggerEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIGGEREDITOR_H
