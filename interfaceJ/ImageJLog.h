// ImageJLog
// a class to handle the standard output of Running ImageJ through commandline
// ImageJLog(): Constructor for the imageJLog class
// precondition(): The process of running imageJ has been written as output.txt
// postcondition(): output.txt has been written into the string output_file
// ~ImageJLog(): destructor for the ImageJLog class
// precondition(): values for logs and output file have been set
// postcondition(): those values have been destroyed
// ProcessOutput(): goes through and sorts the string for errors,warnings and everything else
// precondition(): value for output_file has been set
// postcondition: values for errorLog,warningLog and elseLog have been set
//WriteLogs(): writes the log string variables into text files
#include <string>
#include <fstream>

#ifndef McFarland_ImageJLog
#define McFarland_ImageJLog

class ImageJLog
{
public:
ImageJLog();
~ImageJLog();
void GetFile(std::string filename);
void ProcessOutput();
void WriteLogs();





std::ifstream in_s;
std::string errorLog;
std::string warningLog;
std::string elseLog;

};

#endif
