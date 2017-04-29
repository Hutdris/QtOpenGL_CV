#include "Tracer.h"
#define ALL_LED_NUM 9
#define UPPER_LED_NUM 5
#define LOWER_LED_NUM 4
// STD
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "Eigen\Dense"
#include "Eigen\SVD"
#include "Eigen\Jacobi"


using namespace std;

inline bool compare_by_pt_y(cv::KeyPoint &k1,cv::KeyPoint &k2) { return k1.pt.y < k2.pt.y; };
inline bool compare_by_pt_x(cv::KeyPoint &k1,cv::KeyPoint &k2) { return k1.pt.x < k2.pt.x; };

void Tracer::getTransformation(cv::Mat pre_pts, cv::Mat cur_pts, cv::Mat &RT)
{
	if (pre_pts.cols != cur_pts.cols) {
		return;
	}
	int no_of_samples_(0);
	float accumulated_weight(0.0);
	Eigen::Vector3f pre_mean_(0,0,0), cur_mean_(0,0,0);
	Eigen::Matrix<float, 3, 3> covariance_;
	covariance_.fill(0);
	
	for (int i = 0; i < pre_pts.cols; i++) {
		Eigen::Vector3f pre_pts_, cur_pts_;
		pre_pts_<<(pre_pts.at<float>(0, i)), (pre_pts.at<float>(1, i)), (pre_pts.at<float>(2, i));
		cur_pts_<<(cur_pts.at<float>(0, i)), (cur_pts.at<float>(1, i)), (cur_pts.at<float>(2, i));
		accumulated_weight += 1.0;
		float alpha = 1.0 / accumulated_weight;
		Eigen::Vector3f diff1 = pre_pts_ - pre_mean_, diff2 = cur_pts_ - cur_mean_;
		covariance_ = (1.0f - alpha)*(covariance_ + alpha *(diff2*diff1.transpose()));

		pre_mean_ += alpha*(diff1);
		cur_mean_ += alpha*(diff2);
		
	}



  //Eigen::JacobiSVD<Eigen::Matrix<float, 3, 3> > svd (covariance_, Eigen::ComputeFullU | Eigen::ComputeFullV);
  Eigen::JacobiSVD<Eigen::Matrix<float, 3, 3> > svd (covariance_, Eigen::ComputeFullU | Eigen::ComputeFullV);
  const Eigen::Matrix<float, 3, 3>& u = svd.matrixU(),
                                   & v = svd.matrixV();
  Eigen::Matrix<float, 3, 3> s;
  s.setIdentity();
  if (u.determinant()*v.determinant() < 0.0f)
    s(2,2) = -1.0f;
  
  Eigen::Matrix<float, 3, 3> r = u * s * v.transpose();
  Eigen::Vector3f t = pre_mean_ - r*cur_mean_;
  
  cv::Mat  ret = (cv::Mat_<float>(4, 4) << 
  r(0,0), r(0,1),r(0,2),t(0),
  r(1,0),r(1,1),r(1,2),t(1),
  r(2,0),r(2,1),r(2,2),t(2),
  0.0f,0.0f,0.0f,1.0f
  );
  ret.copyTo(RT);
  /*
  ret(0,0)=r(0,0); ret(0,1)=r(0,1); ret(0,2)=r(0,2); ret(0,3)=t(0);
  ret(1,0)=r(1,0); ret(1,1)=r(1,1); ret(1,2)=r(1,2); ret(1,3)=t(1);
  ret(2,0)=r(2,0); ret(2,1)=r(2,1); ret(2,2)=r(2,2); ret(2,3)=t(2);
  ret(3,0)=0.0f;   ret(3,1)=0.0f;   ret(3,2)=0.0f;   ret(3,3)=1.0f;
  */
}
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
			for (int j = 0; j < 3; j++) {
				 tri_points.at<float>(j, i) /= tri_points.at<float>(3, i);
			}
		}


		cv::Mat _sub_tri;
		cv::Mat _sub_pre_tri;
		cv::Mat tfRT(3, 4, CV_64F)
			, inliner;
		cv::Mat pre_lower_RT;
		vector<cv::Point3d> pre_points, cur_points;
		
		int max_b = max_RT_sem;
		switch (RT_sem)
			{
			case(0):
				_sub_tri = tri_points(cv::Rect(5, 0, 4, 4));
				_sub_pre_tri = pre_tri_points(cv::Rect(5, 0, 4, 4));

				getTransformation(init_pos, _sub_tri, lower_RT);
				printRT(init_pos);

				for (int i = 0; i < 3; i++) {
					lower_RT_display.at<float>(i, 3) -= init_RT.at<float>(i, 3);
				}

				break;
				// printRT(lower_RT);
			case (2): // Fisrt itr: init pre_tripoints
				tri_points.copyTo(pre_tri_points);
				lower_RT_display = lower_RT.clone();
				init_RT = lower_RT.clone();

				RT_sem--;
				break;
			default: // init init_RT loop (0<case<sem_bound)
				_sub_tri = tri_points(cv::Rect(5, 0, 4, 4));
				_sub_pre_tri = pre_tri_points(cv::Rect(5, 0, 4, 4));
				init_pos = _sub_tri.clone();

				getTransformation(init_pos, _sub_tri, lower_RT);

				RT_sem--;
				break;
				/*
				lower_RT = _sub_tri
					*_sub_pre_tri.inv()
					*lower_RT;
						for (int i = 0; i < 3; i++) {
							init_RT.at<float>(i, 3) = tri_points.at<float>(i, 8);
						}

		*/
						//qDebug("lower: %f, %f, %f\n", lower_RT.at<float>(0, 3), lower_RT.at<float>(1, 3), lower_RT.at<float>(2, 3));
						//qDebug("init: %f, %f, %f\n", init_RT.at<float>(0, 3), init_RT.at<float>(1, 3), init_RT.at<float>(2, 3));
		}
			printRT(lower_RT);
		tri_points.copyTo(pre_tri_points);
	}
}


cv::KalmanFilter Tracer::kf3d_gen() {
	cv::KalmanFilter kf(6, 3);
	return kf;
	

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
	glob_blob_p.minThreshold = 10;
	glob_blob_p.maxThreshold = 400;
	glob_blob_p.filterByColor = true;
	glob_blob_p.blobColor = 255;
	glob_blob_p.filterByArea = true;
	glob_blob_p.minArea = 50;
	glob_blob_p.maxArea = 500;
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


int Tracer::find_points(cv::Mat & frame, vector<cv::KeyPoint>& pts) {
	qDebug(type2str(frame.type()).c_str());
	cv::Mat bw(frame.size(), frame.type());
	cv::threshold(
		frame, // src
		frame, // dst
		250,   // threshold_value(0~255)
		255,   // max_binary value
		0	   // thredshold type
	);
	bw = frame;




	// for 0
	cv::Mat labelImage(bw.size(), CV_16U);
	cv::Mat stats;
	cv::Mat centroids;
	int nLabels;
	int neighborhood = 8;
	// for 1
	vector<cv::Vec3f> circles;
	// for 2
	vector<vector<cv::Point>> contours; // Vector for storing contour
	vector<cv::Vec4i> hierarchy;

	int total_leds_cnt = 9;

	pts.clear();
	//if (nLabels == total_leds_cnt+1) { // stats/centroids contains background as label[0]
	int sw = 2; //0->connetedComponent
				// 1-> Hough
				// 2-> findContour
	switch (sw){
	case 0:
		nLabels = cv::connectedComponentsWithStats(bw, labelImage, stats, centroids, 4, CV_16U);
		qDebug("stats:%s centroids:%s", type2str(stats.type()).c_str(), type2str(centroids.type()).c_str());
		qDebug("%d", nLabels);


		for (auto i = 1; i<nLabels; i++) {
			cv::KeyPoint _pt;
			_pt.pt = cv::Point2f((float)centroids.at<double>(i, 0), (float)centroids.at<double>(i, 1));
			_pt.size = max(stats.at<int>(i, cv::ConnectedComponentsTypes::CC_STAT_WIDTH), stats.at<int>(i, cv::ConnectedComponentsTypes::CC_STAT_HEIGHT));
			if (_pt.size > 5)
				pts.push_back(_pt);
		}
		return pts.size();
	case 1:
		// Apply the Hough Transform to find the circles
		cv::HoughCircles(bw, circles, CV_HOUGH_GRADIENT, 1, bw.rows / 50, 80, 40);
		for (auto it = circles.begin(); it != circles.end(); it++) {
			if ((*it)[2] > 3) {
				cv::KeyPoint _pt;
				_pt.pt = cv::Point2f((*it)[0], (*it)[1]);
				_pt.size = (*it)[2]*2;
				pts.push_back(_pt);
			}
		}
		return pts.size();
	case 2:
		//find contour
		cv::findContours(bw, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image
		for (auto it = contours.begin(); it != contours.end(); it++) {
			double a = cv::contourArea(*it, false);
			if (a > 6) {
				cv::Rect b_rect;
				b_rect = cv::boundingRect(*it);

				cv::KeyPoint _pt;
				_pt.pt = cv::Point2f(
					b_rect.x + (b_rect.width / 2)
					, b_rect.y + (b_rect.height / 2));
				_pt.size = max(b_rect.width, b_rect.height);
				pts.push_back(_pt);
				// cv::rectangle(frame, b_rect, 255);
				
			}
		}
		return pts.size();

	}
	if (1) { // stats/centroids contains background as label[0]
	}
	return 0;
}

void Tracer::point_registor(vector<cv::KeyPoint> & pts) {
	std::sort(pts.begin(), pts.end(), compare_by_pt_y);
	std::sort(pts.begin(), pts.begin()+5, compare_by_pt_x);

	std::sort(pts.begin()+5, pts.begin()+5+2, compare_by_pt_x);
	std::sort(pts.begin()+7, pts.begin()+7+2, compare_by_pt_x);
}
int Tracer::points_update() {
	const int total_leds_cnt = 9;
	bool display_flag = true;
	int zoom_ratio = 1;
	cv::Mat pt;
	for (int i = 0; i < total_leds_cnt; i++) {  //Predict pt by kalman fliter
		kalman_handler.l_predict.at(i) = kalman_handler.l_cam_KF.at(i).predict();
		kalman_handler.r_predict.at(i) = kalman_handler.r_cam_KF.at(i).predict();
	}

	cv::Mat zoom_src1, zoom_src2;
	cv::resize(raw_src1, zoom_src1, raw_src1.size() / zoom_ratio);
	cv::resize(raw_src2, zoom_src2, raw_src2.size() / zoom_ratio);
	//glob_blob_detector->detect(zoom_src1, all_leds_key1);
	//glob_blob_detector->detect(zoom_src2, all_leds_key2);

	find_points(raw_src1, all_leds_key1);
	find_points(raw_src2, all_leds_key2);
	for (auto itt = all_leds_key1.begin(); itt != all_leds_key1.end(); itt++) {
		itt->pt *= zoom_ratio;
		itt->size *= zoom_ratio;
	}
	for (auto itt = all_leds_key2.begin(); itt != all_leds_key2.end(); itt++) {
		itt->pt *= zoom_ratio;
		itt->size *= zoom_ratio;
	}


	if (display_flag) {
		drawKeypoints(raw_src1, all_leds_key1, raw_src1, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		drawKeypoints(raw_src2, all_leds_key2, raw_src2, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		// cv::flip(raw_src1, frame1, 0);
		// cv::flip(raw_src2, frame2, 0);
		cv::imshow("camL", raw_src1);
		cv::waitKey(5);
		cv::imshow("camR", raw_src2);
		cv::waitKey(5);
		//cv::destroyAllWindows();
	}
	//qDebug("%d, %d", all_leds_key1.size(), all_leds_key2.size());
	if ((all_leds_key1.size() != total_leds_cnt) | (all_leds_key2.size() != total_leds_cnt)){
		all_leds_key1.clear();
		all_leds_key2.clear();
		return 0;
	}
	point_registor(all_leds_key1);
	point_registor(all_leds_key2);

	return total_leds_cnt; // Shut kalman filter
	
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
				cv::cvtColor(raw_src1,raw_src1, cv::COLOR_BGR2GRAY);
				cv::cvtColor(raw_src2,raw_src2, cv::COLOR_BGR2GRAY);
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
