#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

int getdir (std::string dir, std::vector<std::string> &files, std::string ext);

#endif
