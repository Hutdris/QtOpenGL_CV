#include "ICP.h"
#include <algorithm>
#include "GoICP\ConfigMap.hpp"
using namespace std;

ICP::ICP()
{
}


ICP::~ICP()
{
	delete(pModel);
	delete(pData);

}
bool isEqual_float(float x, float y) {
	const float EPSILON = 0.001f;
	return (fabs(x - y) < EPSILON);
}    
/*
struct {
bool operator()(const POINT3D &p1,const  POINT3D &p2) {
	if (p1.x != p2.x) return p1.x < p2.x;
	else if (p1.y != p2.y) return p1.y < p2.y;
	else if (p1.z != p2.z) return p1.z < p2.z;
	}
} compare_POINTS3D;
*/
bool compare_POINTS3D(const POINT3D &p1,const  POINT3D &p2) {
	if (isEqual_float(p1.x, p2.x)) {
		if (isEqual_float(p1.y, p2.y)) {
			return p1.z < p2.z;
		}
		return p1.y < p2.y;
	}
	return p1.x < p2.x;
}
bool isEqual_POINTS3D(const POINT3D &p1,const  POINT3D &p2) {
	return (isEqual_float(p1.x, p2.x)&isEqual_float(p1.y, p2.y)&isEqual_float(p1.z, p2.z));
}
void ICP::load_points(const std::vector<float> &vModel, const std::vector<float> &vData) {
	vector<POINT3D> temp_points;
	for (auto itt = vModel.begin(); itt != vModel.end(); itt += 3) {
		POINT3D temp_point;
		temp_point.x = *itt;
		temp_point.y = *(itt + 1);
		temp_point.z = *(itt + 2);
		temp_points.push_back(temp_point);
	}
	std::sort(temp_points.begin(), temp_points.end(), compare_POINTS3D);
	auto trim_head = unique(temp_points.begin(), temp_points.end(), isEqual_POINTS3D);
	temp_points.erase(trim_head, temp_points.end());
	pModel = (POINT3D *)malloc(sizeof(POINT3D) * temp_points.size());
	for (auto i = 0; i < temp_points.size(); i++) {
		pModel[i] = temp_points.at(i);
	}
	Nm = int(temp_points.size());
	temp_points.clear();
	for (auto itt = vData.begin(); itt != vData.end(); itt += 3) {
		POINT3D temp_point;
		temp_point.x = *itt;
		temp_point.y = *(itt + 1);
		temp_point.z = *(itt + 2);
		temp_points.push_back(temp_point);
	}
	std::sort(temp_points.begin(), temp_points.end(), compare_POINTS3D);
	trim_head = unique(temp_points.begin(), temp_points.end(), isEqual_POINTS3D);
	temp_points.erase(trim_head, temp_points.end());
	pData = (POINT3D *)malloc(sizeof(POINT3D) * temp_points.size());
	for (auto i = 0; i < temp_points.size(); i++) {
		pData[i] = temp_points.at(i);
	}
	Nd = int(temp_points.size());
	temp_points.clear();	

}

void ICP::readConfig(string FName)
{
	// Open and parse the associated config file
	ConfigMap config(FName.c_str());

	goicp.MSEThresh = config.getF("MSEThresh");
	goicp.initNodeRot.a = config.getF("rotMinX");
	goicp.initNodeRot.b = config.getF("rotMinY");
	goicp.initNodeRot.c = config.getF("rotMinZ");
	goicp.initNodeRot.w = config.getF("rotWidth");
	goicp.initNodeTrans.x = config.getF("transMinX");
	goicp.initNodeTrans.y = config.getF("transMinY");
	goicp.initNodeTrans.z = config.getF("transMinZ");
	goicp.initNodeTrans.w = config.getF("transWidth");
	goicp.trimFraction = config.getF("trimFraction");
	// If < 0.1% trimming specified, do no trimming
	if (goicp.trimFraction < 0.001)
	{
		goicp.doTrim = false;
	}
	goicp.dt.SIZE = config.getI("distTransSize");
	goicp.dt.expandFactor = config.getF("distTransExpandFactor");

	cout << "CONFIG:" << endl;
	config.print();
	//cout << "(doTrim)->(" << goicp.doTrim << ")" << endl;
	cout << endl;
}
void ICP::run() {

	goicp.pModel = pModel;
	goicp.Nm = Nm;
	goicp.pData = pData;
	goicp.Nd = Nd;

	// Build Distance Transform
	goicp.BuildDT();
	int NdDownsampled = min(1000, Nd);
	// Run GO-ICP
	if (NdDownsampled > 0)
	{
		goicp.Nd = NdDownsampled; // Only use first NdDownsampled data points (assumes data points are randomly ordered)
	}
	goicp.Register();
	cout << "Optimal Rotation Matrix:" << endl;
	cout << goicp.optR << endl;
	cout << "Optimal Translation Vector:" << endl;
	cout << goicp.optT << endl;

	string outputFname = "result/icp_output.txt";
	ofstream ofile;
	ofile.open(outputFname.c_str(), ofstream::out);
	ofile << "#time " << time << endl;
	ofile << "#optR " << goicp.optR << endl;
	ofile << "#optT " << goicp.optT << endl;
	ofile << "#Error " << goicp.optError << endl;
	ofile.close();

}