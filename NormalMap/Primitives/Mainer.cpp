#include <Qt\qapplication.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qpushbutton.h>
#include "MeGlWindow.h"
#include "DebugGuiManager.h"

float meFloat = 0;
float meOtherFloat = 0;
float meOtherFloat2 = 0;

void main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QWidget myWidget;
	QVBoxLayout* mainLayout;
	QVBoxLayout* debugLayout;
	myWidget.setLayout(mainLayout = new QVBoxLayout());
	mainLayout->addLayout(debugLayout = new QVBoxLayout());

	//debugGuiManager.initialize(debugLayout);
	//debugGuiManager.slide("This be me float", meFloat, 0, 10, "Lighting");
	//debugGuiManager.slide("This be me float", meOtherFloat, 0, 10, "Positioning");
	//debugGuiManager.slide("This be yet another me float", meOtherFloat2, 0, 10, "Positioning");
	//debugGuiManager.watch("Ooooooh scary", meFloat, "Lighting");


	MeGlWindow* meh;
	mainLayout->addWidget(meh = new MeGlWindow());
	meh->setMinimumSize(500, 500);
	myWidget.show();

	//MeOpenGl widg;
	//widg.show();
	app.exec();
}