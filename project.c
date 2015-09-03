/*Laura Stalker
LS451494
lstalker@albany.edu
*/


#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A function that takes two strings as parameters, and returns nothing.
It is assumed that the first string param is a relation name and the 
second is an attribute name for the database.  First, create the name of
then open the schema file for this relation. Get the tuple length from the 
schema file and use it to determine whether the given attribute is an
attribute of the given relation. Next, get the name of the data file and 
open it. Locate the spot in each tuple that the value for the attribute 
can be found. Add the value of this attribute for each tuple (if it is not
a duplicate) to an array. Then print all the values in the array to stdout.
*/

void project( char *relName, char *attrName){

	FILE *schemaFile;  /*Ptr to schema file*/
	FILE *dataFile;  /*Ptr to data file*/
	int numAttr;  /*Stores number of attributes in schema file*/
	char name[MAX_STRING];  /*Temp for attribute name*/
	char type[CHAR_LENGTH];  /*Temp for attr type*/
	int bytes;  /*Temp for num of bytes in attribute*/
	int bytesToAttr;  /*Num of bytes from begin of tuple to attr*/
	int flag;  /*Flag to tell if attr found in relation*/
	char strings[MAX_TUPLES][MAX_TUPLES];  /*Array of strings (type S)*/
	int numbers[MAX_TUPLES];  /*Array of ints (type I)*/
	int tempInt;  /*Int for storing attribute values*/
	int arrayCounter;  /*Keeps track of current array index*/
	int tupleLen;  /*Length of a tuple for this attribute*/
	char *temp;  /*Temp array for fread junk*/
	int counter;  /*Loop counter for array*/

	/*Create an array large enough for a string that is the attribute
	name plus ".sch\0". This will be the name of the schema file. 
	*/
	char fileName[ strlen( relName) + strlen( ".sch\0")];
	strcpy( fileName, relName);
	strcat( fileName, ".sch\0");

	/*Open the schema file for reading. If unable to open, print an
	error message and exit.
	*/
	if( (schemaFile = ( fopen( fileName, "r"))) == NULL ){
		printf("There was an error trying to open schema file.\n");
		exit(1);
	}

	tupleLen = tuplen( relName, 0);  /*Get length of tuple for this rel'n*/
	/*Set temp array to length of a tuple, for use in a bit*/
	if( (temp = ((char *)malloc( tupleLen))) == NULL){
		printf("Space allocation failed.\n");
		exit(1);
	}

	fscanf( schemaFile, "%d", &numAttr);  /*Get first int in schema file*/
	flag = 0;  /*Initialize flag to attribute not found*/
	bytes = bytesToAttr = 0;  /*Initialize num bytes to 0*/

	/*Check all attributes in schema file for attrName. If found, set 
	flag to found(1), and stop looking. Keep track of number of bytes 
	in schema before the attribute we want.
	*/
	while( numAttr > 0){
		fscanf( schemaFile, "%s%s%d", name, type, &bytes);
		if( strcmp( attrName, name) == 0){
			flag = 1;
			break;
		}
		numAttr--;
		bytesToAttr = bytesToAttr + bytes;
	}

	/*If the attribute was not found for this relation, print a message
	*/
	if( flag == 0){
		printf(" Error: %s attribute is not part of %s relation.\n\n", 
				attrName, relName);
		fflush( stdout);
		return;
	}

	/////////////////BEGIN WORKING WITH DATA FILE///////////////////

	/*Copy attribute name plus ".dat\0" into fileName string. This will 
	now be the name of the data file. 
	*/
	strcpy( fileName, relName);
	strcat( fileName, ".dat\0");

	/*Open the data file for reading. If unable to open, print an
	error message and exit.
	*/
	if( (dataFile = ( fopen( fileName, "r"))) == NULL ){
		printf("There was an error trying to open data file.\n");
		exit(1);
	}

	arrayCounter = 0;  /*Initialize array counter*/
	tempInt = 0;  /*Initialize temporary int to 0*/
	flag = 0;  /*Initialize flag to no duplicate value found*/

	/*If the attribute type int or string, then check whether it is the first 
	attribute in a tuple or not.  If it is the first attribute in a tuple,
	look through the array of values already come across and see if there is 
	a duplicate. If it is a duplicate, do not add it to the array. If it isn't
	a dup, add it to the next open array space in the correct array (numbers
	if an int, strings if a string). Read through all tuples in the data file.
	Then print all the values for the attribute.
	*/
	if( strcmp( type, "I") == 0){  /*Attribute type == INT*/
		if( bytesToAttr == 0){  
		/*First attribute in a tuple, read the attr and store the value*/
			while( (fread( (void *) &tempInt, sizeof(int), 1, dataFile)) == 1){
				counter = 0;
				flag = 0;
				while( counter < arrayCounter){  /*Look for dup value*/
					if( tempInt == numbers[counter] ){
						flag = 1;  /*Duplicate value found in array*/
					}
					counter++;
				}
				if( flag == 0){  /*If not a duplicate value*/
					numbers[arrayCounter] = tempInt;  /*Add value to array*/
					arrayCounter++;
				}
			/*Continue reading data file
			*/
			fread( (void *) temp, (tupleLen - bytes), 1, dataFile);  
			}
		
		}///////////////////IF NOT FIRST ATTRIBUTE IN TUPLE///////////////////

		else if( bytesToAttr != 0){  /*Not first attribute in tuple*/
			while( (fread( (void *) temp, bytesToAttr, 1, dataFile)) == 1){
				/*Read until beginning of the attribute, then read space
				of desired attribute and store value*/
				fread( (void *) &tempInt, sizeof(int), 1, dataFile);
				counter = 0;
				flag = 0;
				while( counter < arrayCounter){  /*Look for dup value*/
					if( tempInt == numbers[counter] ){
						flag = 1;  /*Duplicate value found in array*/
					}
					counter++;
				}
				if( flag == 0){  /*If not a duplicate value*/
					numbers[arrayCounter] = tempInt;  /*Add to array*/
					arrayCounter++;
				}
				fread( (void *) temp, (tupleLen - bytesToAttr - bytes),
						 1, dataFile);  /*Continue reading data file*/
			}
		}
		
		counter = 0;  /*Init loop counter*/
		/*Look at each spot in the numbers array, print the value stored 
		there to stdout
		*/
		while( counter < arrayCounter){  
			printf("%d\n", numbers[counter]);
			fflush( stdout);  /*Flush the buffer*/
			counter++;
		}
		printf("\n");  /*Leave space between query results*/
		fflush( stdout);

	}////////////END TYPE 'I'/////////////

	else{  /*If attribute is of type 'S'*/
		if( bytesToAttr == 0){  
		/*Attr is first in tuple, read the tuple and store the string*/
			while( (fread( (void *) temp, bytes, 1, dataFile)) == 1){
				counter = 0;
				flag = 0;
				while( counter < arrayCounter){  /*Look for dup string*/
					if( strcmp( temp, strings[counter]) == 0 ){
						flag = 1;  /*Dup string in array*/
					}
					counter++;
				}
				if( flag == 0){  /*If not a duplicate value*/
					strcpy( strings[arrayCounter], temp);  /*Add to array*/
					arrayCounter++;
				}
			/*Continue reading data file
			*/
			fread( (void *) temp, (tupleLen - bytes), 1, dataFile);
			}
		
		}/////////////////IF NOT FIRST ATTRIBUTE IN TUPLE////////////////////
		
		else if( bytesToAttr != 0){  
		/*Not first attr in a tuple, read before the attribute, then read the
		attribute and store the string
		*/
			while( (fread( (void *) temp, bytesToAttr, 1, dataFile)) == 1){
				fread( (void *) temp, bytes, 1, dataFile);
				counter = 0;
				flag = 0;
				while( counter < arrayCounter){  /*Look for dup string*/
					if( strcmp( temp, strings[counter]) == 0 ){
						flag = 1;  /*Dup string in array*/
					}
					counter++;
				}
				if( flag == 0){  /*If not a duplicate value*/
					strcpy( strings[arrayCounter], temp);  /*Add to array*/
					arrayCounter++;
				}
				/*Continue reading data file
				*/
				fread( (void *) temp, (tupleLen - bytesToAttr - bytes),
						 1, dataFile);
			}
		}

		counter = 0;
		/*Look at each spot in the string array, and print the string at
		each location to stdout.
		*/
		while( counter < arrayCounter){
			printf("%s\n", strings[counter]);
			fflush( stdout);  /*Flush buffer*/
			counter++;
		}
		printf("\n");  /*Leave space between query results*/
		fflush( stdout);

	}/////////END TYPE 'S'///////



	/*Close schema file
	*/
	if( fclose( schemaFile) == EOF){
		fprintf( stderr, "Error closing schema file.\n");
		exit(1);
	}

	/*Close data file
	*/
	if( fclose( dataFile) == EOF){
		fprintf( stderr, "Error closing data file.\n");
		exit(1);
	}

}