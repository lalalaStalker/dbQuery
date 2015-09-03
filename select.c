/*Laura Stalker
LS451494
lstalker@albany.edu
*/


#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A function that takes 6 parameters: a string that is a relation name, a
string that is an attribute name, a string that is a relational operator
(one of: ==, <=, >=, <, >, or !=), an int that is the integer value of an
attribute to be tested, a string that is also from a query and is to be 
tested, and a string that is a type (either "S" for string or "I" for int).
The function returns nothing. The function pulls various info from the 
schema and data files that correspond to this relation/ attribute combo: 
including number of tuples in the relation, size of a tuple in the relation,
the type of the atttribute being compared, and the byte the attribute starts
at in any given tuple. It determines whether or not a tuple in the data file 
satisfies the condition given in the query (represented by attribute name, 
relational operator, and either string or int value). It assumes that any 
strings in the query file that are to be compared, are surrounded by double
quotes. The function will print to stdout, either the tuple that satisfies, 
or messages that state any problems with the query, such as: If the query is 
asking to compare a string to an int, if the relational operator is invalid 
for comparing the types asked to compare, or if there are no tuples in the db
which satisfy the condition.
*/

void select( char *relName, char *attrName, char *relop, int value, 
								char *string, char *type){

	FILE *dataFile;  /*Ptr to data file*/
	FILE *schemaFile;  /*Ptr to schema file*/

	int tupleLen;  /*Length of a tuple for this relation*/
	int numTuples;  /*Number of tuples for this relation*/
	int startByte;  /*Byte to start at for this attribute in tuple*/
	int numAttr;  /*Number of attributes in schema*/
	char tempType[MAX_STRING];  /*Temp for attribute type*/
	char tempName[MAX_STRING];  /*Temp for attribute name*/

	int bytes;  /*Temp storage for bytes of an attribute*/
	int bytesArray[MAX_ATTRIBUTES];  /*Array for bytes of each attribute*/
	/*Array for type of each attribute
	*/
	char typeArray[MAX_ATTRIBUTES][MAX_ATTRIBUTES];  
	int attrBytes;  /*Number of bytes for this specific attribute*/

	int counter;  /*Array index counter*/

	int tempInt;  /*Temp to store attribute value in*/
	char tempString[STRING_LENGTH];  /*Temp to store attr string in*/
	char finalString[STRING_LENGTH];  /*string param with "" added*/

	int tupleFound;  /*Flag to tell if a tuple satisfying condition was found*/


	/*Create an array large enough for a string that is the attribute
	name plus ".dat\0". This will be the name of the data file. 
	*/
	char fileName[ strlen( relName) + strlen( ".dat\0")];
	strcpy( fileName, relName);
	strcat( fileName, ".dat\0");

	/*Open the data file for reading. If unable to open, print an
	error message and exit.
	*/
	if( (dataFile = ( fopen( fileName, "r"))) == NULL ){
		printf("There was an error trying to open data file.\n");
		exit(1);
	}

	/*Copy the attribute name plus ".sch\0" into fileName. This will be the 
	name of the schema file. 
	*/
	strcpy( fileName, relName);
	strcat( fileName, ".sch\0");

	/*Open the schema file for reading. If unable to open, print an
	error message and exit.
	*/
	if( (schemaFile = ( fopen( fileName, "r"))) == NULL ){
		printf("There was an error trying to open schema file.\n");
		exit(1);
	}


	/////////////FILES ARE OPEN, NOW FILL ARRAY WITH BYTE SIZES/////////////

	tupleLen = tuplen( relName, 0);  /*Get length of tuple for this rel'n*/
	numTuples = count( relName, 0);  /*Get num of tuples for this rel'n in db*/
	/*Get byte in a tuple where attribute begins
	*/
	startByte = whatType( relName, attrName, 1); 
	fscanf( schemaFile, "%d", &numAttr);  /*Get numAttr from schema file*/
	
	counter = 0;  /*Initialize array counter*/
	bytes = 0;  /*An initial value for bytes for an attribute*/

	/*Go through schema file and get number of bytes for each attribute
	and the type, and store the bytes value in a successive spots in an 
	array and types in another array.
	*/
	while( numAttr > 0){
		fscanf( schemaFile, "%s%s%d", tempName, tempType, &bytes);
		if( strcmp( attrName, tempName) == 0){  /*If match attr*/
			attrBytes = bytes;  /*Store # bytes in attrBytes*/
		}
		bytesArray[counter] = bytes;  /*Add attr length to bytesArray*/
		strcpy( typeArray[counter], tempType);
		numAttr--;  /*To get all attr in schema file*/
		/*Move to next position in array to store bytes of each attr
		*/
		counter++;  
	}


	/////ARRAYS FILLED, LOOP THRU DATA FILE TO FIND TUPLES THAT SATISFY/////

	numAttr = counter;  /*Reset numAttr to total # of attributes*/
	counter = 0;  /*Reset array counter*/
	tupleFound = 0;  /*No tuples satisfying found yet*/

	while( numTuples > 0){  /*While there are more tuples to check*/

		fseek( dataFile, (long) startByte, SEEK_CUR);  /*Get to attr to check*/

		if( strcmp( type, "I") == 0){  /*If the type is INT*/

			/*If we are going to compare an int, but we were given a string,
			then the comparison will fail, print an error message, and end*/
			if( value == -1){  
				printf(" Error: Wrong type of value for attribute %s.\n\n", 
							attrName);
				fflush( stdout);
				return;
			}

			/*Store value to check in tempInt*/
			fread( (void *) &tempInt, sizeof(int), 1, dataFile); 


			if( strcmp( relop, "==") == 0){  /***If relop is EQUALS***/

				if( tempInt == value){  /*If condition satisfied*/
					tupleFound = 1;  /*A tuple satisfying condition was found*/
					/*back up to start of tuple in the file 
					*/
					fseek( dataFile, (long) -(sizeof(int) + startByte), SEEK_CUR);
					counter = 0;  /*Reset loop counter*/

					//get each piece of tuple & print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 1, 
										dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}

					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{  /*If condition not satisfied, move to end of tuple*/
					fseek( dataFile, 
						(long) (tupleLen - startByte - sizeof(int)), SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else if( strcmp( relop, ">=") == 0){  /*If relop is GREATER/EQUAL*/

				if( tempInt >= value){
					tupleFound = 1;  /*A tuple satisfying condition was found*/					
					//back up to start of tuple
					fseek( dataFile, 
						(long) -(sizeof(int) + startByte), SEEK_CUR);
					counter = 0;  /*Reset loop counter*/
					//get each piece of tuple & print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 1,
										dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}
					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{  /*If condition not satisfied, move to end of tuple*/
					fseek( dataFile, 
						(long) (tupleLen - startByte - sizeof(int)), SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else if( strcmp( relop, "<=") == 0){  /**If relop is LESS/EQUAL**/

				if( tempInt <= value){
					tupleFound = 1;  /*A tuple satisfying condition was found*/
					//back up to start of tuple
					fseek( dataFile, 
						(long) -(sizeof(int) + startByte), SEEK_CUR);
					counter = 0;  /*Reset loop counter*/
					//get each piece of tuple & print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, 
										dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 
										1, dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}
					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{  /*If condition not satisfied, move to end of tuple*/
					fseek( dataFile, 
						(long) (tupleLen - startByte - sizeof(int)), SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else if( strcmp( relop, ">") == 0){  /**If relop is GREATER THAN**/

				if( tempInt > value){
					tupleFound = 1;  /*A tuple satisfying condition was found*/
					//back up to start of tuple
					fseek( dataFile, (long) -(sizeof(int) + startByte), 
								SEEK_CUR);
					counter = 0;  /*Reset loop counter*/
					//get each piece of tuple & print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, 
										dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 
										1, dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}
					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{  /*If condition not satisfied, move to end of tuple*/
					fseek( dataFile, 
						(long) (tupleLen - startByte - sizeof(int)), SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else if( strcmp( relop, "<") == 0){  /***If relop is LESS THAN***/

				if( tempInt < value){
					tupleFound = 1;  /*A tuple satisfying condition was found*/
					//back up to start of tuple
					fseek( dataFile, (long) -(sizeof(int) + startByte), 
									SEEK_CUR);
					counter = 0;  /*Reset loop counter*/
					//get each piece of tuple & print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, 
										dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 
										1, dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}
					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{  /*If condition not satisfied, move to end of tuple*/
					fseek( dataFile, 
						(long) (tupleLen - startByte - sizeof(int)), SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else if( strcmp( relop, "!=") == 0){  /***If relop is NOT EQUAL***/

				if( tempInt != value){
					tupleFound = 1;  /*A tuple satisfying condition was found*/
					//back up to start of tuple
					fseek( dataFile, (long) -(sizeof(int) + startByte), 
									SEEK_CUR);
					counter = 0;  /*Reset loop counter*/
					//get each piece of tuple & print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, 
										dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 
										1, dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}
					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{  /*If condition not satisfied, move to end of tuple*/
					fseek( dataFile, 
						(long) (tupleLen - startByte - sizeof(int)), SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else{  /*If given an invalid relop, print error message*/
				printf(" Error: %s is not a valid relational operator\n\n", 
							relop);
				fflush( stdout);
				/*So as not to print "no satisfying tuples" message in 
				addition to this message
				*/
				tupleFound = 1;  /*Set flag for tuple was found*/
				break;
			}
		}  ////////END IF TYPE I///////////


		else if( strcmp( type, "S") == 0){  /*If attribute is of type STRING*/

			/*If we are expecting to compare two strings, but we were given an
			int to compare, comparison will fail. Print an error message, and
			stop.
			*/
			if( *(string) != '\"'){
				printf(" Error: Wrong type of value for attribute %s.\n\n", 
							attrName);
				fflush( stdout);
				return;
			}

			/*Store value to check in tempInt*/
			fread( (void *) tempString, attrBytes, 1, dataFile);

			/*Add quotes to string from data file to make comparison with 
			query string correct
			*/
			strcpy( finalString, "");
			strcpy( finalString, "\"");  
			strcat( finalString, tempString);
			strcat( finalString, "\"");

			if( strcmp( relop, "==") == 0){  /*If relop is EQUALS*/

				if( strcmp( finalString, string) == 0){
					tupleFound = 1;  /*A tuple satisfying condition was found*/
					//back up to the start of tuple
					fseek( dataFile, (long) -(startByte + attrBytes), 
								SEEK_CUR);
					counter = 0;  /*Reset loop counter*/
					//get each piece of tuple & print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, 
										dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 
										1, dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}
					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{
					//move to end of tuple
					fseek( dataFile, (long) (tupleLen - startByte - attrBytes),
								SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else if( strcmp( relop, "!=") == 0){  /*If relop is NOT EQUALS*/

				if( strcmp( finalString, string) != 0){
					tupleFound = 1;  /*A tuple satisfying condition was found*/
					//back up to start of tuple
					fseek( dataFile, (long) -(startByte + attrBytes), 
								SEEK_CUR);
					counter = 0;  /*Reset loop counter*/
					//get each piece of tuple and print it
					while( counter < numAttr){
						/*If this attribute in tuple is an int, get value and
						store it in tempInt. Then print out the value.
						*/
						if( strcmp( typeArray[counter], "I") == 0 ){
							fread( (void *) &tempInt, sizeof(int), 1, 
										dataFile);
							printf("%d\t", tempInt);
							fflush( stdout);
						}
						/*If this attribute of the tuple is a string, get the 
						string and store it. Then print it.
						*/
						else{
							fread( (void *) tempString, bytesArray[counter], 
										1, dataFile);
							printf("%s\t", tempString);
							fflush( stdout);
						}
					counter++;  /*Move to next attr in tuple*/
					}
					printf("\n");  /*Print a blank line between query results*/
					fflush( stdout);
				}
				else{
					//move to end of tuple
					fseek( dataFile, (long) (tupleLen - startByte - attrBytes),
								SEEK_CUR);
				}
				numTuples--;  /*One less tuple to check*/
				continue;  /*Move to next iteration (next tuple)*/
			}
			else{
				printf(" Error: %s is not a valid relop for strings.\n\n", 
							relop);
				fflush( stdout);
				/*So as not to print "no satisfying tuples" message in 
				addition to this message
				*/
				tupleFound = 1;  /*Set flag for tuple was found*/
				break;
			}

		}  ////////END IF TYPE S//////////


		numTuples--;  /*One less tuple to check*/
	}  //////////END WHILE NUMTUPLES > 0///////////////

	/*If no tuple satisfying the condition was found, print a message
	*/
	if( tupleFound == 0){
		printf("No tuples satisfy the specified condition.\n");
		fflush( stdout);
	}

	printf("\n");  /*Print a blank line between different query results*/
	fflush( stdout);


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



/*A function that takes a string that is a relation name, a string
that is an attribute name, and an int that tells whether the user wants
the number of bytes in a tuple before the given attribute returned
(set to 1), or if the user wants the type of this attribute returned.
The function looks through the schema file for the given relation to 
determine these pieces of information. The int returned is either the
number of bytes that occur in a tuple before the given attribute (if the 
value of wantBytes is 1), or the type of the attribute (1 if an int, and
some other number if a string type).
*/

int whatType( char *relName, char *attrName, int wantBytes){

	FILE *schemaFile;  /*Ptr to schema file*/
	int tupleLen;  /*Length of a tuple for relation*/
	int numAttr;  /*Number of attributes for this relation*/
	char name[MAX_STRING];  /*For storing string name in schema file*/
	char type[CHAR_LENGTH];  /*For storing char type in schema file*/
	int bytes;  /*Number of bytes for attr*/
	int bytesToAttr;  /*Num bytes to start of attr*/

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
	bytesToAttr = 0;

	fscanf( schemaFile, "%d", &numAttr);  /*Get first int in schema file*/

	/*Check all attributes in schema file for attrName. If found, stop 
	looking. 
	*/
	while( numAttr > 0){
		fscanf( schemaFile, "%s%s%d", name, type, &bytes);
		if( strcmp( attrName, name) == 0){
			break;
		}
		numAttr--;
		/*Compute the number of bytes in the tuple before the attribute
		we are going to compare
		*/
		bytesToAttr = bytesToAttr + bytes;
	}

	/*Close schema file
	*/
	if( fclose( schemaFile) == EOF){
		fprintf( stderr, "Error closing schema file.\n");
		exit(1);
	}

	/*If the user wants the number of bytes in a tuple before the given 
	attribute returned, rather than they type of the attribute, return that*/
	if( wantBytes == 1){
		return bytesToAttr;
	}
	else{
		/*If type for this attribute is an int, return 1. If the type is 
		string, return 0
		*/
		if( strcmp( type, "I") == 0){
			return 1;  /*1 for an int type*/
		}
		else{
			return 0;  /*0 for a string type*/
		}
	}

}
