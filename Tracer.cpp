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

inline bool compare_by_pt_y(cv::KeyPoint &k1,cv::KeyPoint &k2) { return k1.pt.y < k2.pt.y; };
inline bool compare_by_pt_x(cv::KeyPoint &k1,cv::KeyPoint &k2) { return k1.pt.x < k2.pt.x; };

string type2str(int type) {
	// print cv::Mat type
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

void Tracer::load_calibrate_result(const char *filePath) {
	fstream fr(filePath);
	if (!filePath) {
		qDebug("Calibrate file loaded fail.");
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
					mtx1 = cv::Mat(3, 3, CV_32FC1);
					memcpy(mtx1.data, _mat.data(), _mat.size() * sizeof(float));
					for (int i = 0; i < 3; i++) {
						qDebug("%f, %f, %f", mtx1.at<float>(i, 0), mtx1.at<float>(i, 1), mtx1.at<float>(i, 2)) ;
					}
				}
				else if (text == "#RightCamK") {
					load_mtx(fr, _mat, 3 * 3);
					// 害人不淺mtx2 = cv::Mat(3, 3, CV_32FC1, &(_mat[0]));
					mtx2 = cv::Mat(3, 3, CV_32FC1);
					memcpy(mtx2.data, _mat.data(), _mat.size() * sizeof(float));
					for (int i = 0; i < 3; i++) {
						qDebug("%f, %f, %f", mtx2.at<float>(i, 0), mtx1.at<float>(i, 1), mtx1.at<float>(i, 2)) ;
					}
				}
				else if (text == "#LeftCamDist") {
					load_mtx(fr, _mat, 5*1);
					dist1 = cv::Mat(5, 1, CV_32FC1);
					memcpy(dist1.data, _mat.data(), _mat.size() * sizeof(float));
				}
				else if (text == "#RightCamDist") {
					load_mtx(fr, _mat, 5*1);
					dist2 = cv::Mat(5, 1, CV_32FC1);
					memcpy(dist2.data, _mat.data(), _mat.size() * sizeof(float));
				}
				else if (text == "#LeftCamRT") {
					load_mtx(fr, _mat, 3*4);
					RT1 = cv::Mat(3, 4, CV_32FC1);
					memcpy(RT1.data, _mat.data(), _mat.size() * sizeof(float));
				}
				else if (text == "#RightCamRT") {
					load_mtx(fr, _mat, 3*4);
					RT2 = cv::Mat(3, 4, CV_32FC1);
					memcpy(RT2.data, _mat.data(), _mat.size() * sizeof(float));
				}
			}

		}
		cout << endl;
		fr.close();
	}

}

void printMat(cv::Mat m) {

	// jstring matAsString(m.begin<unsigned char>(), m.end<unsigned char>()); 
	// qDebug(matAsString.c_str());
	qDebug() << "hi" << endl;
}
void printTriPTs(cv::Mat pts) {
	for (int i = 0; i < 4; i++) {
		// qDebug("%f %f %f %f %f %f %f %f %f", pts.at<float>(i, 0), pts.at<float>)
	}
	qDebug("");
}
void printRT(cv::Mat rt) {
		for (int r = 0; r < rt.rows; r++) {
				qDebug("%f %f %f %f", rt.at<float>(r, 0), rt.at<float>(r, 1), rt.at<float>(r, 2), rt.at<float>(r, 3));
		}
		qDebug("");
}

void printRTd(cv::Mat rt) {
		for (int r = 0; r < rt.rows; r++) {
				qDebug("%f %f %f %f", rt.at<double>(r, 0), rt.at<double>(r, 1), rt.at<double>(r, 2), rt.at<double>(r, 3));
		}
		qDebug("");
}
void Tracer::leds_triangulate(cv::Mat &tri_points) {
	const int led_num = 9;
	if ((all_leds_key1.size() == all_leds_key2.size())&(all_leds_key2.size() == led_num)) {
		//traiangluate eat '2*N' array not '1*N' points vector
		//Translate keypoints to sorted vector<Point2d>
		vector<cv::Point2f> f_leds1, f_leds2;
		for (int i = 0; i < led_num; i++) {
			f_leds1.push_back(all_leds_key1[i].pt);
			f_leds2.push_back(all_leds_key2[i].pt);
		}
		//sort(f_leds1.begin(), f_leds1.end(), pt_compare_by_y);
		//sort(f_leds2.begin(), f_leds2.end(), pt_compare_by_y);
		//4*N {x, y, z, alpha}^T each col
		cv::triangulatePoints(project1, project2, f_leds1, f_leds2, tri_points);
		// x/=w, y/=w, z/=w, w/=w
		for (int i = 0; i < led_num; i++) {
			for (int j = 0; j < 4; j++) {
				tri_points.at<float>(j, i) /= tri_points.at<float>(3, i);
			}
		}


		cv::Mat _sub_tri;
		cv::Mat _sub_pre_tri;
		cv::Mat tfRT(3, 4, CV_64F)
			, inliner;
		vector<cv::Point3d> pre_points, cur_points;
		

		switch (RT_sem)
			{
			case(0):

		_sub_tri = tri_points(cv::Rect(5, 0, 4, 4));
		_sub_pre_tri = pre_tri_points(cv::Rect(5, 0, 4, 4));

		lower_RT = _sub_tri
			*_sub_pre_tri.inv()
		*lower_RT;
				// printRT(lower_RT_display);
				for (int i = 0; i < 3; i++) {
					lower_RT_display.at<float>(i, 3) -= init_RT.at<float>(i, 3);
				}

				break;
				// printRT(lower_RT);
			case (3): // Fisrt itr: init pre_tripoints
			tri_points.copyTo(pre_tri_points);
			lower_RT_display = lower_RT.clone();
			init_RT = lower_RT.clone();
			RT_sem--;
			break;
			default: // init init_RT loop (0<case<sem_bound)


		for (int i = 5; i < 9; i++) {
			cv::Point3d pre_pt, cur_pt;
			pre_pt.x = double(pre_tri_points.at<float>(0, i));
			pre_pt.y = double(pre_tri_points.at<float>(1, i));
			pre_pt.z = double(pre_tri_points.at<float>(2, i));
			cur_pt.x = double(tri_points.at<float>(0, i));
			cur_pt.y = double(tri_points.at<float>(1, i));
			cur_pt.z = double(tri_points.at<float>(2, i));
			pre_points.push_back(pre_pt);
			cur_points.push_back(cur_pt);
		}
		
		cv::estimateAffine3D(
			pre_points
			,cur_points
			, tfRT, inliner
		);
		pre_points.clear();
		cur_points.clear();
		printRTd(tfRT);
		

		_sub_tri = tri_points(cv::Rect(5, 0, 4, 4));
		_sub_pre_tri = pre_tri_points(cv::Rect(5, 0, 4, 4));
		printRT(_sub_tri);
		printRT(_sub_pre_tri);
		lower_RT = _sub_tri
			*_sub_pre_tri.inv()
			*lower_RT;
				for (int i = 0; i < 3; i++) {
					init_RT.at<float>(i, 3) = tri_points.at<float>(i, 8);
				}

				//qDebug("lower: %f, %f, %f\n", lower_RT.at<float>(0, 3), lower_RT.at<float>(1, 3), lower_RT.at<float>(2, 3));
				//qDebug("init: %f, %f, %f\n", init_RT.at<float>(0, 3), init_RT.at<float>(1, 3), init_RT.at<float>(2, 3));
				RT_sem--;
				break;
		}
			// printRT(lower_RT);
		tri_points.copyTo(pre_tri_points);
	}
}



cv::KalmanFilter Tracer::kf_gen() {
	cv::KalmanFilter kf(4, 2);
	kf.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);

		kf.statePre.at<float>(0) = 0;
		kf.statePre.at<float>(1) = 0;
		kf.statePre.at<float>(2) = 0;
		kf.statePre.at<float>(3) = 0;
		cv::setIdentity(kf.measurementMatrix);
		cv::setIdentity(kf.processNoiseCov, cv::Scalar::all(1e-4));
		cv::setIdentity(kf.measurementNoiseCov, cv::Scalar::all(10));
		cv::setIdentity(kf.errorCovPost, cv::Scalar::all(.1));

		return kf;

}

Tracer::Tracer()
{
	const int led_num = 9;
	vh.leftCam =  cv::VideoCapture("qrc/video/camL.mp4");
	vh.rightCam = cv::VideoCapture("qrc/video/camR.mp4");
	/*
	vh.leftCam = cv::VideoCapture("qrc/video/camL.avi");
	vh.rightCam = cv::VideoCapture("qrc/video/camR.avi");
	*/
	vh.rightFlip = false;
	vh.leftFlip = true;
	pre_tri_points = 
		(cv::Mat_<float>(4, 9)<<
		1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, // x
		0.0 ,1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, // y
		0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, // z
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 // w
		);
	lower_RT = 
		(cv::Mat_<float>(4, 4)<<
		1.0, 0.0, 0.0, 0.0, // x
		0.0 ,1.0, 0.0, 0.0, // y
		0.0, 0.0, 1.0, 0.0, // z
		0.0, 0.0, 0.0, 1.0  //dummy values
		);
	initialize();
	for (int i = 0; i < 9; i++) {
		kalman_handler.l_cam_KF.push_back(kf_gen());
		kalman_handler.r_cam_KF.push_back(kf_gen());
		kalman_handler.l_predict.push_back(cv::Mat::zeros(2, 1, CV_32F));
		kalman_handler.r_predict.push_back(cv::Mat::zeros(2, 1, CV_32F));
		kalman_handler.l_measure.push_back(cv::Mat::zeros(2, 1, CV_32F));
		kalman_handler.r_measure.push_back(cv::Mat::zeros(2, 1, CV_32F));
		kalman_handler.l_estimate.push_back(cv::Mat::zeros(2, 1, CV_32F));
		kalman_handler.r_estimate.push_back(cv::Mat::zeros(2, 1, CV_32F));
	}
	

}

void Tracer::initialize() {
	//Initalization
	// Cali params	
	load_calibrate_result("qrc/cam_config.txt");
	project1 = mtx1*RT1;
	project2 = mtx2*RT2;
	//BlobDetector parms
	glob_blob_p.minThreshold = 1;
	glob_blob_p.maxThreshold = 400;
	glob_blob_p.filterByColor = true;
	glob_blob_p.blobColor = 255;
	glob_blob_p.filterByArea = true;
	glob_blob_p.minArea = 1;
	glob_blob_p.maxArea = 1000;
	glob_blob_p.filterByCircularity = true;
	glob_blob_p.minCircularity = 0.4f;
	glob_blob_p.filterByConvexity = false;
	glob_blob_p.minConvexity = 0.7f;
	glob_blob_p.filterByInertia = false;
	glob_blob_p.minInertiaRatio = 0.1f;
	glob_blob_detector = cv::SimpleBlobDetector::create(glob_blob_p);
}

void keys2pts(vector <cv::KeyPoint> &keys, vector <cv::Point2f> &pts) {
	if (keys.size() != 9) {
		return;
	}
	pts.clear();
	pts.reserve(9);
	for (int i = 0; i < keys.size(); i++) {
		pts.push_back(keys.at(i).pt);
	}
}
void pts2keys(vector <cv::Point2f> &pts, vector <cv::KeyPoint> &keys) {
	if (keys.size() != 9) {
		return;
	}
	for (int i = 0; i < keys.size(); i++) {
		keys.at(i).pt = pts[i];
	}
}

int Tracer::points_update() {
	const int total_leds_cnt = 9;
	bool display_flag = true;
	int zoom_ratio = 4;
	cv::Mat pt;
	for (int i = 0; i < total_leds_cnt; i++) {  //Predict pt by kalman fliter
		kalman_handler.l_predict.at(i) = kalman_handler.l_cam_KF.at(i).predict();
		kalman_handler.r_predict.at(i) = kalman_handler.r_cam_KF.at(i).predict();
	}
	cv::Mat zoom_src1, zoom_src2;
	cv::resize(raw_src1, zoom_src1, raw_src1.size() / zoom_ratio);
	cv::resize(raw_src2, zoom_src2, raw_src2.size() / zoom_ratio);
	glob_blob_detector->detect(zoom_src1, all_leds_key1);
	glob_blob_detector->detect(zoom_src2, all_leds_key2);

	for (auto itt = all_leds_key1.begin(); itt != all_leds_key1.end(); itt++) {
		itt->pt *= zoom_ratio;
		itt->size *= zoom_ratio;
	}
	for (auto itt = all_leds_key2.begin(); itt != all_leds_key2.end(); itt++) {
		itt->pt *= zoom_ratio;
		itt->size *= zoom_ratio;
	}
/*
	if (!points_init) {
		glob_blob_detector->detect(raw_src1, all_leds_key1);
		glob_blob_detector->detect(raw_src2, all_leds_key2);
		points_init = true;
	}
	else {
		pair <vector<cv::Point2f>, vector<cv::Point2f>> pre_pts, pts;
		// 臭
		keys2pts(all_leds_key1, pts.first);
		keys2pts(all_leds_key2, pts.second);
		keys2pts(pre_leds.first, pre_pts.first);
		keys2pts(pre_leds.second, pre_pts.second);

		vector<uchar> status;
		vector<float> err;
		const int max_pry = 3;
		cv::Size winsize(51, 51);
		cv::TermCriteria termcrit(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 30, 0.0001);
		cv::calcOpticalFlowPyrLK(pre_raw_src.first, raw_src1, pre_pts.first, pts.first, status, err, winsize,
	for (auto itt = all_leds_key1.begin(); itt != all_leds_key1.end(); itt++) {
		itt->pt *= zoom_ratio;
		itt->size *= zoom_ratio;
	}
			max_pry, termcrit, 0, 0.001);
		cv::calcOpticalFlowPyrLK(pre_raw_src.second, raw_src2, pre_pts.second, pts.second, status, err, winsize,
			max_pry, termcrit, 0, 0.001);

		pts2keys(pts.first, all_leds_key1);
		pts2keys(pts.second, all_leds_key2);
	}
	*/
	//vector <cv::KeyPoint> test_leds;
	/*
	raw_src1 = cv::imread("qrc/l.png", 0);
	raw_src2 = cv::imread("qrc/r.png", 0);
	*/

	if (display_flag) {
		drawKeypoints(raw_src1, all_leds_key1, raw_src1, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		drawKeypoints(raw_src2, all_leds_key2, raw_src2, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		// cv::flip(raw_src1, frame1, 0);
		// cv::flip(raw_src2, frame2, 0);
		cv::imshow("camL", raw_src1);
		cv::waitKey(10);
		cv::imshow("camR", raw_src2);
		cv::waitKey(10);
		//cv::destroyAllWindows();
	}
	//qDebug("%d, %d", all_leds_key1.size(), all_leds_key2.size());
	if ((all_leds_key1.size() != total_leds_cnt) | (all_leds_key2.size() != total_leds_cnt)){
		all_leds_key1.clear();
		all_leds_key2.clear();
		return 0;
	}
	std::sort(all_leds_key1.begin(), all_leds_key1.end(), compare_by_pt_y);
	std::sort(all_leds_key1.begin(), all_leds_key1.begin()+5, compare_by_pt_x);
	std::sort(all_leds_key1.begin()+5, all_leds_key1.end(), compare_by_pt_x);

	std::sort(all_leds_key2.begin(), all_leds_key2.end(), compare_by_pt_y);
	std::sort(all_leds_key2.begin(), all_leds_key2.begin()+5, compare_by_pt_x);
	std::sort(all_leds_key2.begin()+5, all_leds_key2.end(), compare_by_pt_x);
	
	for (int i = 0; i < total_leds_cnt; i++) {  //keypt to mat(measure)
		kalman_handler.l_measure.at(i).at<float>(0) = all_leds_key1.at(i).pt.x;
		kalman_handler.l_measure.at(i).at<float>(1) = all_leds_key1.at(i).pt.y;
		kalman_handler.r_measure.at(i).at<float>(0) = all_leds_key2.at(i).pt.x;
		kalman_handler.r_measure.at(i).at<float>(1) = all_leds_key2.at(i).pt.y;
		kalman_handler.l_estimate.at(i) = kalman_handler.l_cam_KF.at(i).correct(kalman_handler.l_measure.at(i)); // update kalman filter by measure pt
		kalman_handler.r_estimate.at(i) = kalman_handler.r_cam_KF.at(i).correct(kalman_handler.r_measure.at(i));
		if (!kalman_handler.inited) {
			kalman_handler.inited = true;
			return 0;
		}
		all_leds_key1.at(i).pt = cv::Point2f(kalman_handler.l_estimate.at(i).at<float>(0), kalman_handler.l_estimate.at(i).at<float>(1)); //set keypt by est pt
		all_leds_key2.at(i).pt = cv::Point2f(kalman_handler.r_estimate.at(i).at<float>(0), kalman_handler.r_estimate.at(i).at<float>(1));
	}
	return total_leds_cnt;

}

void Tracer::pre_frame_check() {
	if (!(raw_src1.empty()&&raw_src2.empty())) {
		pre_raw_src.first = raw_src1.clone();
		pre_raw_src.second = raw_src2.clone();
	}
	if ((all_leds_key1.size() != 0) && (all_leds_key2.size() != 0)) { // 假如沒抓到剛好九個點 all_led_keys都會被清空 

		pre_leds.first = all_leds_key1;
		pre_leds.second = all_leds_key2;
	}
}
void Tracer::image_update(PGApi &pgmgr) {
	pgmgr.GetStereoImage(raw_src1, raw_src2);
}

void Tracer::image_update_from_video() {
		if (vh.leftCam.isOpened() & vh.rightCam.isOpened()){
			if (vh.leftCam.read(raw_src1)&vh.rightCam.read(raw_src2)) {
				cv::flip(raw_src1, raw_src1, -1);
				//cv::flip(raw_src2, raw_src2, 0);
				int _zoom = 1;
				cv::resize(raw_src1, raw_src1, raw_src1.size() / _zoom);
				cv::resize(raw_src2, raw_src2, raw_src2.size() / _zoom);
			}
			else {  // reset frames pos when video ends
				vh.leftCam.set(CV_CAP_PROP_POS_FRAMES, 0);
				vh.rightCam.set(CV_CAP_PROP_POS_FRAMES, 0);
				image_update_from_video();
			}
		}
		

}

void Tracer::load_mtx(fstream &fileReader, vector <float> &mat, int mat_size) {
	string _text;
	mat.clear();
	for (int i = 0; i<mat_size; i++) {
		fileReader >> _text;
		mat.push_back(stof(_text));
	}
	//qDebug("mat_size: %d, vec_size: %d", mat_size, int(mat.size()));
}
