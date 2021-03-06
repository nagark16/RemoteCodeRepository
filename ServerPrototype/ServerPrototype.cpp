/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
//                                                                     //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: RepositoryApp                                          //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////

/*
* Purpose:
* ----------------
*   Package contains one class, Server, that contains a Message-Passing Communication
*  facility. It processes each message by invoking an installed callable object
*  defined by the message's command key.
*
*  Message handling runs on a child thread, so the Server main thread is free to do
*  any necessary background processing (none, so far).
*
*
* Build Process:
* ---------------
* - Required files: ServerPrototype.h, ServerPrototype.cpp, Comm.h, Comm.cpp, IComm.h
*					Message.h, Message.cpp, FileSystem.h, FileSystem.cpp, Utilities.h
* - Compiler command: devenv RepositoryApp.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1.0 : 6th April 2018
*  - first release
*/


#include "ServerPrototype.h"
//#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <chrono>
#include <stdio.h>  /* defines FILENAME_MAX */
#define WINDOWS  /* uncomment this line to use it for windows.*/ 
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using namespace std;


using Msg = MsgPassingCommunication::Message;

//----< Retrieves files at a given location >-------------------------
Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}
//----< Retrieves directory at a given location >-------------------------
Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

//----< Displays the message >-------------------------
template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}

//----< Lambda to reply self message >-------------------------
std::function<Msg(Msg)> echo = [](Msg msg) {
	 Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command(msg.command());
  return reply;
};

//----< Get current directory >------------------------------------------
inline std::string getCurrentWorkingDirectory2(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}
//----< Lambda to reply list of files for a request >-------------------------
std::function<Msg(Msg)> getFiles = [](Msg msg) {
	std::cout << "\n\n Requirement files to demonstrate " << msg.command();
	std::cout << "\n======================================================";
	std::string dir = getCurrentWorkingDirectory2();
	Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command(msg.command());
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
	
	size_t val = dir.find("ServerPrototype");
	if (val == std::string::npos)
		searchPath = path;
	
    Files files = Server::getFiles(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};
//----< Helper function to demonstrate req#4 >-----------
void req7() {
	std::cout << "\n\nDemonstrating requirement #7";
	std::cout << "\n========================================";
	std::cout << "\n Below, I will clearly demonstrate all the requirements without user intervention, with status displayed on status bar, including transmission of files";
}

//----< Lambda to reply about connect request >-------------------------
std::function<Msg(Msg)> connectCheck = [](Msg msg) {
	std::cout << "\n\n Demonstrating connect requirement";
	std::cout << "\n============================================";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());
	return reply;
};
//----< Lambda to reply about viewfile request >-------------------------
std::function<Msg(Msg)> viewFile = [](Msg msg) {
	std::cout << "\n\n Demonstrating view file requirement";
	std::cout << "\n============================================";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());
	reply.file(msg.value("ViewFileName")+":");
	return reply;
};

//----< Lambda to reply about list of directory at a path request >-----------
std::function<Msg(Msg)> getDirs = [](Msg msg) {
	std::cout << "\n\n Retrieving directories to demonstrate " << msg.command();
	std::cout << "\n=====================================================";
	std::string dir = getCurrentWorkingDirectory();
	
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command(msg.command());
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
	size_t val = dir.find("ServerPrototype");
	if (val == std::string::npos)
		searchPath = "codeRepository/remoteRepositoryFiles";

    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};
//----< Helper function to demonstrate req#1 >-----------
void req1() {
	std::cout << "\nDemonstrating requirement #1";
	std::cout << "\n========================================";
	std::cout << "\n\t1. " << typeid(std::function<bool()>).name() 
		<< ", declared in this function, "
		<< "is only valid for C++11 and later versions.";
	std::cout << "\n\t2. As you can see the GUI, which is developed using Windows Presentation foundatin framework";
	std::cout << "\n\t3. The communication between C# GUI and C++ server is done using C++\\CLI";
}

//----< Helper function to demonstrate req#1 >-----------
void req3() {
	std::cout << "\nDemonstrating requirement #3";
	std::cout << "\n========================================";
	std::cout << "\n As it you can see GUI, I have provided option to upload files via CheckIn tab."
		<<" Download files via Checkout tab. View repository contents via browse, view metadata, view files.";
}
//----< Helper function to reply about browse request >-------------------------
Msg Server::browse(Msg msg) {
	std::cout << "\nDemonstrating: Few of client purposes i.e., displaying filenames and categories based on queries of filenames and dependencies";
	std::cout << "\nIn client(GUI) program this can be seen in browse tab under package description";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());

	std::string fileName = msg.value("fileName");
	if (fileName != "")
	{
		std::vector<std::string> files = repo_.browseAFile(fileName);
		int count = 0;
		std::cout << "\nfiles size:" << files.size();
		for (auto descrip : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("description" + countStr, descrip);
		}
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}

	return reply;
}

//----< Lambda to reply about view metadata request >-------------------------
Msg Server::viewMetadata(Msg msg) {
	std::cout << "\n\n Demonstrating view metadata requirement";
	std::cout << "\n================================================";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());
	std::vector<std::string> files = repo_.getMetaData(msg.value("ViewFileName"));
	int count = 0;
	for (auto descrip : files)
	{
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("description" + countStr, descrip);
	}
	
	return reply;
}
//----< Helper function to reply about check out request >-------------------------
Msg Server::checkOut(Msg msg) {
	std::cout << "\n\n Demonstrating checkout requirement";
	std::cout << "\n============================================";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());
	std::string checkOut = msg.value("checkOutFiles");
	std::string isChecked = msg.value("isChecked");
	if (isChecked == "FALSE") {
		std::cout << "\nChecking out only the selected file";
		reply.file(msg.value("checkOutFile")+":");
	}
	else {
		std::cout << "\nChecking out selected file & all its dependencies";
		std::vector<std::string> files = repo_.checkOut(msg.value("checkOutFile"),"");
		std::string filesList;
		for (auto file : files)
			filesList = file + ":" + filesList;
		reply.file(filesList);
	}
	return reply;
}

Msg Server::checkIn(Msg msg) {
	std::cout << "\n\n Copying files for checkin";
	std::cout << "\n============================================";
	std::cout << "\nDemonstrating: Upload one or more source code text files to the Remote Repository, specifying one or more categories with which those files are associated";
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());
	return reply;
}

//----< Helper function to reply about check in request >-------------------------
Msg Server::checkInFiles(Msg msg) {
	std::cout << "\n\n Demonstrating checkin requirement";
	std::cout << "\n============================================";
	std::cout << "\n\t" << msg.value("description");
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());
	std::string path = msg.value("path");
	std::string categories = msg.value("categories");
	std::string name = msg.value("name");
	std::string description = msg.value("description");
	std::string parent = msg.value("parent");
	std::string file = msg.value("files");
	std::string dependencies = msg.value("dependencies");
	std::list<std::string> dependenciesList;
		
	DbElement<PayLoad> elem;
	elem.name(name);
	elem.descrip(description);
	if(dependencies.length() > 0)
		elem.children().push_back(dependencies);
	PayLoad pl;
	pl.value() = "codeRepository\\remoteRepositoryFiles";
	pl.categories().push_back(categories);
	if(msg.value("isClosed").compare("True") == 0)
		pl.isClose() = true;
	else
		pl.isClose() = false;
	elem.payLoad(pl);
	repo_.checkIn(file, elem);
	repo_.displayRepo();
	return reply;
}

//----< Test stub >-------------------------
int main()
{
	req1();
	req3();
	req7();
	std::cout << "\n\nDemonstrating requirement #2 & #3 together";
	std::cout << "\n=============================================";
	std::cout << "\nReq#2. All the communication between client & server done via asynchronous message-passing"
		<< "\n\tcommunication(I have displayed message as request and reply messages)."
		<< "\n\tAlso, file transfer is demonstrated for view file request."
		<< "\n\tThis transferred file is displayed in new ViewFile window.";
	std::cout << "\n\nReq#3. As you can see in GUI I have developed separate tab for each of Connect,CheckIn,CheckOut,Browse,View File and View metadata\n";
	Server server(serverEndPoint, "ServerPrototype");
	server.start();
	
	std::cout << "\n Starting server & doing self test";
	std::cout << "\n======================================";
	server.addMsgProc("echo", echo);
	server.addMsgProc("getFilesForBrowse", getFiles);
	server.addMsgProc("getDirsForBrowse", getDirs);
	server.addMsgProc("getFilesForViewFile", getFiles);
	server.addMsgProc("getDirsForViewFile", getDirs);
	server.addMsgProc("getFilesForViewMetadata", getFiles);
	server.addMsgProc("getDirsForViewMetadata", getDirs);
	server.addMsgProc("getFilesForCheckOut", getFiles);
	server.addMsgProc("getDirsForCheckOut", getDirs);
	server.addMsgProc("connectToServer", connectCheck);
	server.addMsgProc("viewFile", viewFile);
	server.addMsgProc("serverQuit", echo);
	server.processMessages();
	Msg msg(serverEndPoint, serverEndPoint);  // send to self
	msg.name("msgToSelf");
	
	std::cout << "\n  press enter to exit";
	std::cin.get();
	msg.command("serverQuit");
	server.postMessage(msg);
	server.stop();
	return 0;
}

