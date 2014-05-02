#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "interfaceJ.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void intfj_report(interfaceJReportType type, std::vector<std::string> data, void* user_object);

class Node
{
private:
	int _id;
public:
	int argc;
	char **argv;

	Node(int id){_id=id;}
	
	int run() 
	{
		//check argument quantity
		if (argc!= 4)
		{
			std::cout<<"invalid arguments";
			return 0;
		}
		//local vars

		std::string macro; //actualy macro contents
		std::string imageName(argv[1]);
		std::string macroName (argv[2]);
		std::string outdir(argv[3]);
		std::ifstream  macroStream;
		std::stringstream strStream;


		
		//open the macro and convert to string
		//code modified from james parser
		macroStream.open(argv[2]);
		if(!macroStream.is_open())
		{
			std::cout<<"Macro file could not be opened"<<'\n';
			return 0;
		}
		else
		{
			strStream <<macroStream.rdbuf();
			macro = strStream.str();
			macroStream.close();
		}
		std::cout << "Starting\n";
		
		std::string imageJPath ("/usr/local/imagej/Fiji.app/ImageJ-linux64");
		
		
		interfaceJ ij;
		ij.setJobId(100);
		ij.setImagePath(imageName);
		ij.setMacroText(macro);
		ij.setOutputDir(outdir);
		ij.setImageJDir(imageJPath);
		ij.setUserObject(this); // Default = NULL
		ij.setCallback(&intfj_report);
		
		int i = ij.runImageJ();
		//ij.kill();
		
		
		std::cout<<"The runner returned with value : "<<i<<'\n';
		return 0;
	}
	
	void report(interfaceJReportType type, std::vector<std::string> data)
	{
		std::cout << "Node " << _id << ": ";
		if(type == REPORT_PROGRESS)
			std::cout << "REPORT_PROGRESS\n";
		else if(type == REPORT_ERROR)
			std::cout << "REPORT_ERROR\n";
		else if(type == REPORT_FINISHED)
			std::cout << "REPORT_FINISHED\n";
		
		for(int i = 0; i < data.size(); ++i)
		{
			std::cout << data[i] << "\n";
		}
	}
	
};

void intfj_report(interfaceJReportType type, std::vector<std::string> data, void* user_object)
{
	((Node*)user_object)->report(type, data);
}


Node* m_node;

void runIJ()
{
	m_node->run();
}

int main(int argc, char *argv[]) 
{
	Node node (123);
	node.argc = argc;
	node.argv = argv;
	
	m_node = &node;
	
	std::thread t (runIJ);
	t.join();
	
}
