/*Laura Stalker
LS451494
lstalker@albany.edu
*/

#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A program that reads queries from a text file and checks them against a 
relational database. The command to run the program is <executable configFile
Name queryFileName>. The 7 query commands are: "nattr" (which tells
the number of attributes for a certain relation in the db), "tuplen" (which
tells the length in bytes of each tuple in a specified relation), "infattr"
(which tells the type of a specified attribute, and its length in bytes), 
"count" (which tells the number of tuples in a specified relation), "project"
(which tells all the distinct values for a given attribute in a relation), 
"select" (which gives all tuples that satisfy some condition specified by
a query command), and "quit" (which ends the program). The structure of the
query (text) file is (on each line) a command (required), a relation (required
for all except "quit" command, an attribute (optional), a relational operator 
followed by either a string or an int value(optional). The results for each
of these queries is printed to stdout. The program does not handle invalid
command names, but it can determine whether an attribute belongs to a certain
relation or not. It assumes no errors in the config, schema, and data files. 
It also assumes no more than 10 attributes per relation, and at most 100 
tuples in a relation. The schema file is assumed to be the name of the 
relation followed by ".sch", and the data file is assumed to be the name of 
the relation followed by ".dat". If an erroneous query is encountered, the
program prints a message about the problem encountered, and continues 
processing the remainder of the queries in the file.  It assumes the query 
file contains a "quit" command.
*/

int main( int argc, char const *argv[] ){

	FILE *configFile;  /*Pointer to config file*/
	FILE *queryFile;  /*Poiner to query file*/
	char command[COMMAND_LENGTH];  /*A temp string for the query command*/
	char relName[MAX_STRING];  /*A temp string for the relation name*/
	char attrName[MAX_STRING];  /*A temp string for attribute name*/
	char relop[RELOP_LENGTH];  /*Array to hold relational operator*/
	char value[MAX_STRING];  /*Array to hold string to compare*/
	int numericValue;  /*Int to hold integer value to compare*/


	/*If the wrong number of arguments is given, print an error message and 
	exit.
	*/
	if( argc != NUM_ARGS){
		fprintf( stderr, "Incorrect number of arguments\n" );
		exit(1);
	}

	/*If the config file cannot be opened for reading, print an error message 
	and exit.
	*/
	if( (configFile = ( fopen( argv[CONFIG_FILE_ARG], "r"))) == NULL){
		fprintf( stderr, "There was an error trying to open config file %s\n",
				argv[CONFIG_FILE_ARG]);
		exit(1);
	}

	/*If the query file cannot be opened for reading, print an error message 
	and exit.
	*/
	if( (queryFile = ( fopen( argv[QUERY_FILE_ARG], "r"))) == NULL){
		fprintf( stderr, "There was an error trying to open query file %s\n",
				argv[QUERY_FILE_ARG]);
		exit(1);
	}


	/*Keep checking query file for another command
	*/
	while( fscanf( queryFile, "%s", command) == 1){

		/*If command entered is "nattr", get the relation name after the cmd, 
		and check to see if it is in the db. If so, call function to get the
		number of attributes for the specified relation. If not, print an 
		error message and move to the next query.
		*/
		if( strcmp( command, "nattr") == 0){
			fscanf( queryFile, "%s", relName);
			if( checkRelation( configFile, relName) == 1){
				nattr( relName);
			}
			else{
				printf(" Error: %s relation is not in the database.\n\n", 
						relName);
				fflush( stdout);
				continue;
			}
		}

		/*If command entered is "tuplen", get the relation name after the 
		cmd and check to see if it is in the db. If so, call the function to
		get the length of each tuple in this relation. If the relation is 
		not in the db, print an error message and move to the next query.
		*/
		else if( strcmp( command, "tuplen") == 0){
			fscanf( queryFile, "%s", relName);
			if( checkRelation( configFile, relName) == 1){
				tuplen( relName, 1);
			}
			else{
				printf(" Error: %s relation is not in the database.\n\n", 
						relName);
				fflush( stdout);
				continue;
			}

		}

		/*If command entered is "infattr", get the relation and attribute
		names after the cmd.  Check if the relation is in the database. If
		it is, call the function to check if the attribute is in that 
		relation (and if it is, print the type and number of bytes of the 
		attribute). If the relation is not in the db, print an error message
		and move to the next query.
		*/
		else if( strcmp( command, "infattr") == 0){
			fscanf( queryFile, "%s", relName);
			fscanf( queryFile, "%s", attrName);
			if( checkRelation( configFile, relName) == 1){
				infattr( relName, attrName, 1);
			}
			else{
				printf(" Error: %s relation is not in the database.\n\n", 
						relName);
				fflush( stdout);
				continue;
			}
		}

		/*If the command entered is "count", get the relation name after
		the cmd and check that it is a relation in the db. If so, call the
		function which prints the number of tuples in this relation. If the 
		relation is not in the db, print an error message and move to the 
		next query in the query file.
		*/
		else if( strcmp( command, "count") == 0){
			fscanf( queryFile, "%s", relName);
			if( checkRelation( configFile, relName) == 1){
				count( relName, 1);
			}
			else{
				printf(" Error: %s relation is not in the database.\n\n", 
						relName);
				fflush( stdout);
				continue;
			}
		}

		/*If command entered is "project", get the relation name and 
		attribute name after the cmd. Check that the relation is indeed
		a relation in the database. If so, call the function which prints
		all non duplicate values for this relation in the database. If this
		is not a relation in the database, print an error message and move
		on to the next query.
		*/
		else if( strcmp( command, "project") == 0){
			fscanf( queryFile, "%s", relName);
			fscanf( queryFile, "%s", attrName);
			if( checkRelation( configFile, relName) == 1){
				project( relName, attrName);
			}
			else{
				printf(" Error: %s relation is not in the database.\n\n", 
						relName);
				fflush( stdout);
				continue;
			}
		}

		/*If command entered is "select", get the relation name, attribute
		name, and relational operator that follow, and store their values.
		Check if relation is in db, then check that the attribute belongs
		to the relation. Then get the type. If the type is INT, then get
		the int from the next position in the query file, and set the string
		to "". If the type is STRING, then get the string from the next 
		position in the query file, and set the int to -1. In either case
		("I" or "S") call the function which prints out all tuples who
		satisfy the condition in the query. If this is not a relation in the 
		db, print an error message and go on to the next query.
		*/
		else if( strcmp( command, "select") == 0){
			fscanf( queryFile, "%s", relName);
			fscanf( queryFile, "%s", attrName);
			fscanf( queryFile, "%s", relop);
			/*If the relation is in db
			*/
			if( checkRelation( configFile, relName) == 1){
				/*If the attribute belongs to the relation
				*/
				if( (infattr( relName, attrName, 0)) == 1){
					/*If the relation type is "I"
					*/
					if( whatType( relName, attrName, 0) == 1){  /*INT type*/
						fscanf( queryFile, "%d", &numericValue); /*Store int*/
						/*Dummy value used to determine if we are trying to 
						compare an int and a string, later on in select 
						function
						*/
						strcpy( value, "\0");  
						select( relName, attrName, relop, numericValue, 
									value, "I");
					}
					/*If the relation type is "S"
					*/
					else{
						fscanf( queryFile, "%s", value);  /*Store string*/
						/*Dummy value used to determine if we are trying to 
						compare a string and an int, later on in select 
						function
						*/
						numericValue = -1;  
						select( relName, attrName, relop, numericValue, 
									value, "S");
					}
				}
			}
			/*If the relation is not in db, print an error message and go to 
			next query
			*/
			else{  
				printf(" Error: %s relation is not in the database.\n\n", 
						relName);
				fflush( stdout);
				continue;
			}
		}

		/*If the command entered is "quit", then close the files and exit the
		program.
		*/
		else if( strcmp( command, "quit") == 0){
			/*Close query file*/
			if( fclose( queryFile) == EOF){
				fprintf( stderr, "Error closing query file.\n");
				exit(1);
			}
			/*Close config file*/
			if( fclose( configFile) == EOF){
				fprintf( stderr, "Error closing config file.\n");
				exit(1);
			}
			exit(1);
		}
	}
	return 0;
}