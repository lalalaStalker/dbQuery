/*Laura Stalker
LS451494
lstalker@albany.edu
*/


#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A function that takes a string and an int as parameters and returns an 
int. The first string is assumed to be the name of a relation in a database,
and the int is 1 if we want to print out information about the length of a 
tuple to stdout, and something else if not. This function adds ".sch\0" to the 
end of the given relation name, to create the name of the schema file for this 
relation. The schema file is then opened, and the total number of bytes for 
this relation is tallied, then printed to stdout. This function assumes that 
the schema file begins with an int which represents the number of lines in 
the file (plus 1), and that each row contains a string, a string, and an int 
(which tells the number of bytes needed for that attribute).  
*/

int tuplen( char *relationName, int flag){

	FILE *schemaFile;  /*Ptr to schema file we will name from relation*/
	int numAttr;  /*For storing number of attributes*/
	char temp[MAX_STRING];  /*For storing string to skip in schema file*/
	char type[CHAR_LENGTH];  /*For storing char to skip in schema file*/
	int addToSum;  /*Temp storage for num bytes before adding to sum*/
	int sum;  /*Sum of the bytes needed for this relation*/

	/*Create an array large enough for a string that is the attribute
	name plus ".sch\0". This will be the name of the schema file. 
	*/
	char fileName[ strlen( relationName) + strlen( ".sch\0")];
	strcpy( fileName, relationName);
	strcat( fileName, ".sch\0");

	/*Open the schema file for reading. If unable to open, print an
	error message and exit.
	*/
	if( (schemaFile = ( fopen( fileName, "r"))) == NULL ){
		printf("There was an error trying to open schema file.\n");
		exit(1);
	}


	/*Start the sum of the bytes in the relation at 0. Get the first 
	int in the schema file and set that as numAttr. Scan the schema 
	file numAttr times for an attribute name, a type, and the length
	of the atttribute in bytes. Add the length to the running tally
	of length for this relation.
	*/
	sum = addToSum = 0;
	fscanf( schemaFile, "%d", &numAttr);
	while( numAttr > 0){
		fscanf( schemaFile, "%s%s%d", temp, type, &addToSum);
		sum = sum + addToSum;
		numAttr--;
	}

	/*Print a message stating the number of bytes in this relation
	to stdout, if the flag is set to 1 (which means we do want this
	info to print out).
	*/
	if( flag == 1){
		printf( "Each tuple in the %s relation is %d bytes.\n\n",
			relationName, sum);
		fflush( stdout);
	}

	/*Close schema file
	*/
	if( fclose( schemaFile) == EOF){
		fprintf( stderr, "Error closing schema file.\n");
		exit(1);
	}

	/*Return the total number of bytes in this relation.
	*/
	return sum;
}