#include "stlloader.h"

#include <vector>

#include <string>

#include <QtOpenGL/qgl.h>

#include <fstream>
#include <iostream>
using namespace std;

void STLloader::set_path(const char * file_path)
{
	path.assign(file_path);
}

void STLloader::set_path(const string file_path){
    this->path = file_path;
}

void STLloader::load(){
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
            char triangle[50] = "";
            myFile.read(triangle, 50);
            for (int j = 0; j < 12; j++)
                this->normals.push_back(triangle[j]);

            for (int v = 0; v < 3; v++) {
                for (int k = 0; k < 12; k++)
                    this->vertexs.push_back(triangle[12 * v + k + 12]);
            }
        }
        myFile.close();
    }

}

