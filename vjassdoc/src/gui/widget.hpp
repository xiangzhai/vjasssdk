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

	public slots:
		void run();

	public:
		Widget(class MainWindow *parent);
		~Widget();
};

}

}

#endif
