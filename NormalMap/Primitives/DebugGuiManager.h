#ifndef DEBUG_GUI_MANAGER_H
#define DEBUG_GUI_MANAGER_H
#include <glm\glm.hpp>
#include <QtGui\qlabel.h>
#include <QtGui\qcheckbox.h>
#include "FastDelegate.h"
#include <Qt\qlist.h>
class QPushButton;
class QVBoxLayout;
class QTabWidget;
class DebugSlider;
#define DEFAULT_TAB_NAME "General"

struct ButtonInfo : public QObject
{
	Q_OBJECT
public:
	QPushButton* button;
	fastdelegate::FastDelegate0<> delegate;
	private slots:
	void buttonClicked();
};

class DebugGuiManager
{
	enum WatchType
	{
		WT_FLOAT,
		WT_VECTOR3,
		WT_VECTOR4
	};
	QVBoxLayout* debugLayout;
	QTabWidget* meTabs;
	uint numWatches;
	uint numSlides;
	uint numToggles;
	uint numButtonInfos;
	uint numTextInfos;
	struct WatchInfo
	{
		const float* vector;
		QLabel *xLabel;
		QLabel *yLabel;
		QLabel *zLabel;
		QLabel *wLabel;
		WatchType watchType;
		WatchInfo() : watchType(WT_VECTOR3) {}
	};
	struct SlideInfo
	{
		float* vector;
		DebugSlider* xSlider;
		DebugSlider* ySlider;
		DebugSlider* zSlider;
		DebugSlider* wSlider;
		glm::vec4 lastFrameValue;
		WatchType watchType;
		SlideInfo() : watchType(WT_VECTOR3) {}
	};
	struct ToggleInfo
	{
		bool* value;
		QCheckBox* checkBox;
		bool lastFrameValue;
	};
	struct TextInfo
	{
		const char** text;
		QLabel* theLabel;
	};
	static const uint MAX_INFOS = 50;
	WatchInfo watchInfos[MAX_INFOS];
	SlideInfo sliderInfos[MAX_INFOS];
	ToggleInfo toggleInfos[MAX_INFOS];
	TextInfo textInfos[MAX_INFOS];
private:

	ButtonInfo buttonInfos[MAX_INFOS];
	void addWatch(
		const char* text,
		const float* value,
		WatchType watchType,
		const char* tabName = DEFAULT_TAB_NAME);
	void addSlide(
		const char* text,
		float* value,
		WatchType watchType,
		float minSliderValue,
		float maxSliderValue,
		const char* tabName = DEFAULT_TAB_NAME);

	static DebugGuiManager instance;
	DebugGuiManager() {}
	DebugGuiManager(const DebugGuiManager&);
	DebugGuiManager& operator=(const DebugGuiManager& original);
	QVBoxLayout* getLayoutOfTab(const char* tabName);
	void updateWatches();
	void updateSliders();
	void updateToggles();
	void updateTexts();
public:
	bool initialize(QVBoxLayout* debugLayout);
	bool shutdown() { return true; }
	static DebugGuiManager& getInstance();
	void watch(const char* text, const float& value, const char* tabName = DEFAULT_TAB_NAME);
	void watch(const char* text, const glm::vec3& vector, const char* tabName = DEFAULT_TAB_NAME);
	void watch(const char* text, const glm::vec4& vector, const char* tabName = DEFAULT_TAB_NAME);
	void slide(const char* text, float& value, float minSliderValue = -15.0f, float maxSliderValue = 15.0f, const char* tabName = DEFAULT_TAB_NAME);
	void slide(const char* text, glm::vec3& value, float minSliderValue = -15.0f, float maxSliderValue = 15.0f, const char* tabName = DEFAULT_TAB_NAME);
	void slide(const char* text, glm::vec4& value, float minSliderValue = -15.0f, float maxSliderValue = 15.0f, const char* tabName = DEFAULT_TAB_NAME);
	void toggle(const char* text, bool& value, const char* tabName = DEFAULT_TAB_NAME);
	void button(const char* text, fastdelegate::FastDelegate0<> callback, const char* tabName = DEFAULT_TAB_NAME);
	void text(const char* text, const char*& value, const char* tabName = DEFAULT_TAB_NAME);
	void update();
};

#define debugGuiManager DebugGuiManager::getInstance()

#endif