#ifndef ICP_H
#define ICP_H

#include "GoICP\jly_goicp.h"
#include <vector>
#include <string>
class ICP
{
public:
	ICP();
	~ICP();
	void load_points(const std::vector<float> &vModel, const std::vector<float> &vData);
	void readConfig(string FName);
	void run();
	GoICP goicp;
	POINT3D * pModel, * pData;

	int Nm, Nd;

};


#endif //ICP_H