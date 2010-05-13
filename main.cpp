/**********************************************************************************
**	Filename 		: main.cpp	
**	Authors 		: CBro
**  Last Modified	: Monday, 29 Mar 2010
**  
**  Description		: Main program of the project that controls the order in which
**					  functions from the vigenere and generic library are called.
**					  Its dependencies --> vigenere.cpp 
**
**********************************************************************************/

/* Header Includes */
#include "vigenere.h"

using namespace std;

/* Help Usage */
void help() {
  cout << "typical: [ ./main (-c|--cipher) <cipher_file_name>] " << endl;
  cout << "  where: " << endl;
  cout << "  -c (--cipher) is the input cipher file to break" << endl;
}

int main(int argc, char* argv[]) {
	/* Variables */
	vector<int>  cipher_text;
	vector<char> plain_text;
	vector<int>  key;
	string i_cipher_file;
	
	int key_length = 0;
	
	/* Process the command line arguments */
	for(int i = 1; i < argc; ++i ){
		if (strcmp(argv[i], "-c") == 0 or strcmp(argv[i], "--cipher") == 0){
			if( i+1 == argc) {
				cout << "Invalid " << argv[i];
			    cout << " Parameter: No cipher file specified" << endl;
			    help();
			    exit(1); 
			}
			i_cipher_file = argv[++i]; 
		}
	}
	if (i_cipher_file.empty()) {
	    help();
	    exit(1);
	}
	cout << " Processing Cipher File  --->" << i_cipher_file << endl;
	
	/* Read in the cipher */
	cipher_text = read_cipher( i_cipher_file ); 
	
	/* Execution Time Measure */
	clock_t begin = clock();
	
	/* Guess Key Length */
	key_length = guess_key_length( cipher_text ); 
	cout << "The Key Length is --> " << key_length << endl;
	
	/* Guess the Actual Key */
	key = guess_key();
	cout << "The Actual Key is --> "; 
	/* Display the Key */
	for( size_t i=0; i< key.size(); i++){
		cout << static_cast<char>( key[i] + ASCII_FOR_A ) << " " ;
	}
	cout << endl;
	
	/* Decrypt the Cipher using the key just found */
	decrypt_vigenere( cipher_text, key, plain_text );
	clock_t end = clock();
	
	cout << "The Plaintext is --> " << endl; 
	/* Display the Plaintext !! */
	for( size_t i=0; i< plain_text.size(); i++){
		cout << plain_text[i] <<" " ;
	}
	cout << endl <<endl;
	cout << "Execution time: "<< diffclock(end,begin) <<" milliseconds."<<endl;

return 0;
}	

