#pragma once
#pragma once

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <windows.h>
#include <thread>

#include <tchar.h>
#include <atlstr.h>

class Grab
{
public:
	Grab()
	{
		m_strRTSPAddr = "";
		m_isDevInit = false;
		m_bStartStopThead = false;
		m_pCvCap = NULL;
	}

	~Grab()
	{
		if (m_pCvCap != NULL)
		{
			m_pCvCap->release();
			delete m_pCvCap;
			m_pCvCap = NULL;
		}
	}

	void setIsInitDev(bool _OnOff)
	{
		m_isDevInit = _OnOff;
	}
	bool getIsInitDev()
	{
		return m_isDevInit;
	}

	void setStartStop(bool _OnOff)
	{
		m_bStartStopThead = _OnOff;
	}

	bool getStartStop() { return m_bStartStopThead; }


	std::queue<cv::Mat>* getBuffer() { return &buffer; }

	cv::Mat getFrame(std::queue<cv::Mat>* buffer) {
		mutex.lock();
		//std::cout << "buffer: "<<buffer.size() << std::endl;
		
		auto img = buffer->front();
		while (!buffer->empty())
			buffer->pop();
		//std::cout << "buffer: " << buffer.size() << std::endl;
		mutex.unlock();

		return img;
	}

	cv::VideoCapture getm_pCvCap() { return *m_pCvCap; }

	bool Init(std::string strRTSPAddr)
	{
		m_strRTSPAddr = strRTSPAddr;

		if (m_pCvCap != NULL)
		{
			m_pCvCap->release();
			delete m_pCvCap;
			m_pCvCap = NULL;
		}

		m_pCvCap = new cv::VideoCapture(strRTSPAddr);

		//비디오 캡쳐가 준비되었는지 확인하기
		if (!m_pCvCap->isOpened())
		{
			setIsInitDev(false);
			return false;
		}

		setIsInitDev(true);

		return true;
	}//end of function

	bool start();

	void wait()
	{
		m_grabThread.join();
	}
	std::mutex mutex;

private:
	bool m_isDevInit;
	bool m_bStartStopThead;

	std::thread m_grabThread;
	std::thread m_rtspThread;
	std::string m_strRTSPAddr;

	std::queue<cv::Mat> buffer;

	cv::VideoCapture* m_pCvCap;
};
