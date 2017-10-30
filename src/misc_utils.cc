/*****************************************************
Created Oct 2017: Find Political Donors 
This file includes functions to:
- Get log/debug details
- Validate input date format
- tokanizer function to return | separated tokens in input data

***********************************************/

#include <misc_utils.h>

using namespace std;

//declare global log file pointer and log level
ofstream log_file;
int log_level;

/************************************************************
   Function to write log details for debugging
************************************************************/
void logger (string file, string function, int line, string message, int level)
{
    if (level <= log_level) {
        log_file << "[" << file << ":" << function << "():" << line << "]";
        if (level == INFO)
            log_file << "[INFO] ";
        else if (level == ERROR)
            log_file << "[ERROR] ";
        else if (level == WARNING)
            log_file << "[WARNING] ";
        else if (level == DEBUG)
            log_file << "[DEBUG] ";
        log_file << message << endl;
    }
    return;
}

/************************************************************
   Function to extract all fields from the FEC data dictionary 
************************************************************/

char *strtok_parse (char *str, char const *delim)
{
	static char* buffer = NULL;
	char* token = NULL;
	char* ptr   = NULL;
	stringstream msg;

	// initialize static buffer 
	if ( str != NULL )
		buffer = str;

	//if str read completely then return
    if(buffer == NULL) {
        logger (__FILE__, __FUNCTION__, __LINE__, "Finished tokenizing string", DEBUG);
        return NULL;
    }

    msg << "Starting to tokenize string: " << buffer;
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");

	if ( buffer == NULL )
		return NULL;

	//point to beginning of buffer string
	token = buffer;
	
	// find the delim (|) in the buffer
	if ((ptr = strpbrk ( buffer, delim )) != NULL ) {
		*ptr = '\0';
		buffer = ++ptr;
	}
	else {
		//at end of str, no more delim found	
		buffer = NULL;
	}
	
	return token;

}
		  
/************************************************************
	Function to check if the date provided in input file is valid
	MMDDYYYY
************************************************************/
bool isValid( string date) {
	
	stringstream msg;

	if ( ( date == "" ) || date.size() != 8 ) {
    	msg << "Transaction date is invalid";
    	logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), WARNING);
    	msg.str("");
		return false;
	}

	//Check if all 8 chars are digits
	for ( string::iterator it=date.begin(); it!=date.end(); it++)
	{
		if (!isdigit(*it)) {
			logger (__FILE__, __FUNCTION__, __LINE__, "Date "+date+"is not a number", WARNING);
			return false;
		}
	}

	//Need current date to get upper bound for MM DD YYYY
    time_t rawtime;
    struct tm * timeinfo;
    char m[3];
    char d[3];
    char y[5];

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (m,3,"%m",timeinfo);
    int curr_month = stoi ((const char *)m, NULL, 10);
    strftime (d,3,"%d",timeinfo);
    long curr_day = strtol ((const char *)d, NULL, 10);
    strftime (y,5,"%Y",timeinfo);
    long curr_yr = strtol ((const char *)y, NULL, 10);

	//Validate MMDDYYYY format
	string month = date.substr(0,2);
	int  mm      = stoi(month.c_str(), NULL, 10);
	string day   = date.substr(2,2);
	int  dd      = stoi(day.c_str(), NULL, 10);
	string year  = date.substr(4,4);
	int  yyyy    = stoi(year.c_str(), NULL, 10);

	//check boundary
	if ((mm <=0 || mm > 12 ) ||
		(dd <= 0 || dd > 31 ) ||
		(yyyy <= 0 || yyyy > curr_yr ) ) {
	return false;
	}
	
	//date cannot be a future date
	if ( yyyy == curr_yr ) {
		 if ( mm > curr_month ) {
			return false;
		}
		else if ( mm == curr_month && dd > curr_day ) {
			return false;
		}
	}	

	//Day cannot be 29(for non leap year) or 30/31 in Feb
	if ( mm == 2 ) {
		if ( yyyy%4 == 0 ) {
		 	if ( dd > 29 ) {
				return false;
			}
		}
		else {
			if ( dd > 28) {
				return false;
		}
		}
	}	

	return true;
}

