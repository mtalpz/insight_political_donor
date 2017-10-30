/*****************************************************
Created Oct 2017: Find Political Donors 
This file has functions to parse the input file and then
populate required details in each recipient record for zip and date.

The zip record is used to write to output zip data file as we parse/read each line.
The output data file is written in the end after entire input file is read and recipient date record has been populated as results are cumulative and non repeated.

Will output files:
	medianvals_by_zip.txt
	medianvals_by_date.txt

//chcek max size of input line ? if > 1024??
// if lines in between 2 records in input file
//if trans amount number(14,2) float or integer, is it rounded in final output?
//parse-strtok, why preincrement?
// check input same recipient , different zip
// check input diff recipient , same zip
//get transaction amount (it could be of type float 14,2) as specified, convert to int?
***********************************************/

#include <find_political_donors.h>
#include <heap_utils.h>
#include <misc_utils.h>

using namespace std;

//log file details
extern ofstream log_file;
extern int log_level;

//map of recipient to zip, to record
map <string, map <string, record> > median_zip_record;

//map of recipient to date, to record
map <string, map <string, record> > median_date_record;

/*======================================================*/

int main ( int argc, char* argv[])
{
	stringstream msg;

	//open global log file and set log level
	log_file.open (LOG_FILE);
	log_level = DEBUG;

	msg << endl << "==================================="<< endl;
	msg << "Running file find_political_donors" << endl;
	msg << endl << "==================================="<< endl;
	logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
	msg.str();

	//Taking input file 
	ifstream input;
	input.open (INPUT_FILE);

	// Variable to hold each input file line
	int MAX_LEN = 1024; 	//Acc to specifications its 520.
	char LINE[MAX_LEN];

	ofstream output_zip;
	ofstream output_date;

	//Open meadianvals_by_zip file to write results to it
	output_zip.open (OUTPUT_ZIP_FILE);

	//Open meadianvals_by_date file to write results to it
	output_date.open (OUTPUT_DATE_FILE);

	if ( input.is_open() ) {
		
		// Parse each line from file and write to medianvals_zip.txt simultaneously
		while ( input.getline ( LINE, MAX_LEN ) ) {
			parse_input_and_write_zip ( LINE, &output_zip);
		}
		
		//Close  files
		input.close();
		output_zip.close();

		//Now write to medianvals_date.txt file after reading input file completely
		write_output_date_file(&output_date);

		msg << endl << "==================================="<< endl;
		msg << "Processing done." << endl;
		msg << endl << "==================================="<< endl;
		logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
		msg.str();

		output_date.close();

		cout << "Processing done. Check results in output folder.Check logs in src/logs folder." << endl;
	}	
	else {
		cout << "Error in opening input file %s"<< INPUT_FILE << endl;
	}

	log_file.close();
	return 0;
}

/************************************************************
	Function to process data from input txt file for both
	zip and data based on key.
	key: string for "zip" or "date"
	recipient_record: points to correct map
	eg: map <string, map <string, record> > median_zip_record
	    map <string, map <string, record> > median_date_record
************************************************************/

void process_recipient_data(map <string, map <string, record> > *recipient_record,
							 string recipient,string key, record *new_rec)
{
	stringstream msg;

    msg << "Started processing record for recipient = " << recipient << " and key = " << key;
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
    msg.str("");

	// iterator to access in order to add or update new record and other details for each recipient	
	map <string, map <string, record> >::iterator it1;

	if ( (it1 = (*recipient_record).find(recipient)) == (*recipient_record).end() ) {
			//NEW RECIPIENT, first time entry for this record
			//Recipient not found, add new recipient, new zip/date and corresponding record
			map <string, record> new_key;
			new_key[key] = *new_rec;
			(*recipient_record)[recipient] = new_key;
	}
	else {
		//Found recipient, find first time entry for key
		map <string, record>::iterator it2;
		if ( (it2 = it1->second.find(key)) == it1->second.end() ) {
			//Found recipient, new zip, add new zip and add corresponding record
			(it1->second)[key] = *new_rec;	
		}
		else {
			//Found recipient & key, update earlier values in corresponding record
			//update earlier amount for this key & recipient
			(it2->second).total_num_transactions  += (*new_rec).total_num_transactions;
			(it2->second).total_amount_contributions += (*new_rec).total_amount_contributions;
			//To keep track of median
			insert_to_balance_heap ( &((it2->second).max_heap), &((it2->second).min_heap),
							(*new_rec).total_amount_contributions);

			(it2->second).median = get_median( &((it2->second).max_heap), &((it2->second).min_heap) );

			//calulate updated values for this key+recipient
			(*new_rec).total_num_transactions = (it2->second).total_num_transactions;
			(*new_rec).total_amount_contributions = (it2->second).total_amount_contributions;
			(*new_rec).median = (it2->second).median;
		}
	}

    msg << "Finished processing record for recipient = " << recipient << " and key = " << key;
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
    msg.str("");
	
	return;
}


/************************************************************
	Function to parse the input file and write the corresponding
	requested information to output medianvals_by_zip.txt
************************************************************/

void parse_input_and_write_zip ( char* line, ofstream *out)
{
	stringstream msg;

    msg << "Started parsing line: " << line;
    logger ( __FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
    msg.str("");

	// to store all fields in data dictionary
	vector <string> tokens;

	char* token = strtok_parse( line, "|");
	while ( token != NULL ) {
		if ( *token != '\0') {
			tokens.push_back(token);
		}
		else {
			tokens.push_back("");
		}
		token = strtok_parse( NULL, "|");
	}
			
	// Check for input file considerations provided
	//1. OTHER_ID field should be NULL, else ignore it
	//5. If TRANSACTION_AMOUNT is NULL, ignore 
	//5. If CMTE_ID is NULL, ignore 
	
	if ((tokens[OTHER_ID_POS] != "" ) ||
		(tokens[CMTE_ID_POS]  == "" ) ||
		(tokens[TRANSACTION_AMT_POS]  == "" )) {
		msg <<"Skipping record";
    	logger ( __FILE__, __FUNCTION__, __LINE__, msg.str(), WARNING);
		msg.str();
		return;
	}
	
	// get recipient id:
	string recipient = tokens[CMTE_ID_POS];

	// get transaction amount (it could be of type float 14,2) as specified, convert to int?
	float trans_amt_f = strtof(tokens[TRANSACTION_AMT_POS].c_str(), NULL );		
	float trans_amt = roundf(trans_amt_f * 100 ) / 100; //restrict decimal to 2 places		

	// make sure amount is not negative
    if (trans_amt < 0) {
        msg << "Skipping record: ";
        msg << "transaction amount = " << trans_amt << " is less than zero";
        logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), WARNING);
        msg.str("");
        return;
    }

	// create new record
	record new_rec;
	new_rec.total_num_transactions     = 1;
	new_rec.total_amount_contributions = trans_amt;
	new_rec.max_heap.push_back(trans_amt);
	new_rec.median = (int)(trans_amt+0.5); //First time record encountered

	//Check if valid Zip code:
	//3,4.ZIP_CODE: only first 5 digits to be condidered, if less than 5 digits, use in medianvals_date.txt, do not use in medianvals_zip.txt

	// process for zip data
	if ( (tokens[ZIP_CODE_POS] != "") &&
		 (tokens[ZIP_CODE_POS].size() >= 5 ) ) {

		// get zip code:
		string zip_code = tokens[ZIP_CODE_POS].substr(0,5);

		process_recipient_data(&median_zip_record, recipient, zip_code, &new_rec);
		
		//write to zip output file for every line processed
		*out << recipient << "|";
		*out << zip_code  << "|";
		*out << new_rec.median << "|";
		*out << new_rec.total_num_transactions<< "|";
		*out << new_rec.total_amount_contributions << endl;
	}	//zip validity check
	else {
        msg << "Skipping record for zip processing: ";
        msg << "zip code = " << tokens[ZIP_CODE_POS] << " is invalid";
        logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), WARNING);
        msg.str("");
	}

	// Process for date data
	// reset new record
	new_rec.total_num_transactions     = 1;
	new_rec.total_amount_contributions = trans_amt;
	new_rec.median = (int)(trans_amt+0.5); //First time record encountered

	//2.TRANSACTION_DT if invalid : use in medianvals_zip.txt, do not use in medianvals_date.txt
	// get date and validate it
	if ( isValid(tokens[TRANSACTION_DT_POS]) ) {
		string date = tokens[TRANSACTION_DT_POS];
		process_recipient_data(&median_date_record, recipient, date, &new_rec);
	}
	else {
        msg << "Skipping record for date processing: ";
        msg << "transaction date = "<< tokens[TRANSACTION_DT_POS] << " is invalid";
        logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), WARNING);
        msg.str("");
	}

    msg << "Finished parsing line = " << line;
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
    msg.str("");
	
	return;
}

/************************************************************
Function to write to output medianvals_date.txt after entire input file is parsed and median_date_record is populated.
************************************************************/

void write_output_date_file(ofstream *out)
{
	stringstream msg;

    msg << "Started writing output medianvals_by_date.txt ";
    logger ( __FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
    msg.str("");

	map <string, map <string, record> >::iterator it1;
	for ( it1 = median_date_record.begin(); it1!= median_date_record.end(); ++it1 ) {
		string recipient = it1->first;
		map<string,record>::iterator it2;
		for (it2=(it1->second).begin(); it2!=(it1->second).end(); ++it2) {
			string date = it2->first;
			int median = (it2->second).median;
			int count  = (it2->second).total_num_transactions;
			float amt  = (it2->second).total_amount_contributions;

			//write to zip output file for every line processed
			*out << recipient << "|";
			*out << date  << "|";
			*out << median << "|";
			*out << count  << "|";
			*out << amt << endl;
		}	
	}

    msg << "Finished writing output medianvals_by_date.txt ";
    logger ( __FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
    msg.str("");

	return;
}

/******************************************************** 
	//Function to insert new amount in min or max heap 
	based on the status of two heaps and setup parameters
	for heap operations
********************************************************/ 

void insert_to_balance_heap (deque<float> *max_heap, deque<float> *min_heap, float num)
{
    stringstream msg;

    msg << "Started to insert " << num << " into heap and balance if needed";
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), INFO);
    msg.str("");

    bool balance = false;
    bool heapify_max, insert_max;
    deque<float> *src_heap, *dst_heap;
    int max_heap_size = max_heap->size();
    int min_heap_size = min_heap->size();

    msg << "Before insertion: max_heap_size = " << max_heap_size << "; min_heap_size = " << min_heap_size;
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");

    msg << "Comparing with max_heap root = " << (*max_heap)[0];
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");
	
    // there is always at least one entry in max_heap
    // to start with as we initialize with new_rec that has
    // max_heap filled with the initial amount. so just
    // check heap value vs num to check which heap it should go into
	// but just simulate the insert by increasing size, so we can optimize
    if ((*max_heap)[0] >= num) {
        insert_max = true;
        src_heap = max_heap;
        max_heap_size++;
        logger (__FILE__, __FUNCTION__, __LINE__,"To insert into max_heap", DEBUG);
    } else {
        insert_max = false;
        src_heap = min_heap;
        min_heap_size++;
        logger (__FILE__, __FUNCTION__, __LINE__,"To insert into min_heap", DEBUG);
    }

    msg << "After test insertion: max_heap_size = " << max_heap_size<< "; min_heap_size = " << min_heap_size;
    logger (__FILE__, __FUNCTION__, __LINE__, msg.str(), DEBUG);
    msg.str("");

    // check to see if the heaps need to be balanced
    if ((max_heap_size - min_heap_size) > 1) {
        heapify_max = true;
        insert_max = false;
        src_heap = max_heap;
        dst_heap = min_heap;
        balance = true;
    } else if ((min_heap_size - max_heap_size) > 1) {
        heapify_max = false;
        insert_max = true;
        src_heap = min_heap;
        dst_heap = max_heap;
        balance = true;
    }
	if (balance) {
        logger (__FILE__, __FUNCTION__, __LINE__, "Need to insert and re-balance", DEBUG);
        // insert the top of src heap into dest
        insert_heap (dst_heap, (*src_heap)[0], insert_max);

        // copy the bottom to top, delete bottom, and heapify
        (*src_heap)[0] = num;
        heapify (src_heap, heapify_max);
    } else {
        logger (__FILE__, __FUNCTION__, __LINE__, "Only insert, no re-balance required", DEBUG);
        insert_heap (src_heap, num, insert_max);
    }

    return;
}

/*======================================================*/
