#ifndef VJASSDOC_GUI_WIDGET_H
#define VJASSDOC_GUI_WIDGET_H

#include <QObject>
#include <QWidget>

#include <kdialog.h>

#include "ui/ui_widget.hpp"

namespace vjassdoc
{

namespace gui
{

class MainWindow;

class Widget : public QWidget, Ui::Widget
{
	Q_OBJECT

	public:
		Widget(class MainWindow *parent);
		void run();

	private slots:
		void buttonClicked(KDialog::ButtonCode button);
};

}

}

#endif
