#pragma once
#include <string>
#include "Matrix.h"
using std::string;

class FileUtils {
public:
	static bool CompareFiles(string fileName1, string fileName2);
	static Matrix ReadFileAsMatrix(string filePath);
};
