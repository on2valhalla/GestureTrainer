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



#include "mainwindow.h"
#include "ui_mainwindow.h"


//##############################################################################
//	Constructors / Destructor

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	// setup and display form
	ui->setupUi(this);


	//set up video ------------------
	//get camera
	cap.open(CAMERA);
	// check if we succeeded, if not do not enable camera toggle
    if(!cap.isOpened())
		ui->pushButton_Camera->setEnabled(false);
	//set up timer for camera display
    timer = new QTimer(this);
	//end setup video ---------------


	//default settings
	process = false;
	histEnable = false;
	cHist = ColorHistogram();

	//Hardcoded default HSV filter for home environment
	min = cv::Scalar(0, 40, 93);
	max = cv::Scalar(23, 255, 255);
	setSliders();


	// connect slot action methods ----------
	// select color
	connect(timer, SIGNAL(timeout()), 
				this, SLOT(updateTimer()));
	connect(ui->pushButton_OpenImage, SIGNAL(clicked()), 
				this, SLOT(setImage()));
	connect(ui->pushButton_Camera, SIGNAL(clicked()),
				this, SLOT(toggleCamera()));
	connect(ui->pushButton_Process, SIGNAL(clicked()),
				this, SLOT(processColorDetection()));
	connect(ui->pushButton_Histogram, SIGNAL(clicked()),
				this, SLOT(showHistogram()));
	connect(ui->verticalSlider_MinHue, SIGNAL(valueChanged(int)), 
				this, SLOT(setMinHue(int)));
	connect(ui->verticalSlider_MinSat, SIGNAL(valueChanged(int)), 
				this, SLOT(setMinSat(int)));
	connect(ui->verticalSlider_MinValue, SIGNAL(valueChanged(int)), 
				this, SLOT(setMinValue(int)));
	connect(ui->verticalSlider_MaxHue, SIGNAL(valueChanged(int)), 
				this, SLOT(setMaxHue(int)));
	connect(ui->verticalSlider_MaxSat, SIGNAL(valueChanged(int)), 
				this, SLOT(setMaxSat(int)));
	connect(ui->verticalSlider_MaxValue, SIGNAL(valueChanged(int)), 
				this, SLOT(setMaxValue(int)));
	//end slots------------------------------

}

MainWindow::~MainWindow()
{
	delete ui;
	delete timer;
}

//	END Constructors / Destructor
//##############################################################################



//##############################################################################
//	Utility Functions

/*
	A simple conversion method to display a cv::Mat image (BGR or binary)
	in a QLabel, that only takes a QImage as a pixelmap.
*/
void MainWindow::displayMat(const cv::Mat& image)
{
	//BGR openCV Mat to QImage
	QImage img_qt = QImage((const unsigned char*)image.data,image.cols,
							image.rows, image.step, QImage::Format_RGB888);

	//For Binary Images
	if (img_qt.isNull())
	{
		//ColorTable for Binary Images
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++)
			colorTable.push_back(qRgb(i, i, i));

		img_qt = QImage((const unsigned char*)image.data,image.cols, 
							image.rows, image.step, QImage::Format_Indexed8);
		img_qt.setColorTable(colorTable);
	}

	//Display the QImage in the Label
	QPixmap img_pix = QPixmap::fromImage(img_qt.rgbSwapped()); //BGR to RGB
	this->ui->label_Camera->setPixmap(
						img_pix.scaled(ui->label_Camera->size(), Qt::KeepAspectRatio));
}

//	END Utility Functions
//##############################################################################



//##############################################################################
//	UI Functions

/*
	Set the sliders to specific positions (Scalars used for compatibility
	with BackgroundWidget)
*/
void MainWindow::setSliders()
{
	ui->verticalSlider_MinHue->setSliderPosition(min[0]);
	ui->verticalSlider_MinSat->setSliderPosition(min[1]);
	ui->verticalSlider_MinValue->setSliderPosition(min[2]);
	ui->verticalSlider_MaxHue->setSliderPosition(max[0]);
	ui->verticalSlider_MaxSat->setSliderPosition(max[1]);
	ui->verticalSlider_MaxValue->setSliderPosition(max[2]);
}

/*
	Sets the skin detector's masks min and max from
	the cached values in BackgroundWidget
*/
void MainWindow::setThreshold()
{
	SkinDetectController::getInstance()->setThreshold(min, max);
	if(!timer->isActive() && process)
		processColorDetection();
}

// END UI Functions
//##############################################################################


//##############################################################################
//	Slots

/*
	Change in tabs, so adjust display related variables
	to show the correct image
*/
void MainWindow::on_tabWidget_currentChanged(int index)
{
	switch(index)
	{
		case START_TAB:
			process = false;
			histEnable = false;
			break;
		case BACKGROUND_TAB:
			break;
		case MEASURE_TAB:
			process = false;
			histEnable = false;
			break;
		default:
			break;
	}
}

/*
	This method is called by a button on the form
	and in turn pulls up an open file dialog, where
	the user can select a photo for thresholding
*/
void MainWindow::setImage()
{
	timer->stop();
	QFileDialog::Options options;
	QString selectedFilter;
	QString fileName = QFileDialog::getOpenFileName(this,
								tr("Open Image Files"),
								"",
								tr("Image files (*.jpg *.jpeg *.png *.gif *.bmp *.ppm)"),
								&selectedFilter,
								options);
	if (!fileName.isEmpty()){
		cv::Mat image = cv::imread(fileName.toStdString(),1); //0 for grayscale
		displayMat(image);
		//Send Filename to the skin detector
		SkinDetectController::getInstance()->setInputImage(fileName.toStdString());
	}
}

/*
	Called by the Camera button, toggles the video feed
	on and off by toggling the timer. It will only attempt to
	turn on the timer if the cv::VideoCapture is opened
*/
void MainWindow::toggleCamera()
{
	if(!cap.isOpened())
		return;
	if(timer->isActive())
	{
		timer->stop();
		ui->pushButton_Camera->setText("Show Camera");
	}
	else
	{
		timer->start(TIMER_DELAY);
		ui->pushButton_Camera->setText("Hide Camera");
	}

}




/*
	Controls the display of the video, on the interval set in
	toggleCamera above. It also regulates the display of the 
	histogram, and processing of the video if they are enabled.
*/
void MainWindow::updateTimer()
{
	cv::Mat img;
	cap >> img; //capture a frame

	//send SkinDetector the frame
	SkinDetectController::getInstance()->setInputImage(img);
	if(histEnable)
	{   // update the histogram
		histogram = cHist.getHistogramImage(img);
		cv::imshow("Histogram", histogram);
	}
	if(process)
	{   //process the frame
		SkinDetectController::getInstance()->process();
		
		//retrieve the processed frame
		cv::Mat result = SkinDetectController::getInstance()->getLastResult();
		if (!result.empty())
			img = result;
	}
	displayMat(img);
}




/*
	Called by the process button, this method toggles the processing 
	and the display of the processed images/video. This method calls
	the process function from the SkinDetector class
*/
void MainWindow::processColorDetection()
{
	if(timer->isActive())
		process = !process;
	else
	{
		SkinDetectController::getInstance()->process();
		cv::Mat resulting = 
						SkinDetectController::getInstance()->getLastResult();
		if (!resulting.empty())
			displayMat(resulting);
	}
}

/*
	Toggles the histogram window on and off
*/
void MainWindow::showHistogram()
{
	if(timer->isActive() && !histEnable)
	{   //create histogram window for video display
		cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
		histEnable = true;
	}
	else if (!timer->isActive() && !histEnable )
	{   //create histogram for image display
		histogram = cHist.getHistogramImage(
							SkinDetectController::getInstance()->getHSVImage());
		cv::imshow("Histogram", histogram);
		histEnable = true;
	}
	else
	{   //destroy histogram window
		cv::destroyWindow("Histogram");
		histEnable = false;
	}
}




/*
	Changes minimum Hue from slider
*/
void MainWindow::setMinHue(int value)
{
	min[0] = value;
	this->ui->label_MinHue->setText(QString::number(value));
	setThreshold();
}

/*
	Changes minimum saturation from slider
*/
void MainWindow::setMinSat(int value)
{
	min[1] = value;
	this->ui->label_MinSat->setText(QString::number(value));
	setThreshold();
}

/*
	Changes minimum value from slider
*/
void MainWindow::setMinValue(int value)
{
	min[2] = value;
	this->ui->label_MinValue->setText(QString::number(value));
	setThreshold();
}

/*
	Changes maximum hue from slider
*/
void MainWindow::setMaxHue(int value)
{
	max[0] = value;
	this->ui->label_MaxHue->setText(QString::number(value));
	setThreshold();
}

/*
	Changes maximum saturation from slider
*/
void MainWindow::setMaxSat(int value)
{
	max[1] = value;
	this->ui->label_MaxSat->setText(QString::number(value));
	setThreshold();
}

/*
	Changes maximum value from slider
*/
void MainWindow::setMaxValue(int value)
{
	max[2] = value;
	this->ui->label_MaxValue->setText(QString::number(value));
	setThreshold();
}


// END Slots
//##############################################################################
