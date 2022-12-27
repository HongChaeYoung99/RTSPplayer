//C++ header file 
#include <iostream>

//opencv header file include
#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "cmdline.h"
#include "utils.h"
#include "detector.h"
#include <thread>
#include "grab.h"
#include <string>
#include "config.h"
#include "Builder.h"

#define SHAMED_NAME _T("AA")
//#define VIDEO_PATH "C:\\Users\\Minji\\source\\repos\\OpenCV_Test\\OpenCV_Test\\vtest.avi"
#define WEBCAM_NUM 0
////https://www.wowza.com/html/mobile.html
//#define RTSP_URL "C:\\Users\\Minji\\Downloads\\file_example.mp4"


#define VIDEO_WINDOW_NAME "video"

cv::Mat videoImage;

std::string RTSP_URL = "rtsp://210.99.70.120:1935/live/cctv002.stream";

//typedef struct
//{
//	int idx;
//	float x;
//	float y;
//} MYSTRUCT;
int rtsp_player(Grab* grab) {
	bool isGPU = false;
	const std::string classNamesPath = "C:\\Users\\Minji\\source\\repos\\openCV_practice\\openCV_practice\\coco.names";
	const std::vector<std::string> classNames = utils::loadNames(classNamesPath);
	const std::string imagePath = "";
	const std::string modelPath = "C:\\Users\\Minji\\source\\repos\\openCV_practice\\openCV_practice\\smart.onnx";
	const float confThreshold = 0.5f;
	const float iouThreshold = 0.5f;
	YOLODetector detector{ nullptr };
	std::vector<Detection> result;

	//클래스 이름 파일이 비어있을 경우
	if (classNames.empty())
	{
		std::cerr << "Error: Empty class names file." << std::endl;
		return -1;
	}

	//이미지를 window를 생성하여 보여줍니다.
	// /////////////////////////////////////////////////////
	cv::namedWindow(VIDEO_WINDOW_NAME);
	///////////////////////////////////////////////////////

	//std::queue<cv::Mat>* buffer = grab->getBuffer();
	std::queue<cv::Mat>* buffer = grab->getBuffer();
	Director* director = new Director(new miniBuilder);
	Config* config = director->construct();
	int w = config->getInteger(0, "size", "width");
	int h = config->getInteger(0, "size", "height");
	//video 재생 시작
	while (true) {
		if (buffer->empty()) {
			//캡쳐 화면이 없는 경우는 Video의 끝인 경우
			if (!grab->getStartStop())
				break;
			continue;
			std::cout << "Video END" << std::endl;
		}
		//버퍼에 있는 이미지 가져오기1
		auto img = grab->getFrame(buffer);

		try
		{
			if (img.empty())
				continue;

			detector = YOLODetector(modelPath, isGPU, cv::Size(640, 640));
			result = detector.detect(img, confThreshold, iouThreshold);
			 /////////////////////////////////////////////////////
			/*wchar_t path[MAX_PATH] = { 0 };
			GetModuleFileName(NULL, path, MAX_PATH);

			USES_CONVERSION;
			std::string str = W2A(path);*/
			//str = str.substr(0, str.find_last_of("\\/"));

			
			static const UINT regMsg = ::RegisterWindowMessage(_T("OpenCVplayer"));

			//CString str = _T("테스트");

			COPYDATASTRUCT cds;
			cds.dwData = 100;
			/*
			std::wstring widestr = std::wstring(str.begin(), str.end());
			const wchar_t* widecstr = widestr.c_str();*/
			CString str = _T("테스트");
			cds.cbData = sizeof(TCHAR) * (_tcslen(str) + 1);
			cds.lpData = (TCHAR*)(LPCTSTR)str;

			::SendMessage(HWND_BROADCAST, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
			//std::cout << str.c_str() << std::endl;
			///////////////////////////////////////////////////////////
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;

			return -1;
		}

		utils::visualizeDetection(img, result, classNames);
		
		if (img.empty())
		{
			//예외처리
			//return false
		}
		
		//
		//img.data;
		//int nSizeIage = img.cols * img.rows;
		//int nChannel = img.channels();

	


		//공유메모리쓰려고했던코드
		/*HANDLE hMemoryMap = NULL;
		LPBYTE pMemoryMap = NULL;

		hMemoryMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHAMED_NAME);

		if (hMemoryMap == NULL) {
			hMemoryMap = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(MYSTRUCT), SHAMED_NAME);
		}
		if (!hMemoryMap)
		{
			std::cout << "faild createFileMapping\n";
			MessageBox(NULL, "faild createFileMapping", "Error", MB_OK);
			return false;
		}

		pMemoryMap = (BYTE*)MapViewOfFile(hMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!pMemoryMap)
		{
			CloseHandle(hMemoryMap);
			std::cout << "Failed MapViewOfFile\n";
			MessageBox(NULL, "Failed MapViewOfFile", "Error", MB_OK);
			return false;
		}
		MYSTRUCT* pStruct = (MYSTRUCT*)pMemoryMap;
		pStruct->idx = 4;
		pStruct->x = 20.3f;
		pStruct->y = 10.4f;*/
		/////////////////////////////////////////////////////
		//static const UINT regMsg = ::RegisterWindowMessage(_T("detected_img"));

		//COPYDATASTRUCT cds;
		//cds.cbData = sizeof(img); //전송할 메모리 크기
		//cds.dwData = 1; //식별자32비트(원하는 용도로 사용)
		//cds.lpData = &img; //전송할 메모리 주소
		//::PostMessage(HWND_BROADCAST, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
		//std::cout << str.c_str() << std::endl;
		///////////////////////////////////////////////////////////
		

		
		//cv::Mat* image = (cv::Mat*)pMemoryMap;
		//*image = img;
		

		cv::resize(img, img,cv::Size(h, w));
		cv::resizeWindow(VIDEO_WINDOW_NAME, h, w);
		cv::imshow(VIDEO_WINDOW_NAME, img);
		////'ESC'키를 누르면 종료된다.
		////FPS를 이용하여 영상 재생 속도를 조절하여준다.
		////1000이 1초, fps는 1초당 프레임 몇개 -> 프레임 한개당 1/n초 (키 입력 대기 시간)
		////27는 esc키

		float videoFPS = grab->getm_pCvCap().get(cv::CAP_PROP_FPS);

		if (cv::waitKey(1000 / videoFPS) == 27) {
			std::cout << "Stop Video" << std::endl;
			grab->setStartStop(false);
			break;
		}
	}
	//생성하였던 윈도우를 제거합니다.
	cv::destroyWindow(VIDEO_WINDOW_NAME);
	delete config;
	delete director;
	//아래의 함수를 사용하면, 사용하고 있던 윈도우 전부를 제거합니다.
	cv::destroyAllWindows();
	return 0;
}


//project main function
int main() {

	Grab grab;		// (RTSP_URL);

	if (!grab.Init(RTSP_URL))
	{
		return -1;
	}

	grab.setStartStop(true);
	grab.start();
	std::thread thread2 = std::thread(rtsp_player, &grab);


	grab.wait();
	thread2.join();
	return 0;
}
