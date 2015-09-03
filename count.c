/*Laura Stalker
LS451494
lstalker@albany.edu
*/


#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A function that takes a string and an int as a parameter, and returns an 
int. The string is assumed to be the name of a relation in the database. It 
creates the name of a data file from a relation name (for that relation), and 
then opens that file. It calls another function to get the length of each tuple 
in this particular relation. It finds the end of the data file, and uses 
the length of a tuple for this relation to calculate the number of tuples in
this relation in the db. It prints this number to stdout, and returns it. The
flag param is 1 if we want this info printed.
*/

int count( char *relName, int flag){

	FILE *dataFile;  /*Ptr to data file we get from relName*/
	int tupleLen;  /*Holds length of tuple for this relation*/
	long temp;  /*Holds value of end of file, and then length*/

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

	/*Get the length of a tuple for this relation
	*/
	tupleLen = tuplen( relName, 0);  

	/*Move to the end of the file and get the position. If unaable to,
	print an error message.
	*/
	if( fseek( dataFile, 0, SEEK_END) == 0){
		temp = ftell( dataFile);
	}
	else{
		printf("The size of the file could not be determined.\n");
		fflush( stdout);
	}

	/*the number of tuples in the relation is equal to the number of bytes in
	the data file divided by the number of bytes in a tuple.
	*/
	temp = temp / tupleLen;

	/*Print the number of tuples in the relation to stdout.
	*/
	if( flag == 1 ){
		printf("There are %d tuples in the %s relation.\n\n", (int) temp, relName );
		fflush( stdout);
	}
	

	/*Close data file
	*/
	if( fclose( dataFile) == EOF){
		fprintf( stderr, "Error closing data file.\n");
		exit(1);
	}

	return (int) temp;  /*Return num tuples*/
}