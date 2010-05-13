/**********************************************************************************
**	Filename 		: vigenere.cpp	
**	Authors 		: Manu Kaul, Erik Ward, Johan Lindqvuist  
**  Last Modified	: Monday, 29 Mar 2010
**  
**  Description		: Vigenere Breaking related library of functions	
**
**********************************************************************************/

/* Header Includes */
#include "vigenere.h"

using namespace std;

/* Array for English Frequencies */
/* DO NOT MODIFY THE FREQUENCY VALUES !! */
float english_tmp[MAX_LETTERS] =  { 
							0.082, 0.015, 0.028, 0.043, 0.127, 0.022, /* A - F */
							0.020, 0.061, 0.070, 0.002, 0.008, 0.040, /* G - L */
							0.024, 0.067, 0.075, 0.019, 0.001, 0.060, /* M - R */
							0.063, 0.091, 0.028, 0.010, 0.023, 0.001, /* S - X */
							0.020, 0.001							  /* Y - Z */	
							};
									
/* Initialize Vector with array elements from above */
std::vector<float> eng_frequency (english_tmp, 
					english_tmp + sizeof(english_tmp) / sizeof(float) );

/* Globals */
int g_cipher_rows = 0;
int g_cipher_cols = 0;
vector< vector<int> > gCMatrix(g_cipher_rows, vector<int>(g_cipher_cols,FILLER));


/* Decryption using the caesar shift method */
vector<int> caesar_shift( vector<int> c_row, int alpha ){
	
	vector<int> decrypted_row;
	
	// Modulo 26 addition of each element in c_row to alpha 
	for( size_t i = 0; i < c_row.size(); i++ ){
		/* Skip the Row Fillers */
		if( c_row[i] != FILLER ){
			decrypted_row.push_back( (c_row[i] - alpha + MAX_LETTERS) % MAX_LETTERS );
		}	
	}
return decrypted_row;
}

/* 
How to guess the actual Key given the Key Length is correct

Initliase a KEY array.
position = 0;
<< f >>
Foreach substring y-i generated that ranges from 1 to m number of substrings 
	Foreach letter (Alpha) from A --> Z ( 0 --> 25 )
		Decrypt substring y-i using the letter "Alpha" with a Caesar Shift - Which means addition in modulo 26 form! 
		Measure the alphabetic probabilities of each alphabet from A-Z in the decrypted string.
		Compare this to the english distribution table using "Euclidean Squared Distance". (ESD)
		Keep track of the letter (Alpha) in the loop which has the MINIMUM value for ESD distance metric.
	end letter 
	KEY[position++] = Alpha with MINIMUM ESD
end substring

End of this loop you will have the Key broken and placed into the KEY Array.
*/
vector<int> guess_key(){
	
	vector<int> cipher_row(g_cipher_cols, FILLER);
	vector<int> decrypted_row(g_cipher_cols, FILLER);
	vector<int> frequency(MAX_LETTERS, 0); 
	vector<int> final_key;
	
	int   counter 		= 0;
	int   string_length = 0;
	int   alpha			= FILLER;
	int   min_alpha 	= FILLER;
	float min_dist		= LARGE_NUMBER;
	
	/* Processing Row Wise now to compute the IC */
	for(int i=0; i < g_cipher_rows; i++){
		counter = 0; 
		min_alpha 	= FILLER;
		min_dist	= LARGE_NUMBER;
			
  		for(int j=0; j < g_cipher_cols; j++)
			cipher_row[counter++] = gCMatrix[i][j];
		
		/* Now we have the row which was encrypted with the same KEY Alphabet. 
		 * Go through each letter to figure out the key! 
		 */
		for( alpha = 0; alpha < MAX_LETTERS; alpha++ ){
			
			/* Decrypt with letter - alpha the "cipher_row" array */
			decrypted_row = caesar_shift( cipher_row, alpha );
			
			string_length = 0;
			/* Calculate the Frequency of each letter/integer in the decrypted row */
			frequency =  frequency_counter( decrypted_row, string_length );	
			
			/* Compare the Decrypted Row's Frequency with the English Frequency Table 
			 * A distance measure is returned everytime this comparison is made to the 
			 * english probabilities table.
			 */
		 	float distance = distance_metric( frequency, string_length, eng_frequency );
			
			/* Get the least distance and the alphabet with least distance */
			if( distance < min_dist ){
				min_dist  = distance;
				min_alpha = alpha;
			}
		}	/* End of For Loop */
		/* Add the Key Alphabet to the Key Vector */
		final_key.push_back( min_alpha );
	}	/* End of Main For Loop */
return final_key;
}



/*
<< c >>
Foreach possible key length m= 2..10 loop   

	- Place array into a NEW multidimensional array of dimension (rows = m, columns = n/m) where n = total number of characters 
	- At this point we know that each ROW has been encrypted with the SAME ALPHABET OF THE KEY!
	- Compute the "IC" for each substring y-i, that is the ROW in the multi-dimensional array M ( do this for y-1 to y-m )  
	- Compare all the y-i substrings' IC values to check if it lies in the range (0.056 to 0.075) 
		if it all matches then 
            		you found the correct key length so store it and break out of the loop.
	- Delete the multidimensional array to save space!	
end loop << c >>
*/
int guess_key_length( vector<int> cipher ){
	
int key_length = MIN_KEY;	
float total_ic = 0.0;
float avg_ic   = 0.0;

/* Main Loop for guessing Key Length */
	for ( key_length = MIN_KEY; key_length < MAX_KEY; key_length++ ){	
		/* Reset */
		total_ic = 0.0;
		avg_ic 	 = 0.0;
		
		size_t max_rows = key_length;
		size_t max_cols = cipher.size() / max_rows ;
		
		/* Accomodate for the additional values */
		if( cipher.size() % max_rows > 0){
			max_cols = max_cols + 1;
		}
		
		/* Initialize a multi-dimensional vector with all zeros */
		vector< vector<int> > cipher_matrix(max_rows, vector<int>(max_cols, FILLER));
		vector<int> cipher_row(max_cols, FILLER);
		float ic = 0.0;
		size_t i=0, j=0;
		
		/* Allocate into a new multidimensional vector the cipher text array. */
		for( size_t idx = 0; idx < cipher.size(); idx++){
			if( idx > 0 and (idx % max_rows == 0) ){ j++; i = 0; }
			cipher_matrix[i++][j] = cipher[idx];	
		}
		/* Processing Row Wise now to compute the IC */
		for(size_t ii=0, counter=0; ii < max_rows; ii++){
			counter = 0; 	
	  		for(size_t jj=0; jj < max_cols; jj++){
				cipher_row[counter++] = cipher_matrix[ii][jj];
			}
	   		/* At this point we know that each row was encrypted with the same KEY ALPHABET! */
			ic = calc_ic( cipher_row );		/* Get the IC */
			
		total_ic += ic;
		}
		
		avg_ic = total_ic/(float)key_length;
		if( avg_ic >= MIN_IC_LIMIT and avg_ic <= MAX_IC_LIMIT ){ 
			g_cipher_rows 	= max_rows;
			g_cipher_cols 	= max_cols;
			gCMatrix		= cipher_matrix;
			break;			/* Criteria Matchesd so exit further processing */
		}
	}										/* End of For loop */	

if( key_length == MAX_KEY ){
	cout <<"The maximum key length of "<< MAX_KEY <<" was exhausted and key length could not be determined by IC method! "<< endl;
	exit (1);
}
return key_length;
}


float calc_ic( vector<int> row_string ){
	
	int string_length 	= 0;
	float ic 			= 0.0;
	vector<int> frequency(MAX_LETTERS, 0); 
	
	/* Calculate the Frequency of each letter and also get the actual length of string - Fillers */
	frequency = frequency_counter( row_string, string_length );	
	
	/* We now have an array with all the letter/integer frequencies */
	for( int j =0; j < MAX_LETTERS; j++ ){
		if( frequency[j] > 0)
			ic += frequency[j] * (frequency[j] - 1); 
	}
	
	/* Check for exceptions */
	if( (string_length *( string_length - 1 )) == 0 ){
		cout <<" The row string length is too small and analysis cannot be continued! " << endl;
		exit (1);
	}
	return ic/(float)(string_length *( string_length - 1 ));
}

/*
 * Decrypts the cipher string, using the periodic substitution cipher Vigenere,
 * if the message was encrypted using the key.
 */
void decrypt_vigenere( vector<int> cipher, vector<int> key, vector<char>& plaintext ){

	for( size_t i=0; i < cipher.size(); i++ )
		plaintext.push_back(  'a' + ( cipher[i] - key[i % key.size()] + MAX_LETTERS ) % MAX_LETTERS  );
}

/*
 * Calculate Clock Difference used for timing execution time
 * of program
 */
double diffclock(clock_t clock1, clock_t clock2)
{
	double diffticks= clock1 - clock2;
	double diffms=(diffticks * 10)/ CLOCKS_PER_SEC;
	return diffms;
}

/*
 * Read in the CIPHER text from an input file into an integer array
 * The conversion is done directly so there is no need to make 
 * conversions back and forth during the cracking process
 */
vector <int> read_cipher ( string in_cipher ){
	
	char c;
	vector<int> cipher;
	ifstream cfile;
	
	/* Open the Cipher File */
	//ifstream in( in_cipher );
	cfile.open (in_cipher.c_str(), ios::in);
	
	if( !cfile.is_open() ){
		cout <<" The file could not be opened, exiting program! "<< endl;
		exit (1);
	}
	
	while( cfile.get(c) ){
		// Discard newline characters 
		if(c == '\n' or c == '\r'){ /* Do Nothing */}
		else	
			cipher.push_back(c - ASCII_FOR_A);
	}		
	cfile.close();
	
return cipher;
}

/*
 * Read in the vector of integers and then perform a frequency count of 
 * how many times a certain integer is repeated and return the results
 */
vector<int> frequency_counter( vector<int> row_string, int& strlength ){
	
	vector<int> frequency(MAX_LETTERS, 0); 
	strlength = 0;
	
	int bulla = 0;
	
	/* Loop through the integer array to do a "frequency count" */
	for( size_t i = 0; i < row_string.size(); i++, bulla++ ){
		/* Skip the Row Fillers */
		if( row_string[i] != FILLER ){
			frequency[ row_string[i] ] += 1;
			++strlength;
		}	
	}
return frequency;	
}

/* 
 * Calculate the euclidean squared distance between the incoming vectors 
 * Euclidean Squared Distance = sqrt( sigma( square(x - y) )  )
 */
float distance_metric( vector<int> freq, int str_length, vector<float> eng_freq ){
	
	/* Loop through each element in the frequency, convert it to a 
	 * probability for comparison first. 
	 */
		float single_dist = 0.0;
		for( int i=0; i< MAX_LETTERS; i++ ){
			float x = (float)freq[i] / (float)str_length;	
			float y = eng_freq[i];							
			single_dist += (x-y)*(x-y);
		}
return sqrt(single_dist);
}

