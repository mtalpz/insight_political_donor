/*****************************************************
Created Oct 2017: Find Political Donors 
***********************************************/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <math.h>
#include <time.h>

using namespace std;

//Path for input and output files
const char* INPUT_FILE = "../input/itcont.txt";
const char* OUTPUT_ZIP_FILE  = "../output/medianvals_by_zip.txt";
const char* OUTPUT_DATE_FILE = "../output/medianvals_by_date.txt";

const char* LOG_FILE = "logs/find-political_donors.log";

//Indexes for required fields based on the data dictionary
int CMTE_ID_POS         = 0;
int ZIP_CODE_POS        = 10;
int TRANSACTION_DT_POS  = 13;
int TRANSACTION_AMT_POS = 14;
int OTHER_ID_POS        = 15;

//Struct to hold information for every record
typedef struct data 
{
	int total_num_transactions;
	float total_amount_contributions;
	deque<float> max_heap;
	deque<float> min_heap;
	int median;
}record;


/***************************************************************************
 All Functions:
***************************************************************************/
//Function to parse the input file and write the corresponding requested information to output medianvals_by_zip.txt
void parse_input_and_write_zip ( char* line, ofstream *out);

//Function to process data from input txt file for both zip and data based on key.
void process_recipient_data(map <string, map <string, record> > *recipient_record,
                             string recipient,string key, record *new_rec);

//Function to write to output medianvals_date.txt after entire input file is parsed and median_date_record is populated.
void write_output_date_file(ofstream *out);

//Function to insert new amount in min or max heap based on the status of two heaps and setup parameters for heap operations
void insert_to_balance_heap (deque<float> *max_heap, deque<float> *min_heap, float num);
