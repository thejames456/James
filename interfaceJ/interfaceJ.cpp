#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "interfaceJ.h"
#include "imageJRunner.h"
#include "fifoClass.h"
#include "ImageJLog.h"

interfaceJ::interfaceJ()
{
	macroComplete = false;
	
	imageJPath = std::string("");
	user = NULL;
	callback = NULL;
}


void interfaceJ::setJobId(std::string id)
{
	jobId = id;
}
void interfaceJ::setImagePath(std::string path)
{
	image = path;
}
void interfaceJ::setMacroText(std::string text)
{
	macro = text;
}
void interfaceJ::setOutputDir(std::string path)
{
	outputdir = path;
}
void interfaceJ::setImageJDir(std::string path)
{
	imageJPath = path;
}
void interfaceJ::setUserObject(void* obj)
{
	user = obj;
}
void interfaceJ::setCallback(report_callback func)
{
	callback = func;
}



int interfaceJ::runImageJ()
{
	//Prepare variables
	
	//Outputdir must end with /
	if(outputdir[outputdir.size() - 1] != '/')
		outputdir += "/";
	
	//prepend //InterfaceJ Macro to macro
	macro = std::string("//InterfaceJ Macro\n") + macro;
	
	//write macro to file
	std::string macroFilePath;
	macroFilePath += outputdir + "macro.ijm";
	std::ofstream macroFile (macroFilePath.c_str());
	macroFile << macro;
	macroFile.close();
	
	//setup fifo
	fifo = fifoClass(this, jobId, outputdir);
	
	//setup imageJRunner
	imageJ.setImageJPath(imageJPath);
	
	imageJ.enableInterfaceJ();
	imageJ.setInputImage(image);
	imageJ.setOutputPath(outputdir);
	imageJ.setFifoPath(fifo.getPath());
	
	//ex
	int port = 0;
	for(int i = 0; i < jobId.size(); i++)
	{
		port += jobId[i];
	}
	port = port % 100 + 100;
	std::cout << "Port: " << port << "\n";
	imageJ.setPort(port);
	
	//debug
	std::cout << "jobid: " << jobId << "\n";
	std::cout << "macro: " << macroFilePath << "\n";
	std::cout << "image: " << image << "\n";
	std::cout << "outdir: " << outputdir << "\n";
	
	//run imageJ
	std::cout << "Running ImageJ\n";
	if(imageJ.runImageJWithMacro(macroFilePath, false) != 0)
	{
		std::cout << "ImageJ failed to run";
		errorMsg = "InterfaceJ: ImageJ failed to run";
                std::vector<std::string> list;
              	list.push_back(errorMsg);

               	callback(REPORT_ERROR, list, user);

		return 1;
	}
	else
		std::cout << "ImageJ started\n";

	//M
	//imageJ.readOutputToStdout();

	//read from fifo
	fifo.readReports();
	
	//imageJ exit
	int exitCode = imageJ.getExitCode();
	std::cout << "ImageJ exited";
	//process stdout
	
	std::string outputFileName;
	outputFileName += outputdir + "stdout.txt";
	std::ofstream outputFile (outputFileName.c_str());
	
	outputFile << imageJ.readOutputToEnd();
	ImageJLog Logger;
	Logger.GetFile(outputFileName);
	Logger.ProcessOutput();
	Logger.WriteLogs();

	
	outputFile.close();

	
	//stdoutReader.read(imageJ, outputdir);
	
	
	//report finish
	
	if(callback != NULL)
	{
		if(errorMsg != "")
		{
			std::vector<std::string> list;
			list.push_back(errorMsg);
			
			callback(REPORT_ERROR, list, user);
		}
		else if(!macroComplete)
		{
			errorMsg = "InterfaceJ: ImageJ terminated unexpectedly";
			std::vector<std::string> list;
			list.push_back(errorMsg);
			
			callback(REPORT_ERROR, list, user);
		}
		else
		{
			callback(REPORT_FINISHED, savedFiles, user);
		}
	}
	
	//done
	return exitCode;
}
int interfaceJ::kill()
{
	imageJ.kill_children();
	return 0;
}


void interfaceJ::reportProgress(int percent)
{
	if(percent == 255)
	{
		macroComplete = true;
	}
	else if(callback != NULL)
	{
		char number [4];
		if(percent > 100)
			percent = 100;
		else if(percent < 0)
			percent = 0;
		sprintf(number, "%d", percent);
		std::vector<std::string> list;
		std::string item (number);
		list.push_back(item);
		
		callback(REPORT_PROGRESS, list, user);
	}
}
void interfaceJ::reportSavedFile(std::string file)
{
	savedFiles.push_back(file);
}
void interfaceJ::reportError(std::string message)
{
	errorMsg = message;
	/*
	if(callback != NULL)
	{
		std::vector<std::string> list;
		list.push_back(message);
		
		callback(REPORT_ERROR, list, user);
	}
	*/
}
void interfaceJ::reportFinished()
{
	/*
	if(callback != NULL)
	{
		callback(REPORT_FINISHED, savedFiles, user);
	}
	*/
}

