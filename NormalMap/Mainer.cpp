#include <Qt\qapplication.h>
#include <MeGlWindow.h>
#include <MeWidget.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	//MeWidget myWidget;
	//myWidget.show();
	
	MeGlWindow meWindow;
	meWindow.show();

	return app.exec();
}