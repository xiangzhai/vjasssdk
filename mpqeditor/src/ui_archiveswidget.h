/********************************************************************************
** Form generated from reading ui file 'archiveswidget.ui'
**
** Created: Thu Jul 2 15:02:26 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ARCHIVESWIDGET_H
#define UI_ARCHIVESWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>
#include "klineedit.h"
#include "ktreewidgetsearchline.h"

QT_BEGIN_NAMESPACE

class Ui_ArchivesWidget
{
public:
    QGridLayout *gridLayout;
    KTreeWidgetSearchLine *m_treeWidgetSearchLine;
    QTreeWidget *m_treeWidget;

    void setupUi(QWidget *ArchivesWidget)
    {
        if (ArchivesWidget->objectName().isEmpty())
            ArchivesWidget->setObjectName(QString::fromUtf8("ArchivesWidget"));
        ArchivesWidget->resize(400, 300);
        gridLayout = new QGridLayout(ArchivesWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_treeWidgetSearchLine = new KTreeWidgetSearchLine(ArchivesWidget);
        m_treeWidgetSearchLine->setObjectName(QString::fromUtf8("m_treeWidgetSearchLine"));
        m_treeWidgetSearchLine->setEnabled(true);

        gridLayout->addWidget(m_treeWidgetSearchLine, 0, 0, 1, 1);

        m_treeWidget = new QTreeWidget(ArchivesWidget);
        m_treeWidget->setObjectName(QString::fromUtf8("m_treeWidget"));

        gridLayout->addWidget(m_treeWidget, 1, 0, 1, 1);


        retranslateUi(ArchivesWidget);

        QMetaObject::connectSlotsByName(ArchivesWidget);
    } // setupUi

    void retranslateUi(QWidget *ArchivesWidget)
    {
        ArchivesWidget->setWindowTitle(QApplication::translate("ArchivesWidget", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("ArchivesWidget", "Files", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("ArchivesWidget", "Archive", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ArchivesWidget);
    } // retranslateUi

};

namespace Ui {
    class ArchivesWidget: public Ui_ArchivesWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARCHIVESWIDGET_H
