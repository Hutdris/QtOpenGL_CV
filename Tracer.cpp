#include "Tracer.h"
#define ALL_LED_NUM 9
#define UPPER_LED_NUM 5
#define LOWER_LED_NUM 4
// STD
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;
// GLM
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>



//My header
//#include "Mesh.h"
//#include "shader.h"
//#include "Camera.h"
// Function prototypes
inline bool compare_by_pt_y(cv::KeyPoint &k1,cv::KeyPoint &k2) { return k1.pt.y < k2.pt.y; };
inline bool compare_by_pt_x(cv::KeyPoint &k1,cv::KeyPoint &k2) { return k1.pt.x < k2.pt.x; };
// GL control
void Tracer::load_calibrate_result(void) {
	mtx1 = (cv::Mat_<float>(3, 3) << 2441.347367, 0.000000, 661.608922
		, 0.000000, 2437.002900, 509.925022
		, 0, 0, 1);
	mtx2 = (cv::Mat_<float>(3, 3) <<
		2412.136458, 0.000000, 742.768353,
		0.000000, 2409.743589, 569.840267,
		0, 0, 1)
		;
	dist1 = (cv::Mat_<float>(5, 1) << -0.418326, 12.064726, 0.006454, 0.006439, 0.0);
	dist2 = (cv::Mat_<float>(5, 1) << 0.013682, -4.051145, 0.005566, 0.004979, 0.0);
	RT1 = (cv::Mat_<float>(3, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
	RT2 = (cv::Mat_<float>(3, 4) << 0.695238, -0.000966, 0.718779, -236.973392,
		0.012590, 0.999862, -0.010835, -2.722954,
		-0.718670, 0.016582, 0.695154, 99.387476);

}
void Tracer::load_calibrate_result(cv::Mat &mtx1, cv::Mat &mtx2, cv::Mat &RT1, cv::Mat &RT2, cv::Mat &dist1, cv::Mat &dist2, cv::Mat &fund_mat) {
	mtx1 = (cv::Mat_<float>(3, 3) << 2441.347367, 0.000000, 661.608922
		, 0.000000, 2437.002900, 509.925022
		, 0, 0, 1);
	mtx2 = (cv::Mat_<float>(3, 3) <<
		2412.136458, 0.000000, 742.768353,
		0.000000, 2409.743589, 569.840267,
		0, 0, 1)
		;
	dist1 = (cv::Mat_<float>(5, 1) << -0.418326, 12.064726, 0.006454, 0.006439, 0.0);
	dist2 = (cv::Mat_<float>(5, 1) << 0.013682, -4.051145, 0.005566, 0.004979, 0.0);
	RT1 = (cv::Mat_<float>(3, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
	RT2 = (cv::Mat_<float>(3, 4) << 0.695238, -0.000966, 0.718779, -236.973392,
		0.012590, 0.999862, -0.010835, -2.722954,
		-0.718670, 0.016582, 0.695154, 99.387476);
}
void Tracer::load_mtx(fstream &fileReader, vector <float> &mat, int mat_size) {
	string _text;
	mat.clear();
	for (int i = 0; i<mat_size; i++) {
		fileReader >> _text;
		mat.push_back(stof(_text));
	}
}
void Tracer::load_calibrate_result(const char *filePath) {
	fstream fr(filePath);
	if (!filePath) {
		qDebug("Calibrate file reading fail.");
	}
	else {
		/*
		#LeftCamK>>mtx1		3*3
		#LeftCamDist>>dist1 1*5 -> 5*1
		#LeftCamRT>>RT1     3*4 -> 3*4
		 RightCam->Cam2
		*/
		string text;
		while (fr >> text) {
			vector <float> _mat;
			if (text.at(0) == '#') {
				if (text == "#LeftCamK") {
					load_mtx(fr, _mat, 3 * 3);
					mtx1 = cv::Mat(3, 3, CV_32F, &(_mat[0]));
					for (int i = 0; i < 3; i++) {
						qDebug("%f, %f, %f", mtx1.at<float>(i, 0), mtx1.at<float>(i, 1), mtx1.at<float>(i, 2)) ;
					}
				}
				else if (text == "#RightCamK") {
					load_mtx(fr, _mat, 3 * 3);
					mtx2 = cv::Mat(3, 3, CV_32F, &(_mat[0]));
					for (int i = 0; i < 3; i++) {
						qDebug("%f, %f, %f", mtx2.at<float>(i, 0), mtx1.at<float>(i, 1), mtx1.at<float>(i, 2)) ;
					}
				}
				else if (text == "#LeftCamDist") {
					load_mtx(fr, _mat, 5*1);
					dist1 = cv::Mat(5, 1, CV_32F, &(_mat[0]));
				}
				else if (text == "#RightCamDist") {
					load_mtx(fr, _mat, 5*1);
					dist2 = cv::Mat(5, 1, CV_32F, &(_mat[0]));
				}
				else if (text == "#LeftCamRT") {
					load_mtx(fr, _mat, 3*4);
					RT1 = cv::Mat(3, 4, CV_32F, &(_mat[0]));
				}
				else if (text == "#RightCamRT") {
					load_mtx(fr, _mat, 3*4);
					RT2 = cv::Mat(3, 4, CV_32F, &(_mat[0]));
				}
			}

		}
		cout << endl;
		fr.close();
	}

}

void Tracer::leds_triangulate(cv::Mat &tri_points) {
	const int led_num = 9;
	if ((all_leds_key1.size() == all_leds_key2.size())&(all_leds_key2.size() == 9)) {
		//traiangluate eat '2*N' array not '1*N' points vector
		//Tranlate keypoints to sorted vector<Point2d>
		vector<cv::Point2d> f_leds1, f_leds2;
		for (int i = 0; i < led_num; i++) {
			f_leds1.push_back(all_leds_key1[i].pt);
			f_leds2.push_back(all_leds_key2[i].pt);
		}
		//sort(f_leds1.begin(), f_leds1.end(), pt_compare_by_y);
		//sort(f_leds2.begin(), f_leds2.end(), pt_compare_by_y);
		//4*N {x, y, z, alpha}^T each col
		tri_points.create(4, led_num, CV_32F);
		cv::triangulatePoints(project1, project2, f_leds1, f_leds2, tri_points);
		cout << tri_points << endl;
	}
}


void Tracer::leds_triangulate(vector<cv::KeyPoint> &leds1, vector<cv::KeyPoint> &leds2,
	const cv::Mat &project1, const cv::Mat &project2, int led_num, cv::Mat &tri_points) {
	if ((leds1.size() == leds2.size())&(leds1.size() == led_num)) {
		//traiangluate eat '2*N' array not '1*N' points vector
		//Tranlate keypoints to sorted vector<Point2d>
		vector<cv::Point2d> f_leds1, f_leds2;
		for (int i = 0; i < led_num; i++) {
			f_leds1.push_back(leds1[i].pt);
			f_leds2.push_back(leds2[i].pt);
		}
		//sort(f_leds1.begin(), f_leds1.end(), pt_compare_by_y);
		//sort(f_leds2.begin(), f_leds2.end(), pt_compare_by_y);
		//4*N {x, y, z, alpha}^T each col
		tri_points.create(4, led_num, CV_32F);
		triangulatePoints(project1, project2, f_leds1, f_leds2, tri_points);
		cout << tri_points << endl;
	}
}

Tracer::Tracer()
{
	initialize();
}
void Tracer::initialize() {
	//Initalization
	// Cali params	
	load_calibrate_result("qrc/cam_config.txt");
	project1 = mtx1*RT1;
	project2 = mtx2*RT2;
	//BlobDetector parms
	glob_blob_p.minThreshold = 110;
	glob_blob_p.maxThreshold = 400;
	glob_blob_p.filterByColor = true;
	glob_blob_p.blobColor = 255;
	glob_blob_p.filterByArea = true;
	glob_blob_p.minArea = 100;
	glob_blob_p.maxArea = 1000;
	glob_blob_p.filterByCircularity = true;
	glob_blob_p.minCircularity = 0.7f;
	glob_blob_p.filterByConvexity = false;
	glob_blob_p.minConvexity = 0.7f;
	glob_blob_p.filterByInertia = false;
	glob_blob_p.minInertiaRatio = 0.1f;
	glob_blob_detector = cv::SimpleBlobDetector::create(glob_blob_p);
	auto check = 0;
}

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}
void Tracer::points_update() {
	const int total_leds_cnt = 9;
	string _check = type2str(raw_src1.type());

	cv::Ptr<cv::SimpleBlobDetector>test_detector = cv::SimpleBlobDetector::create(glob_blob_p);
 /*
	cv::cvtColor(this->raw_src1, this->raw_src1, cv::COLOR_BGR2GRAY);
	cv::cvtColor(this->raw_src2, this->raw_src2, cv::COLOR_BGR2GRAY);
	glob_blob_detector->detect(raw_src1, all_leds_key1);
	glob_blob_detector->detect(raw_src2, all_leds_key2);
	*/
	vector <cv::KeyPoint> test_leds;
	raw_src1 = cv::imread("qrc/l.png", 0);
	raw_src2 = cv::imread("qrc/r.png", 0);
	test_detector->detect(this->raw_src1, all_leds_key1);
	test_detector->detect(raw_src2, all_leds_key2);
	drawKeypoints(raw_src1, all_leds_key1, raw_src1, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(raw_src2, all_leds_key2, raw_src2, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cv::imshow("cam1", raw_src1);
	cv::waitKey(0);
	cv::imshow("cam2", raw_src2);
	cv::waitKey(0);
	qDebug("%d, %d", all_leds_key1.size(), all_leds_key2.size());
	if ((all_leds_key1.size() > total_leds_cnt) || (all_leds_key1.size() > total_leds_cnt)){
		all_leds_key1.clear();
		all_leds_key2.clear();
		return;
	}
	std::sort(all_leds_key1.begin(), all_leds_key1.end(), compare_by_pt_y);
	std::sort(all_leds_key2.begin(), all_leds_key2.end(), compare_by_pt_y);
	
/*
	drawKeypoints(frame1, all_leds_key1, frame1, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(frame2, all_leds_key2, frame2, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
*/
	
}
void Tracer::image_update(PGApi &pgmgr) {
	pgmgr.GetStereoImage(raw_src1, raw_src2);
}


