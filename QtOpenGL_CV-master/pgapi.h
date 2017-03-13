#pragma once

#include <FlyCapture2.h>
#include "opencv/cv.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace FlyCapture2;
class PGApi {
public:
	PGApi();
	PGApi(unsigned int snum1, unsigned int snum2);
	~PGApi();
	void PrintBuildInfo();
	void PrintCameraInfo(CameraInfo *pCamInfo);
	void PrintError(Error error);
	bool PollForTriggerReady(Camera *pCam);
	void PGApi::GetStereoImage(cv::Mat &image1, cv::Mat &image2);
	int RunMultipleCamera(cv::Mat &image1, cv::Mat &image2);
	unsigned int serialNumber1, serialNumber2;
private:
	PGRGuid guid1, guid2;
	Camera cam1, cam2;
};
