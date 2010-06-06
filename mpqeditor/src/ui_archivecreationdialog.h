/********************************************************************************
** Form generated from reading ui file 'archivecreationdialog.ui'
**
** Created: Fri Jul 10 18:34:13 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ARCHIVECREATIONDIALOG_H
#define UI_ARCHIVECREATIONDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kdialog.h"
#include "knuminput.h"
#include "kurlrequester.h"

QT_BEGIN_NAMESPACE

class Ui_ArchiveCreationDialog
{
public:
    QWidget *gridLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *m_archivePathLabel;
    KUrlRequester *m_archivePathUrlRequester;
    QLabel *m_directoryPathLabel;
    KUrlRequester *m_directoryPathUrlRequester;
    QLabel *m_hashTableSizeLabel;
    KIntSpinBox *m_hashTableSizeSpinBox;
    QCheckBox *m_attributesCheckBox;
    QCheckBox *m_version2CheckBox;

    void setupUi(KDialog *ArchiveCreationDialog)
    {
        if (ArchiveCreationDialog->objectName().isEmpty())
            ArchiveCreationDialog->setObjectName(QString::fromUtf8("ArchiveCreationDialog"));
        ArchiveCreationDialog->resize(380, 231);
        ArchiveCreationDialog->setLocale(QLocale(QLocale::C, QLocale::AnyCountry));
        ArchiveCreationDialog->setSizeGripEnabled(false);
        ArchiveCreationDialog->setModal(true);
        gridLayoutWidget = new QWidget(ArchiveCreationDialog);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 0, 149, 208));
        verticalLayout = new QVBoxLayout(gridLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        m_archivePathLabel = new QLabel(gridLayoutWidget);
        m_archivePathLabel->setObjectName(QString::fromUtf8("m_archivePathLabel"));

        verticalLayout->addWidget(m_archivePathLabel);

        m_archivePathUrlRequester = new KUrlRequester(gridLayoutWidget);
        m_archivePathUrlRequester->setObjectName(QString::fromUtf8("m_archivePathUrlRequester"));

        verticalLayout->addWidget(m_archivePathUrlRequester);

        m_directoryPathLabel = new QLabel(gridLayoutWidget);
        m_directoryPathLabel->setObjectName(QString::fromUtf8("m_directoryPathLabel"));

        verticalLayout->addWidget(m_directoryPathLabel);

        m_directoryPathUrlRequester = new KUrlRequester(gridLayoutWidget);
        m_directoryPathUrlRequester->setObjectName(QString::fromUtf8("m_directoryPathUrlRequester"));

        verticalLayout->addWidget(m_directoryPathUrlRequester);

        m_hashTableSizeLabel = new QLabel(gridLayoutWidget);
        m_hashTableSizeLabel->setObjectName(QString::fromUtf8("m_hashTableSizeLabel"));

        verticalLayout->addWidget(m_hashTableSizeLabel);

        m_hashTableSizeSpinBox = new KIntSpinBox(gridLayoutWidget);
        m_hashTableSizeSpinBox->setObjectName(QString::fromUtf8("m_hashTableSizeSpinBox"));
        m_hashTableSizeSpinBox->setMinimum(16);
        m_hashTableSizeSpinBox->setMaximum(262144);
        m_hashTableSizeSpinBox->setBase(10);

        verticalLayout->addWidget(m_hashTableSizeSpinBox);

        m_attributesCheckBox = new QCheckBox(gridLayoutWidget);
        m_attributesCheckBox->setObjectName(QString::fromUtf8("m_attributesCheckBox"));

        verticalLayout->addWidget(m_attributesCheckBox);

        m_version2CheckBox = new QCheckBox(gridLayoutWidget);
        m_version2CheckBox->setObjectName(QString::fromUtf8("m_version2CheckBox"));

        verticalLayout->addWidget(m_version2CheckBox);


        retranslateUi(ArchiveCreationDialog);

        QMetaObject::connectSlotsByName(ArchiveCreationDialog);
    } // setupUi

    void retranslateUi(KDialog *ArchiveCreationDialog)
    {
        ArchiveCreationDialog->setWindowTitle(QApplication::translate("ArchiveCreationDialog", "Create MPQ Archive", 0, QApplication::UnicodeUTF8));
        m_archivePathLabel->setText(QApplication::translate("ArchiveCreationDialog", "Archive Path:", 0, QApplication::UnicodeUTF8));
        m_archivePathUrlRequester->setFilter(QApplication::translate("ArchiveCreationDialog", "*.mpq, *.MPQ|MPQ archives\\n*.w3m;;*.W3M|Warcraft 3 Maps", 0, QApplication::UnicodeUTF8));
        m_directoryPathLabel->setText(QApplication::translate("ArchiveCreationDialog", "Directory Path:", 0, QApplication::UnicodeUTF8));
        m_directoryPathUrlRequester->setFilter(QString());
        m_hashTableSizeLabel->setText(QApplication::translate("ArchiveCreationDialog", "Hash Table Size:", 0, QApplication::UnicodeUTF8));
        m_attributesCheckBox->setText(QApplication::translate("ArchiveCreationDialog", "(attributes)", 0, QApplication::UnicodeUTF8));
        m_version2CheckBox->setText(QApplication::translate("ArchiveCreationDialog", "Format Version 2.0", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ArchiveCreationDialog);
    } // retranslateUi

};

namespace Ui {
    class ArchiveCreationDialog: public Ui_ArchiveCreationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARCHIVECREATIONDIALOG_H
