#insight_political_donor

**To run the case** 

Use: "./run.sh"
or   "./run.sh <log_level>"

There is provision to print all log details when the case is run.
**ERROR(0), WARNING(1), INFO(2), DEBUG(3)** are the options to which log level can be set.
Default LOG LEVEL has been set to INFO, which will print if any error or warning encountered in the "../src/logs/find-political_donors.log" file
To set alternate level provide it via command line as follows:
eg: "./run.sh 3" ----->This will provide all DEBUG information in log file.


**REPOSITORY**

Repository directory structure is created based on details provided.

"src" folder has multiple files/folders under it all related to the application flow.
The first time we run "./run.sh", additional folders - "src/bin", "src/obj", "src/logs" will be created. On following runs, these folders already exist, so prompt will be displayed accordingly.  A Makefile is added under "src" to build the binary.

"input" folder has sample input file "itcont.txt" is stored under it

"output" folder is added ( with a ".keep" file under it) in the repository. The actual *.txt files will be written here @runtime.
Existing *.txt files are removed and then written for consecutive runs.


**APPROACH:**

The entire functionality to print the two outout files has been split into primarily 3 source files and 3 header files.
A Makefile is added under src.

1- **find_political_donors.cc**
 It is the top level file which parses the input txt file and then populates record structure for evaluating median values by using zip and median values by using date. 
 The output for medianvals_by_zip.txt is written as we parse each line as here, there can be duplicates and the median and total are the running median and total and printed accordingly for duplicate cases. 
 The output for medianvals_by_date.txt is written after the entire input file is parsed as we need all the records pertaining to a particular recipient for a particular date and then provide a unique output record with requested details.

LOG_LEVEL is set to INFO in this file, which will print all information and if any error or warnings are encountered in the "../src/logs/find-political_donors.log" file
  
  
2- **misc_utils.cc**
  This file is used for all helper functions required by functions in other files. It includes functions to get log information for error, warning, debug as required. It also has the function to validate the date provided. The function written to tokenize each field is also included. It provides the pipe separates tokens as specified in the dictionary. If the fields are empty with consecutive pipe separation, we accept that as an empty string field. The function returns NULL when entire string is read.

3- **heap_utils.cc**
  This file is used for all function related to heap - insertion, balance. In order to evaluate the median for running amounts obtained from the FEC, the idea is to use to binary heaps- MIN_HEAP and MAX_HEAP. At every time, both the heaps are sorted and the difference in size in the heaps is atmost 1. When evaluating median, if the two heaps as equal in size, the median value is the sum of value at root od MAX_HEAP and value at root of MIN_HEAP divided by two. If they are not of same size, the median is evaluated as the value at the root of the heap which is bigger in size. Since the algorithm for all functions to insert and balance heaps is similar and only the comparator changes, we have used only one function for both heaps. A bool value is provided as an argument to the function which can switch the flow depending on using it for MIN_HEAP or MAX_HEAP.
  
All src files have corresponding header files located at src/include/*h . The function declarations are provided in header files.

- Used "int" for reporting median based on the data assuming max of median to not exceed 15 digits. (as amount format specified is (14,2))
- Used "float" for total amount reported, which provides a max range of 3.40...e+38



**TESTING**

Added folders under the insight_testsuite testing folder.
For the purpose of testing log_level is set to INFO in all cases.

Tests are run using provided script "run_tests.sh" where result is compared to expected output for different scenarios


*test_2*: This test is to verify if the incorrect input data such as 1)invalid date, 2)invalid zipcode when encountered, that record is ignored and we proceed to the next data. As all input data has invalid dates, the medianvals_by_date.txt file generated is empty.Log files print corresponding information as WARNING.

*test_3*: This test has been provided to check for multiple records in input file which have all the same content. 
The expected output for medianvals_by_zip.txt - the no of records will match the no of records in input file, while updating the median and total.
The expected output for medianvals_by_date.txt will be just one record with cumulative value.

*test_4*:This is a reduced file (for size resctrictions to add to repository) originaly obtained from FEC website.(Tested originaly using large file to check actual data size from FEC). We can observe the output file is sorted by recipient name and then by the date chronologically in medianvals_by_date.txt

**TBD**

*test_4*:Unable to add this input file to git repository due to large size.  
Using a script compare runtime performance for consecutive runs for future enhancements or modifications to measure runtime. In the script we can compare the current runtime with earlier runtime and ensure they are within a fixed tolerance,else generate a warning indicating any issue.  
- Currently the code does not thoroughly check the validity of the entire input data. It only tests for selected tokens to be valid in the input data as specified under data considerations. It assumes input data is always provided in text form and does not check for it before parsing. These functionalities can be added as well.
-Another test would be to check the output text files always contains five fields for each record. 
-Test that the no of records in output file can at max be equal to no of records in input text file. 
