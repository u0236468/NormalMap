#include "MeWidget.h"
#include <QtGui\qvboxlayout>
#include <Qt\qdebug.h>
#include <QtGui\qhboxlayout>
#include <MeGlWindow.h>
#include <Primitives\DebugSlider.h>


//Sliders
MeWidget::MeWidget()
{
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);
	QVBoxLayout* controlsLayout;
	mainLayout->addLayout(controlsLayout = new QVBoxLayout);
//	mainLayout->addWidget(meglwindow = new MeGlWindow(&theModel));

	QHBoxLayout* lightpositionLayout;
	controlsLayout->addLayout(lightpositionLayout = new QHBoxLayout);
	lightpositionLayout->addWidget(lightSliderX = new DebugSlider);
	lightpositionLayout->addWidget(lightSliderY = new DebugSlider);
	lightpositionLayout->addWidget(lightSliderZ = new DebugSlider);

	connect(lightSliderX, SIGNAL(valueChanged(float)), this, SLOT(sliderValueChanged()));
	connect(lightSliderY, SIGNAL(valueChanged(float)), this, SLOT(sliderValueChanged()));
	connect(lightSliderZ, SIGNAL(valueChanged(float)), this, SLOT(sliderValueChanged()));
}

void MeWidget::sliderValueChanged()
{
	theModel.lightPosition.x = lightSliderX->value();
	theModel.lightPosition.y = lightSliderY->value();
	theModel.lightPosition.z = lightSliderZ->value();
	meglwindow->repaint();
	//qDebug() << "Value Changed";
}