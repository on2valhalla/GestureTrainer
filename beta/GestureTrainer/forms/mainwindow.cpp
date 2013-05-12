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
//  Constructors / Destructor

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	// setup and display form
	ui->setupUi(this);
	ui->tabWidget->setCurrentIndex(0);
	//set up timer for camera display
	timer = new QTimer(this);



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


	//set up video ------------------
	//get camera
	cap.open(CAMERA);
	// check if we succeeded, if not do not enable camera toggle
	if(!cap.isOpened())
		ui->pushButton_Camera->setEnabled(false);
	//end setup video ---------------


	//default settings
	backProcess = histEnable = handDetect = measureHand = false;
	cHist = ColorHistogram();

	//Environments
	QFile file(LOC_PREFS.c_str());
	bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
//    qDebug() << QDir::currentPath();
	if( ret )
	{
		QTextStream stream(&file);
		while(!stream.atEnd())
		{
			QString line = stream.readLine();
			if(line.size() < 10)
				continue;
//            qDebug() << line;
			QStringList strList = line.split("#", QString::SkipEmptyParts);
			std::vector<cv::Scalar> loc = {cv::Scalar(), cv::Scalar()};

//            qDebug() << strList;
			locationNames.push_back(strList.at(0));
			loc[0][0] = strList.at(1).toInt();
			loc[0][1] = strList.at(2).toInt();
			loc[0][2] = strList.at(3).toInt();
			loc[1][0] = strList.at(4).toInt();
			loc[1][1] = strList.at(5).toInt();
			loc[1][2] = strList.at(6).toInt();
			locations.push_back(loc);
		}
	}

//	std::vector<cv::Scalar> home = {cv::Scalar(0, 40, 93), cv::Scalar(20, 255, 255)};
//	locations.push_back(home);
//	locationNames.push_back("Home");

	//Hardcoded default HSV filter for home environment
	min = locations[0][0];
	max = locations[0][1];
	setSliders();

	for(unsigned int i = 0; i <locationNames.size(); i++)
		ui->comboBox->addItem(locationNames[i]);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete timer;
}

//  END Constructors / Destructor
//##############################################################################



//##############################################################################
//  Utility Functions

/*
	A simple conversion method to display a cv::Mat image (BGR or binary)
	in a QLabel, that only takes a QImage as a pixelmap.
*/
void MainWindow::displayMat(const cv::Mat image, QLabel *label)
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
	label->setPixmap(img_pix.scaled(label->size(), Qt::KeepAspectRatio));
}

/*
	Simplification utility method, returns the result of the skin detector
	when given a color image
*/
cv::Mat MainWindow::processSkin( const cv::Mat img )
{
	//send SkinDetector the frame
	bool set = SkinDetectController::getInstance()->setInputImage(img);
	if (!set)
		qDebug() << "Image not set!!!!!";
	//process the frame
	SkinDetectController::getInstance()->process();
	
	//retrieve the processed frame
	cv::Mat result = SkinDetectController::getInstance()->getLastResult();  
	return result;
}

/*
	Simplification utility method, stores the hand with the user
	when given a Color and BINARY image, filtered for skin
*/
cv::Mat MainWindow::processHand( const cv::Mat color, const cv::Mat binary )
{
	// send HandDetector the processed frame
	bool set = HandDetectController::getInstance()->setInputImages(color, binary);
	if (!set)
		qDebug() << "Images not set!!!!!";

	// find the hand blob and store
	HandDetectController::getInstance()->findHand();

	// display hand ROI in small window
	user.setCurHand(HandDetectController::getInstance()->getLastHand());
	return HandDetectController::getInstance()->getLastResult();
}

/*
	Utility function for detecting the hand, and displaying it in the 
	smaller ROI label, returns the edited image (color)
*/
cv::Mat MainWindow::detectHand( const cv::Mat img )
{
	// process skin
	cv::Mat binary = processSkin(img);
	cv::Mat result = processHand(img, binary);

	// display hand ROI in small window
	if(!user.curHand.isNone())
	{
		cv::Mat handROI(img, user.curHand.getBoundRect());
		if(handROI.data)
			displayMat(handROI, ui->label_HandDisplay);

		// std::cout << user.curHand.bRatio << std::endl;

//		ui->textBrowser->setText(QString("User bratios(F/S): (%1 / %2)")
//										.arg(user.fist.getB())
//                                        .arg(user.spread.getB()));

        ui->textBrowser->setText(user.getData());
        ui->textBrowser->append(user.curHand.getData());
	}

	return result;

}

cv::Mat MainWindow::trainHand( const cv::Mat img )
{
	cv::Mat result = img.clone();
	cv::Rect captureRect(img.rows/10, img.cols/10, 250, 250);

	rectangle(result, captureRect, COLOR_CAP_RECT, 3);

	cv::Mat capROI(img, captureRect);
	cv::Mat binCapROI = processSkin(capROI);
	capROI = processHand(capROI, binCapROI);
	displayMat(capROI, ui->label_Train);


	return result;
}

void MainWindow::loadDefaultHands()
{
	QString selectedFilter;
	QString fistFile = "../img/fist.jpg";
	QString spreadFile = "../img/palm.fingers.jpg";
	cv::Mat fistImg = cv::imread(fistFile.toStdString(),1); //0 for grayscale
	cv::Mat spreadImg = cv::imread(spreadFile.toStdString(),1); //0 for grayscale


	cv::Scalar localMin(0,40,93);
	cv::Scalar localMax(20,255,255);
	SkinDetectController::getInstance()->setThreshold(localMin, localMax);
	user.fist = Hand(detectHand(fistImg));

	localMin = cv::Scalar(0,40,93);
	localMax = cv::Scalar(20,255,255);
	SkinDetectController::getInstance()->setThreshold(localMin, localMax);
	user.spread = Hand(detectHand(spreadImg));
}
//  END Utility Functions
//##############################################################################



//##############################################################################
//  UI Functions

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
	if(!timer->isActive() && backProcess)
		processColorDetection();
}

// END UI Functions
//##############################################################################


//##############################################################################
//  Slots

//---------Timer--------------

/*
	Controls the display of the video, on the interval set in
	toggleCamera above. It also regulates the display of the 
	histogram, and processing of the video if they are enabled.
*/
void MainWindow::updateTimer()
{
	cv::Mat img, result;
	cap >> img; //capture a frame

	if(histEnable)
	{   // update the histogram
		histogram = cHist.getHistogramImage(img);
		cv::imshow("Histogram", histogram);
	}
	if(backProcess)
		result = processSkin(img);
	else if(measureHand)
		result = trainHand(img);
	else if(handDetect)
		result = detectHand(img);

	if (!result.empty())
		img = result;
	displayMat(img, ui->label_Camera);
}



//---------Main Form Slots--------------

/*
	Change in tabs, so adjust display related variables
	to show the correct image
*/
void MainWindow::on_tabWidget_currentChanged(int index)
{
	switch(index)
	{
		case START_TAB:
			backProcess = false;
			histEnable = false;
			handDetect = false;
			measureHand = false;
			break;
		case BACKGROUND_TAB:
			handDetect = false;
			measureHand = false;
			break;
		case MEASURE_TAB:
			backProcess = false;
			histEnable = false;
			handDetect = false;
			measureHand = true;
			user.fist = Hand();
			user.spread = Hand();
			break;
		case DETECT_TAB:
			backProcess = false;
			histEnable = false;
			measureHand = false;
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
		displayMat(image, ui->label_Camera);
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
	Handles all of the keypress events for the form,
	But mostly used for the measurement tab
*/
void MainWindow::keyPressEvent(QKeyEvent *e)
{
	qDebug() << "KeyPress" << e->key();
	if( e->key() == 32 ) // SPACE BAR
	{
		toggleCamera();
	}
	else if(e->key() == 16777220 && measureHand) // ENTER
	{
		if(user.fist.isNone())
		{
			user.fist = user.curHand;
			//Display the next image
			QPixmap img_pix(":/img/palm.fingers.jpg"); 
			ui->label_Example->setPixmap(img_pix.scaled(
						ui->label_Example->size(), Qt::KeepAspectRatio));
		}
		else if(user.spread.isNone())
		{
			user.spread = user.curHand;
			// CLear the label, done with measurement
			QPixmap img_pix(250,250); 
			img_pix.fill();
			ui->label_Example->setPixmap(img_pix);
		}

		toggleCamera();
	}
	else if(e->key() == 88 && measureHand) // x
	{
		user.fist = Hand();
		user.spread = Hand();
		//Display the first image
		QPixmap img_pix(":/img/fist.jpg"); 
		ui->label_Example->setPixmap(img_pix.scaled(
					ui->label_Example->size(), Qt::KeepAspectRatio));
	}
}




//---------Background Buttons--------------

/*
	Called by the process button, this method toggles the processing 
	and the display of the processed images/video. This method calls
	the process function from the SkinDetector class
*/
void MainWindow::processColorDetection()
{
	if(timer->isActive() || !backProcess)
		backProcess = !backProcess;
	else if(!SkinDetectController::getInstance()->getHSVImage().empty())
	{
		SkinDetectController::getInstance()->process();
		cv::Mat resulting = 
						SkinDetectController::getInstance()->getLastResult();
		displayMat(resulting, ui->label_Camera);
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

void MainWindow::on_pushButton_Save_clicked()
{
	QFile file(LOC_PREFS.c_str());
	if (!file.open(QIODevice::Append | QIODevice::Text))
		 return;

	 QTextStream out(&file);

	 bool ok;
	 QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
										   tr("Location name:"), QLineEdit::Normal,
										  "", &ok);
	 if(ok && !text.isEmpty())
		 out << text << "#";
	 else
		 out << "unknown#";

	 out << min[0] << "#" << min[1] << "#" << min[2] << "#" ;
	 out << max[0] << "#" << max[1] << "#" << max[2] << "\n" ;


	// add to list
	std::vector<cv::Scalar> loc = {min, max};
	locations.push_back(loc);
	locationNames.push_back(text);

	ui->comboBox->addItem(text);
}


//---------Filter Sliders--------------

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



//---------Morpho Checkboxes--------------

/*
	Sets the invert morphological filter on or off based on the checkbox
*/
void MainWindow::on_check_Invert_stateChanged(int state)
{
	if(state == Qt::Checked)
	{
		SkinDetectController::getInstance()->setInvert(true);
	}
	else
	{
		SkinDetectController::getInstance()->setInvert(false);
	}
}

/*
	Sets the erode morphological filter on or off based on the checkbox
*/
void MainWindow::on_check_Erode_stateChanged(int state)
{
	if(state == Qt::Checked)
	{
		SkinDetectController::getInstance()->setErode(true);
	}
	else
	{
		SkinDetectController::getInstance()->setErode(false);
	}
}

/*
	Sets the dilate morphological filter on or off based on the checkbox
*/
void MainWindow::on_check_Dilate_stateChanged(int state)
{
	if(state == Qt::Checked)
	{
		SkinDetectController::getInstance()->setDilate(true);
	}
	else
	{
		SkinDetectController::getInstance()->setDilate(false);
	}
}

/*
	Sets the blur morphological filter on or off based on the checkbox
*/
void MainWindow::on_check_Blur_stateChanged(int state)
{
	if(state == Qt::Checked)
	{
		SkinDetectController::getInstance()->setBlur(true);
	}
	else
	{
		SkinDetectController::getInstance()->setBlur(false);
	}
}



//---------Hand Buttons--------------

/*
	Begin/Stop Hand detection
*/
void MainWindow::on_pushButton_Detect_clicked()
{
	if(timer->isActive())
		handDetect = !handDetect;
	else
	{
		cv::Mat img = SkinDetectController::getInstance()->getInputImage();

		cv::Mat result = detectHand(img);

		if (!result.empty())
			img = result;
		
		displayMat(result, ui->label_Camera);
	}
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
	// qDebug() << "Location:  " << index;
	min = locations[index][0];
	max = locations[index][1];
	setSliders();
}

// END Slots
//##############################################################################


