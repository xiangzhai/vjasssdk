/********************************************************************************
** Form generated from reading ui file 'newmapdialog.ui'
**
** Created: Thu Nov 26 19:51:11 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_NEWMAPDIALOG_H
#define UI_NEWMAPDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kcombobox.h"
#include "kdialog.h"

QT_BEGIN_NAMESPACE

class Ui_NewMapDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    KComboBox *kcombobox;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    KComboBox *kcombobox_2;

    void setupUi(KDialog *NewMapDialog)
    {
        if (NewMapDialog->objectName().isEmpty())
            NewMapDialog->setObjectName(QString::fromUtf8("NewMapDialog"));
        NewMapDialog->resize(367, 334);
        NewMapDialog->setLocale(QLocale(QLocale::C, QLocale::AnyCountry));
        NewMapDialog->setModal(true);
        verticalLayoutWidget = new QWidget(NewMapDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 9, 351, 101));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);

        horizontalLayout_2->addWidget(label_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        kcombobox = new KComboBox(verticalLayoutWidget);
        kcombobox->setObjectName(QString::fromUtf8("kcombobox"));

        horizontalLayout->addWidget(kcombobox);

        horizontalSpacer = new QSpacerItem(60, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        kcombobox_2 = new KComboBox(verticalLayoutWidget);
        kcombobox_2->setObjectName(QString::fromUtf8("kcombobox_2"));

        horizontalLayout->addWidget(kcombobox_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(NewMapDialog);

        QMetaObject::connectSlotsByName(NewMapDialog);
    } // setupUi

    void retranslateUi(KDialog *NewMapDialog)
    {
        NewMapDialog->setWindowTitle(QApplication::translate("NewMapDialog", "Form", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("NewMapDialog", "Map size:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("NewMapDialog", "Width:", 0, QApplication::UnicodeUTF8));
        kcombobox->clear();
        kcombobox->insertItems(0, QStringList()
         << QApplication::translate("NewMapDialog", "32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "64", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "96", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "128", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "160", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "192", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "224", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "256", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "288", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "320", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "352", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "384", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "416", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "448", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "480", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("NewMapDialog", "Height:", 0, QApplication::UnicodeUTF8));
        kcombobox_2->clear();
        kcombobox_2->insertItems(0, QStringList()
         << QApplication::translate("NewMapDialog", "32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "64", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "96", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "128", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "160", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "192", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "224", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "256", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "288", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "320", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "352", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "384", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "416", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "448", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NewMapDialog", "480", 0, QApplication::UnicodeUTF8)
        );
        Q_UNUSED(NewMapDialog);
    } // retranslateUi

};

namespace Ui {
    class NewMapDialog: public Ui_NewMapDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWMAPDIALOG_H
