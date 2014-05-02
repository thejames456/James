#include "imageJRunner.h"
#include <iostream>
#include <string>

char* copyString(std::string str)
{
	const char* src = str.c_str();
	char* dst = new char[str.size() + 1];
	for(int i = 0; i < str.size(); i++)
	{
		dst[i] = src[i];
	}
	dst[str.size()] = 0;
	return dst;
}

imageJRunner::imageJRunner()
{
	useIntfJ = false;
	imageJPath = std::string("/usr/local/imagej/Fiji.app/ImageJ-linux64");
}

void imageJRunner::setImageJPath(std::string path)
{
	imageJPath = std::string(path);
}

void imageJRunner::enableInterfaceJ()
{
	useIntfJ = true;
}
void imageJRunner::setInputImage(std::string file)
{
	inputImage = file;
}
void imageJRunner::setOutputPath(std::string path)
{
	outputPath = path;
}
void imageJRunner::setFifoPath(std::string path)
{
	fifoPath = path;
}
void imageJRunner::setPort(int p)
{
	port = p;
}

int imageJRunner::runImageJWithMacro(std::string macro, bool xvfb)
{
	std::vector<char*> args;
      	std::cout << "ImageJ Path: " << imageJPath << "\n";
	char* cmdPath;
	if(xvfb)
	{
		cmdPath = "/usr/bin/xvfb-run";
		args.push_back("-a");
		
		args.push_back("-n");
		std::string p;
		p += port;
		args.push_back(copyString(p));
		
		args.push_back(copyString(imageJPath));
	}
	else
	{
		cmdPath = copyString(imageJPath);
		args.push_back("--headless");
	}
    args.push_back((char*)macro.c_str());
	
	if(useIntfJ)
	{
		args.push_back("intfj:enable");
		
		std::string imageArg ("intfj:image=");
		imageArg += inputImage;
		args.push_back(copyString(imageArg));
		
		std::string outputArg ("intfj:outputdir=");
		outputArg += outputPath;
		args.push_back(copyString(outputArg));
		
		std::string fifoArg ("intfj:fifo=");
		fifoArg += fifoPath;
		args.push_back(copyString(fifoArg));
	}

    	int result = proc.start(cmdPath, args, NULL);
	if(result != 0)
		return result;
	return 0;
}

std::string imageJRunner::readOutputLine()
{
	return proc.getLine();
}
std::string imageJRunner::readOutputToEnd()
{
	std::string text;
	while(!proc.eof())
	{
		text += proc.getLine() + "\n";
	}
	return text;
}
void imageJRunner::readOutputToStdout()
{
	while(!proc.eof())
        {
                std::cout << proc.getLine() + "\n";
        }
}

bool imageJRunner::eof()
{
	return proc.eof();
}

int imageJRunner::waitForExit()
{
	return proc.waitForExit();
}
int imageJRunner::getExitCode()
{
	return proc.waitForExit();
}
bool imageJRunner::hasExited()
{
	return proc.hasExited();
}

void imageJRunner::kill_children()
{
	proc.kill_the_child();
}

