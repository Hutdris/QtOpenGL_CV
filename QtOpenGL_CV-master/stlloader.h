#ifndef STLLOADER_H
#define STLLOADER_H

#include <vector>
#include <string>
#include <QtOpenGL/qgl.h>

using namespace std;
class STLloader
{
public:
	void set_path(const char* file_path);
	void set_path(const std::string file_path);
	void load();
	void array2vec(char *a, vector <GLfloat>&vec);
	vector<GLfloat> get_vertexs() { return vertexs; };
	vector<GLfloat> get_normals() { return normals; };
	GLuint size;
	STLloader() :size(0) {}
	STLloader(const char* file_path) { path.assign(file_path); }
	STLloader(std::string file_path) { this->set_path(file_path); }
	~STLloader() { this->vertexs.clear(); this->normals.clear(); }

private:
	std::vector<GLfloat> vertexs; //{x0, y0, z0, x1, y1, z1,...}
	std::string path;
	std::vector<GLfloat> normals; //{n0x, n0y, n0z, n1x, ...}
};

#endif // STLLOADER_H
