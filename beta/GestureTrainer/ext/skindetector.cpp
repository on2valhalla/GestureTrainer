/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.

	Created by: Jason Carlisle Mann (on2valhalla | jcm2207@columbia.edu)

	This class uses holds input threshold min and max
	masks to process an image for skin blobs in HSV colorspace
*/

#include "../ext/skindetector.h"
#include "../ext/colorhistogram.h"

/*
	Processes an HSV image and returns a binary image
	containing blobs of skin regions.

	@hsvImg input HSV colorspace OpenCV2 image
	@return processed binary blob image
*/
cv::Mat SkinDetector::processHSV(const cv::Mat &hsvImg)
{

	//re-allocate binary map if necessary
	//if so create one channel image with 
	//same cols and rows as original
	resultImg.create(hsvImg.rows, hsvImg.cols, CV_8U);

	//reduce the colors for faster processing
	ColorHistogram h;
    h.colorReduce(hsvImg, 12);

    //threshold the image with the stored masks
	cv::inRange(hsvImg, hsvThreshold[0], hsvThreshold[1], resultImg);

	//filtering parameter, increase size for greater effect
	cv::Mat morpElement(5,5,CV_8U,cv::Scalar(1));

	//current morphological processing functions to 
	//close the skin blobs
    //cv::bitwise_not(resultImg, resultImg);
    cv::erode(resultImg, resultImg, morpElement);
    cv::dilate(resultImg, resultImg, morpElement);
    cv::blur(resultImg, resultImg, morpElement.size());
    

	//optional morphological processing is helpful
	//depending on the background environment
	// cv::erode(resultImg, resultImg, morpElement);
	// cv::dilate(resultImg, resultImg, morpElement);
	// cv::morphologyEx(resultImg,resultImg,cv::MORPH_CLOSE, morpElement);

	return resultImg;
}
