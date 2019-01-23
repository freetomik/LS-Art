#include "util.hpp"

using namespace std;

// function getdir, author jtshaw
// https://www.linuxquestions.org/questions/programming-9/
// c-list-files-in-directory-379323/#post1932785
int getdir (string dir, vector<string> &files, string ext)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
      return errno;

    string filename;
    while ((dirp = readdir(dp)) != NULL) {
      string filename(dirp->d_name);
      if(ext == "*" || filename.substr(filename.find_last_of(".") + 1) == ext)
        files.push_back(filename);
    }
    closedir(dp);
    return 0;
}
