#ifndef	__UTILITIES_H__
#define __UTILITIES_H__ 

#include <sys/stat.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <vector>

using namespace std;

void createDirAndSubDir(string path);
void parseLine(string line, vector<string>& lineV, vector<string> delimiter);
string LCSubstr(const string& x, const string& y);
#endif
