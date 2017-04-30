#pragma once
#include <vector>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/video/tracking.hpp>
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
	cv::KalmanFilter kf_gen();
	int points_update();
	int find_points(cv::Mat & frame, vector<cv::KeyPoint> &pts);
	void point_registor(vector<cv::KeyPoint> & pts);
	void image_update(PGApi &pgmgr);
	void image_update_from_video();
	void Tracer::pre_frame_check();
	cv::Mat lower_RT, upper_RT;
	const cv::Mat get_tri_points() {
		static cv::Mat new_points = tri_points.clone();
		return new_points;
	};
	const pair<vector<cv::KeyPoint>, vector<cv::KeyPoint>> get_all_pts() {
		return make_pair(all_leds_key1, all_leds_key2);
	}
	cv::Mat init_RT, lower_RT_display;

	const int max_RT_sem = 1;
	void reset_init_pos() {
		RT_sem = max_RT_sem;
	}
private:

	void Tracer::getTransformation(cv::Mat pre_pts, cv::Mat cur_pts, cv::Mat &RT);
	//calibration results matrixs
	cv::Mat dist1, dist2, mtx1, mtx2, RT1, RT2, fund_mat, test, project1, project2;
	//led points
	cv::Mat all_leds, pt_measurement;
	vector<cv::KeyPoint> all_leds_key1, all_leds_key2;
	pair<vector<cv::KeyPoint>, vector<cv::KeyPoint>> pre_leds;
	cv::Mat fourD_points, lower_leds, facebow_leds, tri_points, pre_tri_points,  init_pos;

	//image matrixs
	cv::Mat raw_src1, raw_src2, frame1, frame2, frame1_with_led, frame2_with_led;
	pair<cv::Mat, cv::Mat> pre_raw_src;
	cv::SimpleBlobDetector::Params glob_blob_p;
	cv::Ptr<cv::SimpleBlobDetector> glob_blob_detector;



	class VideoHandler
	{
	public:
		cv::VideoCapture leftCam, rightCam;
		bool leftFlip, rightFlip;
	};
	bool RT_inited = false, points_init = false;
	int RT_sem = max_RT_sem;
	vector<cv::KalmanFilter> tri_KF;
	VideoHandler vh;
	class KalmanHandler {
	public:
		vector<cv::KalmanFilter> l_cam_KF, r_cam_KF;
		vector<cv::Mat> l_predict
			, r_predict
			, l_measure
			, r_measure
			, l_estimate
			, r_estimate;
		bool inited = false;
	};
	KalmanHandler kalman_handler;

	cv::KalmanFilter kf3d_gen(cv::Point3f init_pt);
};


