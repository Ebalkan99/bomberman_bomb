//WRITTEN BY SULEYMAN
#include "ImageLoad.h"
#include <string>
using namespace std;

void ImageLoad::load(CHMAT& matrix, string path) {
	strcpy_s(file_path, path.c_str());
	LoadImage(file_path, matrix);

};