//MyPath.h
//20190704  V0
#ifndef PATH_H_INCLUDE
#define PATH_H_INCLUDE

#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<sstream>

#if defined(WIN32) //Windows
	#include<io.h>
	#include<direct.h>
#else //Linux
	#include<unistd.h>
	#include<sys/stat.h>
	#include<dirent.h>
#endif

class Path {
protected:
	std::string dirName;//must end with Path::separator
	std::string fileName;
public:
	//static member
	static std::string separator;
	static std::string pathSeparator;
	static char separatorChar;
	static char pathSeparatorChar;
	static std::string currentDirName;
	static std::string parentDirName;
	static std::string currentDirNameStr;
	static std::string parentDirNameStr;
	//static function
	static void list(const Path&p, std::vector<Path>&dst, bool list_file, bool list_dir, bool is_recur);
	static int split(std::string str, std::vector<std::string>&dst, std::string sep, bool alw_null = false);
	static int split(std::string str, std::vector<std::string>&dst);
	static Path getCurrentDir();//get current directory for system, in absolute path
	static void setCurrentDir(const Path&p);//back to original current directory for system
	//con-set-get
	//**construct
	Path(){};
	Path(std::string _dirName, std::string _fileName) { this->set(_dirName, _fileName); };
	Path(std::string _pathName) { this->set(_pathName); };
	Path(const Path&_file, std::string _fileName) { this->set(_file, _fileName); };
	Path(const Path&_file) { this->set(_file); };
	//**set
	void set(std::string _dirName, std::string _fileName);//_dirName+_fileName
	void set(std::string _pathName);//_pathName-->dirName+fileName
	void set(const Path&_file, std::string _fileName);//=_file.dirName+_fileName
	void set(const Path&_file);//_file
	void setDirName(std::string _dirName);//_dirName
	void setFileName(std::string _fileName);//_fileName
	//*get
	const std::string getDirName()const { return this->dirName; };
	const std::string getFileName()const { return this->fileName; };
	const std::string getPathName()const { return this->dirName + this->fileName; };
	const std::string getCurrentDirName()const{ return this->toCurrentDir().dirName; };
	const std::string getParentDirName()const{ return this->toParentDir().dirName; };
	friend std::ostream& operator<< (std::ostream&os, const Path&p) {
		using namespace std;
		os<<setiosflags(ios::left)
			<<setw(50)<<p.getPathName()<<"  "
			<<resetiosflags(ios::left);
		return os;
	}
	
	//status judge
	bool isFile()const { return this->dirName.empty() && !this->fileName.empty(); };
	bool isDirectory()const { return !this->dirName.empty() && this->fileName.empty(); };
	bool isEmptyDir()const { return this->dirName.empty(); };
	bool isCurrentDir()const { return this->dirName == Path::currentDirName; };
	bool isParentDir()const { return this->dirName == Path::parentDirName;};
	bool isEqualDir(const Path&p)const { return this->dirName == p.dirName; };
	
	//calculation
	void list(std::vector<Path>&dst, bool list_file = true, bool list_dir = true, bool is_recur = false)const;//list all files and directories
	void listFiles(std::vector<Path>&dst, bool is_recur = false)const;//list all files
	void listDirs(std::vector<Path>&dst, bool is_recur = false)const;//list all directories
	void listRecur(std::vector<Path>&dst, bool list_file = true, bool list_dir = true)const;//list all files and directories Recursively
	void listFilesRecur(std::vector<Path>&dst)const;//list all files Recursively
	void listDirsRecur(std::vector<Path>&dst)const;//list all directories Recursively
	
	//transform
	Path toAbsolutePath()const;//transform to absolute path
	Path toParentDir()const;//transform to parent directory, in absolute path
	Path toCurrentDir()const;//transform to current directory, in absolute path

	
};





#endif//PATH_H_INCLUDE
//~