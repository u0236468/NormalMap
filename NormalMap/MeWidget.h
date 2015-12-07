#pragma once
#include <QtGui\qwidget.h>
#include <MyModel.h>
class DebugSlider;
class MeGlWindow;

class MeWidget : public QWidget
{
	Q_OBJECT

	DebugSlider* lightSliderX;
	DebugSlider* lightSliderY;
	DebugSlider* lightSliderZ;
	//DebugSlider* lightSlider;

	MeGlWindow* meglwindow;

	MyModel theModel;

private slots:
	void sliderValueChanged();

public:
	MeWidget();
};