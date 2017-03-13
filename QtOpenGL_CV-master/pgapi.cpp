//=============================================================================
// Copyright ?2008 Point Grey Research, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of
// Point Grey Research, Inc. ("Confidential Information"). You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the "License Agreement" that you
// entered into with PGR in connection with this software.
//
// UNLESS OTHERWISE SET OUT IN THE LICENSE AGREEMENT, THIS SOFTWARE IS
// PROVIDED ON AN “AS-IS?BASIS AND POINT GREY RESEARCH INC. MAKES NO
// REPRESENTATIONS OR WARRANTIES ABOUT THE SOFTWARE, EITHER EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OR
// CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// NON-INFRINGEMENT. POINT GREY RESEARCH INC. SHALL NOT BE LIABLE FOR ANY
// DAMAGES, INCLUDING BUT NOT LIMITED TO ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES, OR ANY LOSS OF PROFITS,
// REVENUE, DATA OR DATA USE, ARISING OUT OF OR IN CONNECTION WITH THIS
// SOFTWARE OR OTHERWISE SUFFERED BY YOU AS A RESULT OF USING, MODIFYING
// OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================
//=============================================================================
// $Id: FlyCapture2Test.cpp 300855 2016-09-30 22:48:39Z erich $
//=============================================================================
#include <iostream>
#include <sstream>
#include "pgapi.h"


using namespace FlyCapture2;
using namespace std;

PGApi::PGApi(){}
PGApi::PGApi(unsigned int snum1, unsigned int snum2) {
	serialNumber1 = snum1;
	serialNumber2 = snum2;
	BusManager busMgr;

	Error error = busMgr.GetCameraFromSerialNumber(serialNumber1, &guid1);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
	}

	Error error2 = busMgr.GetCameraFromSerialNumber(serialNumber2, &guid2);
	if (error2 != PGRERROR_OK)
	{
		PrintError(error);
	}

	// Connect to a camera
	error = cam1.Connect(&(this->guid1));
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	error = cam2.Connect(&(this->guid2));
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam1.GetCameraInfo(&camInfo);
	error = cam2.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	PrintCameraInfo(&camInfo);

	// Get the camera configuration
	FC2Config config;
	error = cam1.GetConfiguration(&config);
	error = cam2.GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	// Set the number of driver buffers used to 10.
	config.numBuffers = 10;

	// Set the camera configuration
	error = cam1.SetConfiguration(&config);
	error = cam2.SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	// Start capturing images
	error = cam1.StartCapture();
	error = cam2.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}
}
PGApi::~PGApi() {
	cam1.Disconnect();
	cam2.Disconnect();
}
/*
void PGApi::setGuid(bool camera_one) {
	BusManager busMgr;
	if (camera_one) {
		Error error = busMgr.GetCameraFromSerialNumber(serialNumber1, &(this->guid1));

	}
	else {

		Error error = busMgr.GetCameraFromSerialNumber(serialNumber1, &(this->guid1));
	}
	if (error != PGRERROR_OK)
	{
		PrintError(error);
	}
}
*/
void PGApi::PrintBuildInfo()
{
	FC2Version fc2Version;
	Utilities::GetLibraryVersion(&fc2Version);

	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "."
		<< fc2Version.minor << "." << fc2Version.type << "."
		<< fc2Version.build;
	cout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	cout << timeStamp.str() << endl << endl;
}


void PGApi::PrintCameraInfo(CameraInfo *pCamInfo)
{
	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number - " << pCamInfo->serialNumber << endl;
	cout << "Camera model - " << pCamInfo->modelName << endl;
	cout << "Camera vendor - " << pCamInfo->vendorName << endl;
	cout << "Sensor - " << pCamInfo->sensorInfo << endl;
	cout << "Resolution - " << pCamInfo->sensorResolution << endl;
	cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
	cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl
		<< endl;
}

void PGApi::PrintError(Error error) { error.PrintErrorTrace(); }

bool PGApi::PollForTriggerReady(Camera *pCam)
{
	const unsigned int k_softwareTrigger = 0x62C;
	Error error;
	unsigned int regVal = 0;

	do
	{
		error = pCam->ReadRegister(k_softwareTrigger, &regVal);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return false;
		}

	} while ((regVal >> 31) != 0);

	return true;
}

void PGApi::GetStereoImage(cv::Mat &image1, cv::Mat &image2) {
	//image 1 filp !!
	Image rawImage1, rawImage2;
	PollForTriggerReady(&cam1);
	Error error;
	error = cam1.FireSoftwareTrigger();
		// Retrieve an image
	error = cam1.RetrieveBuffer(&rawImage1);
	error = cam2.RetrieveBuffer(&rawImage2);
		//cout << "Grabbed image " << imageCnt << endl;

		// Create a converted image
		Image convertedImage1, convertedImage2;

		// Convert the raw image
		error = rawImage1.Convert(PIXEL_FORMAT_MONO8, &convertedImage1);
		error = rawImage2.Convert(PIXEL_FORMAT_MONO8, &convertedImage2);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return;
		}

		// convert to OpenCV Mat

		unsigned int rowBytes1 = static_cast<uint>((double)convertedImage1.GetReceivedDataSize() / (double)convertedImage1.GetRows());
		cv::Mat _img1(
			convertedImage1.GetRows(),
			convertedImage1.GetCols(),
			CV_8U,
			convertedImage1.GetData(),
			rowBytes1
		);
		cv::flip(_img1, image1, 0);
		//_img1.copyTo(image1);
		unsigned int rowBytes2 = static_cast<uint>((double)convertedImage2.GetReceivedDataSize() / (double)convertedImage2.GetRows());
		cv::Mat _img2(convertedImage2.GetRows(), convertedImage2.GetCols(), CV_8U, convertedImage2.GetData(), rowBytes2);
		_img2.copyTo(image2);
		//cv::Mat _img1(200, 100, CV_8UC3);

}
//int RunSingleCamera(PGRGuid guid)
int PGApi::RunMultipleCamera( cv::Mat &image1, cv::Mat &image2)
{
	const int k_numImages = 10;

	Error error;

	// Connect to a camera
	Camera cam1, cam2;
	error = cam1.Connect(&(this->guid1));
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
	
	error = cam2.Connect(&(this->guid2));
	if (error != PGRERROR_OK)
	{
	PrintError(error);
	return -1;
	}
	
	// Get the camera information
	CameraInfo camInfo;
	error = cam1.GetCameraInfo(&camInfo);
	error = cam2.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	PrintCameraInfo(&camInfo);

	// Get the camera configuration
	FC2Config config;
	error = cam1.GetConfiguration(&config);
	error = cam2.GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Set the number of driver buffers used to 10.
	config.numBuffers = 10;

	// Set the camera configuration
	error = cam1.SetConfiguration(&config);
	error = cam2.SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Start capturing images
	error = cam1.StartCapture();
	error = cam2.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
	char key = 0;
	Image rawImage1, rawImage2;
	cout << "mutiloop" << endl;
	while (key != 'q')
	//for (int imageCnt = 0; imageCnt < k_numImages; imageCnt++)
	{
		PollForTriggerReady(&cam1);
		error = cam1.FireSoftwareTrigger();
		// Retrieve an image
		error = cam1.RetrieveBuffer(&rawImage1);
		error = cam2.RetrieveBuffer(&rawImage2);/////////////Problem!!!!!!!!!!!!///////
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			continue;
		}

		//cout << "Grabbed image " << imageCnt << endl;

		// Create a converted image
		Image convertedImage1, convertedImage2;

		// Convert the raw image
		error = rawImage1.Convert(PIXEL_FORMAT_MONO8, &convertedImage1);
		error = rawImage2.Convert(PIXEL_FORMAT_MONO8, &convertedImage2);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}

		// convert to OpenCV Mat
		
		unsigned int rowBytes1 = static_cast<uint>((double)convertedImage1.GetReceivedDataSize() / (double)convertedImage1.GetRows());
		cv::Mat _img1(
			convertedImage1.GetRows(), 
			convertedImage1.GetCols(), 
			CV_8U, 
			convertedImage1.GetData(), 
			rowBytes1
		);
		_img1.copyTo(image1);
		unsigned int rowBytes2 = static_cast<uint>((double)convertedImage2.GetReceivedDataSize() / (double)convertedImage2.GetRows());
		cv::Mat _img2(convertedImage2.GetRows(), convertedImage2.GetCols(), CV_8U, convertedImage2.GetData(), rowBytes2);
		_img2.copyTo(image2);
		//cv::Mat _img1(200, 100, CV_8UC3);


		cv::imshow("image", image1);
		key = cv::waitKey(30);

		// Create a unique filename
		/*
		ostringstream filename;
		filename << "FlyCapture2Test-" << camInfo.serialNumber << "-"
			<< imageCnt << ".pgm";
		
		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		error = convertedImage.Save(filename.str().c_str());
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}
		*/
	}

	// Stop capturing images
	error = cam1.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
	
	error = cam2.StopCapture();
	if (error != PGRERROR_OK)
	{
	PrintError(error);
	return -1;
	}
	
	// Disconnect the camera
	error = cam1.Disconnect();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
	
	error = cam2.Disconnect();
	if (error != PGRERROR_OK)
	{
	PrintError(error);
	return -1;
	}
	
	return 0;
}
/*
int main(int argc, char ** argv)
{
	PrintBuildInfo();

	Error error;

	// Since this application saves images in the current folder
	// we must ensure that we have permission to write to this folder.
	// If we do not have permission, fail right away.
	FILE *tempFile = fopen("test.txt", "w+");
	if (tempFile == NULL)
	{
		cout << "Failed to create file in current folder.  Please check "
			"permissions."
			<< endl;
		return -1;
	}
	fclose(tempFile);
	remove("test.txt");

	BusManager busMgr;
	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	cout << "Number of cameras detected: " << numCameras << endl;
	PGRGuid guid1, guid2;
	unsigned int serialNumber1, serialNumber2;
	serialNumber1 = 16154282;
	serialNumber2 = 16154288;
	//for (unsigned int i = 0; i < numCameras; i++)
	//{
	//PGRGuid guid;
	error = busMgr.GetCameraFromSerialNumber(serialNumber1, &guid1);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
	
	error = busMgr.GetCameraFromSerialNumber(serialNumber2, &guid2);
	if (error != PGRERROR_OK)
	{
	PrintError(error);
	return -1;

	//RunSingleCamera(guid);

	}
	cv::Mat img1;
	cv::Mat img2;
	RunMultipleCamera(guid1, guid2, img1, img2);
	cout << "Press Enter to exit..." << endl;
	cin.ignore();

	return 0;
}
*/