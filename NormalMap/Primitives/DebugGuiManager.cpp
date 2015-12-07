#include "DebugGuiManager.h"
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <glm\glm.hpp>
#include <qtgui\qlabel>
#include <QtGui\qpushbutton.h>
#include <QtGui\qtabwidget.h>
#include "DebugSlider.h"

DebugGuiManager DebugGuiManager::instance;

bool DebugGuiManager::initialize(QVBoxLayout* debugLayout)
{
	this->debugLayout = debugLayout;
	debugLayout->addWidget(meTabs = new QTabWidget);
	numWatches = 0;
	numSlides = 0;
	numToggles = 0;
	numButtonInfos = 0;
	numTextInfos = 0;
	return true;
}

DebugGuiManager& DebugGuiManager::getInstance()
{
	return instance;
}

void DebugGuiManager::addWatch(
	const char* text,
	const float* value,
	DebugGuiManager::WatchType watchType,
	const char* tabName)
{
	QVBoxLayout* meLayout = getLayoutOfTab(tabName);
	assert(numWatches + 1 < MAX_INFOS);
	WatchInfo& thisInfo = watchInfos[numWatches++];
	thisInfo.watchType = watchType;
	QHBoxLayout* thisRow;
	meLayout->addLayout(thisRow = new QHBoxLayout);
	thisRow->addWidget(new QLabel(text));

	if (watchType == WT_FLOAT)
	{
		thisRow->addWidget(thisInfo.xLabel = new QLabel);
		thisRow->addStretch();
	}
	else
	{
		thisRow->addWidget(new QLabel("<"));
		thisRow->addWidget(thisInfo.xLabel = new QLabel);
		thisRow->addWidget(new QLabel(","));
		thisRow->addWidget(thisInfo.yLabel = new QLabel);
		thisRow->addWidget(new QLabel(","));
		thisRow->addWidget(thisInfo.zLabel = new QLabel);
		if (watchType == WT_VECTOR4)
		{
			thisRow->addWidget(new QLabel(","));
			thisRow->addWidget(thisInfo.wLabel = new QLabel);
		}
		thisRow->addWidget(new QLabel(">"));
	}
	thisInfo.vector = value;
}

void DebugGuiManager::watch(const char* text, const float& value, const char* tabName)
{
	addWatch(text, &value, WT_FLOAT, tabName);
}

void DebugGuiManager::watch(const char* text, const glm::vec3& value, const char* tabName)
{
	addWatch(text, reinterpret_cast<const float*>(&value), WT_VECTOR3, tabName);
}

void DebugGuiManager::watch(const char* text, const glm::vec4& value, const char* tabName)
{
	addWatch(text, reinterpret_cast<const float*>(&value), WT_VECTOR4, tabName);
}

void DebugGuiManager::addSlide(
	const char* text, float* value,
	DebugGuiManager::WatchType watchType,
	float minSliderValue, float maxSliderValue,
	const char* tabName)
{
	assert(numSlides + 1 < MAX_INFOS);
	SlideInfo& thisInfo = sliderInfos[numSlides++];
	thisInfo.watchType = watchType;

	QVBoxLayout* meLayout = getLayoutOfTab(tabName);

	QHBoxLayout* thisRow;
	meLayout->addLayout(thisRow = new QHBoxLayout);
	thisRow->addWidget(new QLabel(text));
	if (watchType == WT_FLOAT)
	{
		thisRow->addWidget(thisInfo.xSlider = new DebugSlider(minSliderValue, maxSliderValue, true));
		thisInfo.xSlider->setValue(*value);
	}
	else
	{
		thisRow->addWidget(new QLabel("<"));
		thisRow->addWidget(thisInfo.xSlider = new DebugSlider(minSliderValue, maxSliderValue));
		thisInfo.xSlider->setValue(value[0]);
		thisRow->addWidget(new QLabel(","));
		thisRow->addWidget(thisInfo.ySlider = new DebugSlider(minSliderValue, maxSliderValue));
		thisInfo.ySlider->setValue(value[1]);
		thisRow->addWidget(new QLabel(","));
		thisRow->addWidget(thisInfo.zSlider = new DebugSlider(minSliderValue, maxSliderValue));
		thisInfo.zSlider->setValue(value[2]);
		if (watchType == WT_VECTOR4)
		{
			thisRow->addWidget(new QLabel(","));
			thisRow->addWidget(thisInfo.wSlider = new DebugSlider(minSliderValue, maxSliderValue));
			thisInfo.zSlider->setValue(value[3]);
		}
		thisRow->addWidget(new QLabel(">"));
	}
	thisInfo.vector = value;
}

void DebugGuiManager::slide(
	const char* text, float& value,
	float minSliderValue, float maxSliderValue, const char* tabName)
{
	addSlide(text, &value, WT_FLOAT, minSliderValue, maxSliderValue, tabName);
}

void DebugGuiManager::slide(
	const char* text, glm::vec3& value,
	float minSliderValue, float maxSliderValue, const char* tabName)
{
	addSlide(text, reinterpret_cast<float*>(&value), WT_VECTOR3, minSliderValue, maxSliderValue, tabName);
}

void DebugGuiManager::slide(
	const char* text, glm::vec4& value,
	float minSliderValue, float maxSliderValue, const char* tabName)
{
	addSlide(text, reinterpret_cast<float*>(&value), WT_VECTOR4, minSliderValue, maxSliderValue, tabName);
}

void DebugGuiManager::toggle(const char* text, bool& value, const char* tabName)
{
	assert(numToggles + 1 < MAX_INFOS);
	QVBoxLayout* meLayout = getLayoutOfTab(tabName);

	ToggleInfo& togInfo = toggleInfos[numToggles++];
	togInfo.value = &value;

	QHBoxLayout* thisRow;
	meLayout->addLayout(thisRow = new QHBoxLayout);
	thisRow->addWidget(togInfo.checkBox = new QCheckBox(text));
	togInfo.checkBox->setChecked(value);
}

void DebugGuiManager::update()
{
	updateWatches();
	updateSliders();
	updateToggles();
	updateTexts();
}

void DebugGuiManager::updateWatches()
{
	for (uint i = 0; i < numWatches; i++)
	{
		WatchInfo& w = watchInfos[i];
		w.xLabel->setText(QString::number(w.vector[0]));
		if (w.watchType != WT_FLOAT)
		{
			w.yLabel->setText(QString::number(w.vector[1]));
			w.zLabel->setText(QString::number(w.vector[2]));
			if (w.watchType == WT_VECTOR4)
				w.wLabel->setText(QString::number(w.vector[3]));
		}
	}
}

void DebugGuiManager::updateSliders()
{
	// Just squint and pretend you can't see how long this function is.
	for (uint i = 0; i < numSlides; i++)
	{
		SlideInfo& w = sliderInfos[i];
		bool programmerUpdatedValueLastFrame;
		if (w.watchType == WT_FLOAT)
		{
			programmerUpdatedValueLastFrame = w.vector[0] != w.lastFrameValue.x;
			if (programmerUpdatedValueLastFrame)
				w.xSlider->setValue(w.vector[0]);
			else
				w.vector[0] = w.xSlider->value();
			w.lastFrameValue.x = w.vector[0];
		}
		else if (w.watchType == WT_VECTOR3)
		{
			programmerUpdatedValueLastFrame =
				w.vector[0] != w.lastFrameValue.x ||
				w.vector[1] != w.lastFrameValue.y ||
				w.vector[2] != w.lastFrameValue.z;
			if (programmerUpdatedValueLastFrame)
			{
				w.xSlider->setValue(w.vector[0]);
				w.ySlider->setValue(w.vector[1]);
				w.zSlider->setValue(w.vector[2]);
			}
			else
			{
				w.vector[0] = w.xSlider->value();
				w.vector[1] = w.ySlider->value();
				w.vector[2] = w.zSlider->value();
			}
			w.lastFrameValue.x = w.vector[0];
			w.lastFrameValue.y = w.vector[1];
			w.lastFrameValue.z = w.vector[2];
		}
		else if (w.watchType == WT_VECTOR4)
		{
			programmerUpdatedValueLastFrame =
				w.vector[0] != w.lastFrameValue.x ||
				w.vector[1] != w.lastFrameValue.y ||
				w.vector[2] != w.lastFrameValue.z ||
				w.vector[3] != w.lastFrameValue.w;
			if (programmerUpdatedValueLastFrame)
			{
				w.xSlider->setValue(w.vector[0]);
				w.ySlider->setValue(w.vector[1]);
				w.zSlider->setValue(w.vector[2]);
				w.wSlider->setValue(w.vector[3]);
			}
			else
			{
				w.vector[0] = w.xSlider->value();
				w.vector[1] = w.ySlider->value();
				w.vector[2] = w.zSlider->value();
				w.vector[3] = w.wSlider->value();
			}
			w.lastFrameValue.x = w.vector[0];
			w.lastFrameValue.y = w.vector[1];
			w.lastFrameValue.z = w.vector[2];
			w.lastFrameValue.w = w.vector[3];
		}
	}
}

void DebugGuiManager::updateToggles()
{
	for (uint i = 0; i < numToggles; i++)
	{
		ToggleInfo& t = toggleInfos[i];
		bool& v = *(t.value);
		bool programmerUpdatedValueLastFrame = v != t.lastFrameValue;
		if (programmerUpdatedValueLastFrame)
			t.checkBox->setChecked(v);
		else
			v = t.checkBox->isChecked();
		t.lastFrameValue = v;
	}
}

void DebugGuiManager::updateTexts()
{
	for (uint i = 0; i < numTextInfos; i++)
	{
		TextInfo& t = textInfos[i];
		t.theLabel->setText(*t.text);
	}
}

void DebugGuiManager::button(const char* text, fastdelegate::FastDelegate0<> callback, const char* tabName)
{
	assert(numButtonInfos + 1 < MAX_INFOS);
	QVBoxLayout* meLayout = getLayoutOfTab(tabName);
	ButtonInfo& buttonInfo = buttonInfos[numButtonInfos++];
	QHBoxLayout* thisRow;
	meLayout->addLayout(thisRow = new QHBoxLayout);
	thisRow->addWidget(buttonInfo.button = new QPushButton(text));
	QObject::connect(buttonInfo.button, SIGNAL(clicked()), &buttonInfo, SLOT(buttonClicked()));
	buttonInfo.delegate = callback;
	thisRow->addStretch();
}

void DebugGuiManager::text(const char* text, const char*& value, const char* tabName)
{
	assert(numTextInfos + 1 < MAX_INFOS);
	TextInfo& t = textInfos[numTextInfos++];
	t.text = &value;
	QVBoxLayout* meLayout = getLayoutOfTab(tabName);
	QHBoxLayout* thisRow;
	meLayout->addLayout(thisRow = new QHBoxLayout);
	thisRow->addWidget(new QLabel(text));
	thisRow->addWidget(new QLabel(": "));
	thisRow->addWidget(t.theLabel = new QLabel(*t.text));
	thisRow->addStretch();
}

QVBoxLayout* DebugGuiManager::getLayoutOfTab(const char* tabName)
{
	for (uint i = 0; i < meTabs->count(); i++)
		if (meTabs->tabText(i) == tabName)
			return static_cast<QVBoxLayout*>(meTabs->widget(i)->layout());
	// Must create new tab:
	QWidget* widg;
	meTabs->addTab(widg = new QWidget, tabName);
	QVBoxLayout* layout;
	widg->setLayout(layout = new QVBoxLayout);
	return layout;
}

void ButtonInfo::buttonClicked()
{
	delegate();
}
