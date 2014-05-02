#include "ImageJLog.h"
#include <sstream>
#include <fstream>
#include <iostream>

//===========================================================================================
//opens a file stream looking for a file named output.txt and puts output.txt into the string
//output_file
ImageJLog::ImageJLog()
{
 

std::string filename;
filename="output2.txt";

in_s.open(filename.c_str());
if(in_s.fail())
{
	std::cout<<"input file opening failed "<<std::endl;

}
else
{
	std::cout<<"input file opening success" <<std::endl;

}

}
//===========================================================================================
//deletes all private variables; output_file,errorLog,warningLog,elseLog 
ImageJLog::~ImageJLog()
{
//delete errorLog;
//delete warningLog;
//delete elseLog;
//delete in_s;
}
//============================================================================================
void ImageJLog::GetFile(std::string filename)
{
	in_s.close();
	std::string openFile=filename;
	
	in_s.open(openFile.c_str());
	if(in_s.fail())
	{
		std::cout<<"input file opening failed "<<std::endl;
		
	}
	
	else
	{
		std::cout<<"input file opening success" <<std::endl;
	}
	
	

}

//===========================================================================================
//reads output_file line by line searches for words error,warning, and places them into
//the appropriate string variable
void ImageJLog::ProcessOutput()
{
int finder;
std::string temp;
std::string find_warning="WARNING";
std::string find_error="ERROR";
	while(getline(in_s,temp))
	{
		finder=0;
		//go through string and find the warning and error statements
		//std::getline(in_s,temp);
		
		std::size_t found=temp.find(find_warning);
		if(found!=std::string::npos)
		{	
			finder=1;
			std::cout<<"finder is "<<finder<<std::endl;
			warningLog.append(temp);
		}
		found=temp.find(find_error);
		 if(found!=std::string::npos)
		{
			std::cout<<"finder is "<<finder<<std::endl;
			finder=2;
			errorLog.append(temp);
		}
		else
		{
			elseLog.append(temp);
			finder=3;
		}	
		
			//write to the string files
		
		
	
	}
	
}
//===========================================================================================
//writes the values of the string logs into text files. 
void ImageJLog::WriteLogs()
{
	std::string error="outdir/ErrorLog.txt";
	std::string warning="outdir/WarningLog.txt";
	std::string e_log="outdir/ElseLog.txt";
	
	std::ofstream out_s;
	
	out_s.open(error.c_str());
	if(out_s.fail())
	{
		std::cout<<"error log file opening failed"<<std::endl;
		
	}
	out_s<<errorLog;
	out_s.close();
	
	out_s.open(warning.c_str());
	if(out_s.fail())
	{
		std::cout<<"warning log file opening failed"<<std::endl;
	}
	out_s<<warningLog;
	out_s.close();
	
	
	out_s.open(e_log.c_str());
	if(out_s.fail())
	{
		std::cout<<"else log file opening failed"<<std::endl;
	}
	out_s<<elseLog;
	out_s.close();
		
	
		
	
	
}
//===========================================================================================
