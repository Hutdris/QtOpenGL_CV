#pragma once
#include <vector>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d.hpp>

#include <qDebug>
#include "pgapi.h"

using namespace std;
class Tracer
{
public:
	Tracer();
	void initialize();
	void load_calibrate_result(const char *filePath);
	void load_mtx(fstream &fileReader, vector <float> &mat, int mat_size);
	void leds_triangulate(cv::Mat &tri_points);
	void nothing() {};
	//inline bool pt_compare_by_y(Point2d p1, Point2d p2) { return p1.y < p2.y; };

	void points_update();
	void image_update(PGApi &pgmgr);
	void image_update_from_video();

	cv::Mat lower_RT, upper_RT;
private:
	//cv::Mat left_image, right_image;
	//calibration results matrixs
	cv::Mat dist1, dist2, mtx1, mtx2, RT1, RT2, fund_mat, test, project1, project2;
	//led points
	cv::Mat all_leds;
	vector<cv::KeyPoint> four_leds1, four_leds2, facebow_leds1, facebow_leds2;
	vector<cv::KeyPoint> all_leds_key1, all_leds_key2;
	cv::Mat fourD_points, lower_leds, facebow_leds, tri_points, pre_tri_points;

	//image matrixs
	cv::Mat raw_src1, raw_src2, frame1, frame2, frame1_with_led, frame2_with_led;
	cv::SimpleBlobDetector::Params glob_blob_p;
	cv::Ptr<cv::SimpleBlobDetector> glob_blob_detector;
	//string cam_config_dir = "config/cam_config.txt";
	class VideoHandler
	{
	public:
		cv::VideoCapture leftCam, rightCam;
		bool leftFlip, rightFlip;
	};
	bool RT_inited = false;
	VideoHandler vh;
};


