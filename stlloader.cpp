#include "stlloader.h"

#include <vector>

#include <string>

#include <QtOpenGL/qgl.h>

#include <fstream>
#include <iostream>
using namespace std;
void STLloader::array2vec(char *a, vector <GLfloat>&vec) {
	char x[4] = { a[0], a[1], a[2], a[3] };
	char y[4] = { a[4], a[5],a[6],a[7] };
	char z[4] = { a[8], a[9],a[10],a[11] };

	float xx = *((float*)x);
	float yy = *((float*)y);
	float zz = *((float*)z);
	vec[0] = xx;
	vec[1] = yy;
	vec[2] = zz;

}
void STLloader::set_path(const char * file_path)
{
	path.assign(file_path);
}

void STLloader::set_path(const string file_path) {
	this->path = file_path;
}

void STLloader::load() {
	vertexs.clear();
	normals.clear();
	ifstream myFile(
		this->path.c_str(), ios::in | ios::binary);
	char header_info[80] = "";
	char triangle_counts[4];
	unsigned long nTriLong;
	if (myFile) {
		myFile.read(header_info, 80);
		myFile.read(triangle_counts, 4);
		nTriLong = *((unsigned long*)triangle_counts);
		cout << nTriLong << endl;
		for (unsigned long i = 0; i < nTriLong; i++) {
			char corr[4];
			float _corr = 0.0f;
			//normal vector x, y, z 4 char per float
			for (int j = 0; j < 3; j++) {
				myFile.read(corr, 4);
				_corr = *((float*)corr);
				normals.push_back(_corr);
			}
			//1 triangle = 3 vertex = 12 points/floats = 48 chars
			for (int t = 0; t < 3; t++) {
				for (int v = 0; v < 3; v++) {
					myFile.read(corr, 4);
					_corr = *((float*)corr);
					vertexs.push_back(_corr);
				}

			}
			//redundent bytes
			char space[2];
			myFile.read(space, 2);
		}
		myFile.close();
	}

}

