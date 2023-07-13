//WRITTEN BY SULEYMAN
#pragma once
#include <string>
#include "chmat.h"
using namespace std;

class ImageLoad {
public:
	char file_path[MAX_PATH];
	void load(CHMAT& matrix, string path);
};