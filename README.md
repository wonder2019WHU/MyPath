# Introduction: MyPath

## Overview
This code provides **cross-platform** file path class. The class `Path` can **describe** and **list** the file path name under both `Windows` and `Linux`.

## Construction/set
+ Support for automatic recognition of separators for different operating systems
+ Support for automatic spliting of directory and file
+ Support for automatic completion of separator at the end of directory.
+ Support for both absolute and relative directory.

**1.** **Name of directory and file**
```C++
//if in linux system
string dirName1 = "/home/Administrator/"
string dirName2 = "/usr/local"
string dirName3 = "."
string fileName = "data.txt"
Path p1(dirName1, fileName);//"home/Administrator/data.txt"
Path p2(dirName2, fileName);//"usr/local/data.txt"
Path p3(dirName3, fileName);//"./data.txt"
```

```C++
//if in windows system
string dirName1 = "E:\\workspace\\"
string dirName2 = "C:\\User"
string dirName3 = ".\\photo"
string fileName = "data.txt"
Path p1(dirName1, fileName);//"E:\\workspace\\data.txt"
Path p2(dirName2, fileName);//"C:\\User\\data.txt"
Path p3(dirName3, fileName);//".\\photo\\data.txt"
```

**2.** **Name of path**
```C++
//if in linux system
string pathName1 = "/home/Administrator/data.txt"
string pathName2 = "/home/Administrator/"
string pathName3 = "data.txt"
string pathName4 = "./data.txt"
Path p1(pathName1);//"/home/Administrator/", "data.txt" 
Path p2(pathName2);//"/home/Administrator/", "" 
Path p3(pathName3);//"", "data.txt" 
Path p4(pathName4);//"./", "data.txt" 
```

```C++
//if in windows system
string pathName1 = "E:\\workspace\\data.txt"
string pathName2 = "E:\\workspace\\"
string pathName3 = "data.txt"
string pathName4 = ".\\data.txt"
Path p1(pathName1);//"E:\\workspace\\", "data.txt" 
Path p2(pathName2);//"E:\\workspace\\", "" 
Path p3(pathName3);//"", "data.txt" 
Path p4(pathName4);//".\\", "data.txt" 
```

**2.** **Class of path and Name of file**
```C++
//if in linux system
Path p1("/home/Administrator/data1.txt");
Path p2(p1, "data2.txt");//"/home/Administrator/data2.txt"
```

```C++
//if in windows system
Path p1("E:\\workspace\\data1.txt");
Path p2(p1, "data2.txt");//"E:\\workspace\\data2.txt"
```

## List your file system
+ Support the same interface in `Windows` and `Linux` system
+ Support to list directories and files
+ Support to list recursively
+ Support to list directories only or files only

**1.** **`list`**: list directories and files, not traverse sub-directories
**2.** **`listDirs`**: list directories, not traverse sub-directories
**3.** **`listFiles`**: list files, not traverse sub-directories
**4.** **`listRecur`**: list directories and files, traverse all sub-directories
**5.** **`listDirsRecur`**: list directories, traverse all sub-directories
**6.** **`listFilesRecur`**: list files, traverse all sub-directories

## Access directory hierarchy
+ Support the same interface in `Windows` and `Linux` system
+ Support to get and set current directory of process
+ Support to get current and parent directory of class `Path`
+ Support to change relative directory to absolute directory

**1.** **`Path::getCurrentDir`**: get current directory of process (static function)
**2.** **`Path::setCurrentDir`**: set current directory of process (static function)
**3.** **`toCurrentDir`**: get current directory of class `Path`
**4.** **`toParentDir`**: get parent directory of class `Path`
**5.** **`toAbsolutePath`**: change relative directory to absolute directory

