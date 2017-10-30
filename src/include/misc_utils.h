/**********************************************
Created Oct 2017: Find Political Donors 
***********************************************/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

//log details as chosen
enum LOG_LEVEL {
	ERROR = 0,
	WARNING = 1,
	INFO = 3,
	DEBUG = 4
};

/***********************************************
 All Functions:
************************************************/

//Function to write log details for debugging
void logger (string file, string function, int line, string message, int level);

//Function to extract all fields from the FEC data dictionary 
char *strtok_parse (char *str, char const *delim);

//Function to check if the date provided in input file is valid
bool isValid( string date );
