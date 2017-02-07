#pragma once
#include <vector>

#include "opencv\cv.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include "opencv2/calib3d.hpp"
#include "pgapi.h"
using namespace cv;
using namespace std;
class Tracer
{
public:
	Tracer();
	~Tracer();
	void initialize();
	void load_calibrate_result(Mat &mtx1, Mat &mtx2, Mat &RT1, Mat &RT2, Mat &dist1, Mat &dist2, Mat &fund_mat);
	void leds_triangulate(vector<KeyPoint> &leds1, vector<KeyPoint> &leds2, const Mat &project1, const Mat &project2, int led_num, Mat &tri_points);
	void nothing() {};
	//inline bool pt_compare_by_y(Point2d p1, Point2d p2) { return p1.y < p2.y; };

	void points_update();
	void image_update(PGApi pgmgr);

private:
	cv::Mat left_image, right_image;
	//calibration results matrixs
	Mat dist1, dist2, mtx1, mtx2, RT1, RT2, fund_mat, test, project1, project2;
	//led points
	Mat all_leds;
	vector<KeyPoint> four_leds1, four_leds2, facebow_leds1, facebow_leds2;
	vector<KeyPoint> all_leds_key1, all_leds_key2;
	Mat fourD_points, lower_leds, facebow_leds, tri_points;
	//image matrixs
	Mat raw_src1, raw_src2, frame1, frame2, frame1_with_led, frame2_with_led;
	SimpleBlobDetector::Params glob_blob_p;
	cv::Ptr<SimpleBlobDetector> glob_blob_detector;

};


