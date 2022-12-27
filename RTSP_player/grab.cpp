#include "grab.h"
#include "utils.h"
#include "detector.h"


bool capture(Grab* pGrabber)
{
	if (pGrabber == NULL)
		return false;

	if (!pGrabber->getIsInitDev()) {
		std::cout << "Can't open video.";
		return false;
	}
	std::queue<cv::Mat>* buffer = pGrabber->getBuffer();
	//OpenCV Mat class
	cv::Mat videoFrame;

	//std::queue<cv::Mat>* buffer = pGrabber->getBuffer();

	while (pGrabber->getStartStop())
	{
		//VideoCapture로 부터 프래임을 받아온다
		pGrabber->getm_pCvCap() >> videoFrame;
		Sleep(5);
		pGrabber->mutex.lock();
		buffer->push(videoFrame);
		pGrabber->mutex.unlock();
	}
	return true;
}


bool Grab::start()
{
	m_grabThread = std::thread(capture, this);
	return true;
}
