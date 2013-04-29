#ifndef BACKWIDGET_H
#define BACKWIDGET_H
#include <QWidget>


class BackgroundWidget : public QWidget
{
 
Q_OBJECT
 
public:

//##############################################################################
//	Constructors / Destructor
	explicit BackgroundWidget( QWidget * parent = 0) :
		QWidget(parent)
	{
	}

	~BackgroundWidget() {}

//##############################################################################



// //##############################################################################
// //	MODIFIERS/ACCESSORS
// 	void setMin(cv::Scalar min)
// 	{
// 		this->min = min;
// 	}

// 	cv::Scalar getMin()
// 	{
// 		return min;
// 	}


// 	void setMax(cv::Scalar max)
// 	{
// 		this->max = max;
// 	}
	
// 	cv::Scalar getMax()
// 	{
// 		return max;
// 	}
// //##############################################################################


// private:
// 	// the thresholding masks
// 	cv::Scalar min, max;
	


};

#endif // BACKWIDGET_H
