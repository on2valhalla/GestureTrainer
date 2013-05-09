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
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>

//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Local Includes
#include "../detectors/skindetectcontroller.h"	//singleton processes skin regions
#include "../detectors/handdetectcontroller.h"	//singleton that finds hands
#include "../include/colorhistogram.h"		//for displaying a 3 color histogram
#include "../include/user.h"


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
	void displayMat(const cv::Mat img, QLabel *label);
	cv::Mat processSkin( const cv::Mat img );
    void processHand( const cv::Mat color, const cv::Mat binary );
	cv::Mat detectHand( const cv::Mat img );

	void loadDefaultHands();

	// UI Functions
	void setSliders();
	void setThreshold();



private:
	Ui::MainWindow *ui;

	// camera vars
	cv::VideoCapture cap;

	// timer vars
	QTimer *timer;
	bool backProcess, histEnable, handDetect, measureHand;

	// thresholding masks
	cv::Scalar min, max;

	// histogram vars
	cv::Mat histogram;
	ColorHistogram cHist;

	// The users data
	User user;


	// CONSTANTS
	const static char 
	// Tab indicies
		START_TAB = 0,
		BACKGROUND_TAB = 1,
		MEASURE_TAB = 2,
		DETECT_TAB = 3,
	// Camera ( 0 = sys default / 1 = iGlasses )
		CAMERA = 0,
	// Timer delay in ms
		TIMER_DELAY = 25;


	
private slots:
	// Form Slots
	void updateTimer();
	void on_tabWidget_currentChanged(int index);
	void setImage();
	void toggleCamera();
	void keyPressEvent(QKeyEvent *e);

	//Background Slots
	void processColorDetection();
	void showHistogram();
	void setMinHue(int value);
	void setMinSat(int value);
	void setMinValue(int value);
	void setMaxHue(int value);
	void setMaxSat(int value);
	void setMaxValue(int value);

	// Morphological slots
	void on_check_Invert_stateChanged(int state);
	void on_check_Erode_stateChanged(int state);
	void on_check_Dilate_stateChanged(int state);
	void on_check_Blur_stateChanged(int state);
	void on_pushButton_Detect_clicked();
};

#endif // MAINWINDOW_H
