//MyPath.h
//20190704  V0

#include "MyPath.h"
using namespace std;

#if defined(WIN32)  //Windows
	//static member
	string Path::separator = "\\";
	string Path::pathSeparator = ";";
	char Path::separatorChar = '\\';
	char Path::pathSeparatorChar = ';';
	string Path::currentDirName = ".\\";
	string Path::parentDirName = "..\\";
	//static function
	void Path::list(const Path&p, std::vector<Path>&dst, bool list_file, bool list_dir, bool is_recur) {
		Path src;
		if(p.dirName.empty())return;
		
		string _dir = p.dirName + "*.*"; //add wildcard for search first file
		const char *dir = _dir.c_str();
		
		_finddata_t findData;
		string filename;
		intptr_t handle = _findfirst(dir, &findData);
		
		if (handle == -1)return;
		do {
			filename = string(findData.name);
			if(findData.attrib == _A_SUBDIR) {//Find sub-directory
				if(filename == Path::currentDirNameStr 
				|| filename == Path::parentDirNameStr)//skip the '.' and '..' directory
					continue;
				if(list_dir)
					dst.push_back(Path(p.dirName+filename, ""));
				if(is_recur) {
					src.dirName = p.dirName + filename + Path::separator;
					Path::list(src, dst, list_file, list_dir, is_recur);
				}
			}
			if (findData.attrib != _A_SUBDIR && list_file) //if regular file and include file
				dst.push_back(Path(p, filename));  
			}while(_findnext(handle, &findData) != -1);
			
		_findclose(handle);
		
	};//list all files and directories
	
#else //Linux
	//static member
	string Path::separator = "/";
	string Path::pathSeparator = ":";
	char Path::separatorChar = '/';
	char Path::pathSeparatorChar = ':';
	string Path::currentDirName = "./";
	string Path::parentDirName = "../";
	//static function
	void Path::list(const Path&p, std::vector<Path>&dst, bool list_file, bool list_dir, bool is_recur) {
		//p must be absolute path
		Path src;
		//get the dir by char*
		if(p.dirName.empty())return;
		const char *dir = p.dirName.c_str();
		//open the dir
		DIR *dp;		
		if ((dp = opendir(dir)) == NULL)return;
		//read the fir
		struct dirent *entry;
		struct stat statbuf;
		string filename;

		chdir(dir);//dir becomes current directory	
		while ((entry = readdir(dp)) != NULL) {
			lstat(entry->d_name, &statbuf);
			filename = string(entry->d_name);
			if (S_ISDIR(statbuf.st_mode)) { // if diretory and include diretory
				if (filename == Path::currentDirNameStr 
				 || filename == Path::parentDirNameStr) //skip "." and ".."
					continue;
				if(list_dir)
					dst.push_back(Path(p.dirName+filename, ""));
				if(is_recur) {
					src.dirName = p.dirName + filename + Path::separator;
					Path::list(src, dst, list_file, list_dir, is_recur);
				}
			}
			if (S_ISREG(statbuf.st_mode) && list_file) //if regular file and include file
				dst.push_back(Path(p, filename));
		}
		chdir("..");
		//close the dir
		closedir(dp);
	};
	
#endif

//class Path
//static member
string Path::currentDirNameStr = ".";
string Path::parentDirNameStr = "..";
//static function
int Path::split(string str, vector<string>&dst, string sep, bool alw_null) {
	if(str.empty())//text is empty
		return 0;
	if(sep.empty()) {//separator is empty
		dst.push_back(str);
		return 1;
	}
	str = str + sep;//make the pos not be -1, tail is like a guard
	int pos = str.find_first_of(sep);
	while(pos != -1) {
		if(alw_null || pos != 0)
			dst.push_back(str.substr(0, pos));

		str = str.substr(pos + sep.size());
		pos = str.find_first_of(sep);
	}
	return dst.size();
}

int Path::split(std::string str, std::vector<std::string>&dst) {
	stringstream ss;
	ss << str;
	string strs;
	while(ss >> strs)
		dst.push_back(strs);
	return dst.size();
}

Path Path::getCurrentDir() {
	char buf[1024];
	getcwd(buf, 1024);
	return Path(buf, "");
};//get current directory, in absolute path

void Path::setCurrentDir(const Path&p) {
	const char *cur = p.dirName.c_str();
	chdir(cur);
};//set current directory

//con-set-get
//**set
void Path::set(std::string _dirName, std::string _fileName) {
	this->dirName = (_dirName.empty() || _dirName[_dirName.size()-1] == Path::separatorChar)?
		(_dirName):(_dirName + Path::separator); 
	this->fileName = _fileName;
};//_dirName+_fileName

void Path::set(std::string _pathName) {
	int pos = _pathName.find_last_of(Path::separatorChar);
	this->dirName = (pos == string::npos)?
		(""):(_pathName.substr(0, pos+1));
	this->fileName = (pos == string::npos)?
		(_pathName):(_pathName.substr(pos+1));
};//_pathName-->dirName+fileName

void Path::set(const Path&_file, std::string _fileName) {
	this->dirName = _file.dirName;
	this->fileName = _fileName;
};//=_file.dirName+_fileName

void Path::set(const Path&_file) {
	this->dirName = _file.dirName;
	this->fileName = _file.fileName;
};//_file

void Path::setDirName(std::string _dirName) {
	this->dirName = (_dirName.empty() || _dirName[_dirName.size()-1] == Path::separatorChar)?
		(_dirName):(_dirName + Path::separator); 
};//_dirName

void Path::setFileName(std::string _fileName) {
	this->fileName = _fileName;
};//_fileName

//calculation
void Path::list(std::vector<Path>&dst, bool list_file, bool list_dir, bool is_recur)const {
	Path curDir(Path::getCurrentDir());
	Path p(this->toAbsolutePath());
	Path::list(p, dst, list_file, list_dir, is_recur);
	Path::setCurrentDir(curDir);
};//list all files and directories
void Path::listFiles(std::vector<Path>&dst, bool is_recur)const {
	Path curDir(Path::getCurrentDir());
	Path p(this->toAbsolutePath());
	Path::list(p, dst, true, false, is_recur);
	Path::setCurrentDir(curDir);
};//list all files
	
void Path::listDirs(std::vector<Path>&dst, bool is_recur)const {
	Path curDir(Path::getCurrentDir());
	Path p(this->toAbsolutePath());
	Path::list(p, dst, false, true, is_recur);
	Path::setCurrentDir(curDir);
};//list all files
	
void Path::listRecur(std::vector<Path>&dst, bool list_file, bool list_dir)const {
	Path curDir(Path::getCurrentDir());
	Path p(this->toAbsolutePath());
	Path::list(p, dst, list_file, list_dir, true);
	Path::setCurrentDir(curDir);
};//list all files and directories Recursively

void Path::listFilesRecur(std::vector<Path>&dst)const {
	Path curDir(Path::getCurrentDir());
	Path p(this->toAbsolutePath());
	Path::list(p, dst, true, false, true);
	Path::setCurrentDir(curDir);
};//list all files Recursively

void Path::listDirsRecur(std::vector<Path>&dst)const {
	Path curDir(Path::getCurrentDir());
	Path p(this->toAbsolutePath());
	Path::list(p, dst, false, true, true);
	Path::setCurrentDir(curDir);
};//list all directories Recursively

//transform
Path Path::toAbsolutePath()const {
	//empty directory
	if(this->dirName.empty())return Path();
	//save original current directory
	Path curDir(Path::getCurrentDir());
	//change to defined directory
	const char *dir = this->dirName.c_str();
	chdir(dir);//dir becomes current directory
	Path dst(Path::getCurrentDir());
	//back to original directory
	Path::setCurrentDir(curDir);
	return Path(dst, this->fileName);
};//transform to absolute path

Path Path::toParentDir()const {
	//empty directory
	if(this->dirName.empty())return Path();
	//save original current directory
	Path curDir(Path::getCurrentDir());
	//change to defined directory
	const char *dir = this->dirName.c_str();
	chdir(dir);//dir becomes current directory
	chdir("..");//dir becomes parent directory
	Path dst(Path::getCurrentDir());
	//back to original directory
	Path::setCurrentDir(curDir);
	return dst;
};//transform to parent directory, in absolute path

Path Path::toCurrentDir()const {
	//empty directory
	if(this->dirName.empty())return Path();
	//save original current directory
	Path curDir(Path::getCurrentDir());
	//change to defined directory
	const char *dir = this->dirName.c_str();
	chdir(dir);//dir becomes current directory
	Path dst(Path::getCurrentDir());
	//back to original directory
	Path::setCurrentDir(curDir);
	return dst;
};//transform to current directory, in absolute path








