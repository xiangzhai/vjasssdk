#ifndef VJASSDOC_GUI_WIDGET_HPP
#define VJASSDOC_GUI_WIDGET_HPP

#include <QObject>
#include <QWidget>

#include <kdialog.h>

#include "ui_widget.h"

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
