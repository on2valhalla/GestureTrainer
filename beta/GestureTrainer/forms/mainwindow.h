/*----------------------------------------------------------------------------*\
	Created by: 
			Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)
			Lindsey Heller (lwh2115@barnard.edu)

	Some inspiration for design and organization from Robert Laganiere's
	OpenCV2 Cookbook.

	This class is a QT based form, for the purposes of training a user to a 
	specific gesture set. The form contains an image display area that can be
	used to display an image or webcamera. 

	The first tab (background) contains sliders so that you can actively select 
	a mask to create a binary skin image from a standard RGB. You select a mask 
	by moving the sliders (in HSV values) for the min and max thresholds sent 
	to the SkinDetector class for processing

\*----------------------------------------------------------------------------*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT Includes
#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Local Includes
#include "../ext/skindetectcontroller.h"	//singleton processes skin regions
#include "../ext/colorhistogram.h"		//for displaying a 3 color histogram


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();



	
protected:
	// Utilities
	void displayMat(const cv::Mat& img);

	// UI Functions
	void setSliders();
	void setThreshold();



private:
	Ui::MainWindow *ui;

	// camera vars
	cv::VideoCapture cap;

	// timer vars
	QTimer *timer;
	bool process;
	bool histEnable;

	// thresholding masks
	cv::Scalar min, max;

	// histogram vars
	cv::Mat histogram;
	ColorHistogram cHist;



	// CONSTANTS
	const static char 
	// Tab indicies
		START_TAB = 0,
		BACKGROUND_TAB = 1,
		MEASURE_TAB = 2,
	// Camera ( 0 = sys default / 1 = iGlasses )
		CAMERA = 1,
	// Timer delay in ms
		TIMER_DELAY = 25;


	
private slots:
	// Form Slots
	void on_tabWidget_currentChanged(int index);
	void setImage();
	void toggleCamera();
	void updateTimer();

	//Background Slots
	void processColorDetection();
	void showHistogram();
	void setMinHue(int value);
	void setMinSat(int value);
	void setMinValue(int value);
	void setMaxHue(int value);
	void setMaxSat(int value);
	void setMaxValue(int value);

};

#endif // MAINWINDOW_H
