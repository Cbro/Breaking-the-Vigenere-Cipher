/**********************************************************************************
**	Filename 		: vigenere.h	
**	Authors 		: CBro
**  Last Modified	: Monday, 29 Mar 2010
**  
**  Description		: Header file for vigenre.cpp with constants and 
**					  function prototypes.	
**
**********************************************************************************/
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>

#ifndef _VIGENERE_H
#define _VIGENERE_H

/* Constants */
const int   MIN_KEY			=  	1;
const int   MAX_KEY			= 	30;
/***** DO NOT MODIFY THE MIN AND MAX VALUES !! ******/
const float MIN_IC_LIMIT 	= 0.063;
const float MAX_IC_LIMIT 	= 0.078;
const float LARGE_NUMBER	= 99999.00;
const int 	ASCII_FOR_A 	= 	65;
const int   FILLER 			= 	-1;
const int   MAX_LETTERS 	= 	26;

// Functions
int guess_key_length( std::vector<int> cipher );
float calc_ic( std::vector<int> row_string );
std::vector<int> guess_key();
std::vector<int> caesar_shift( std::vector<int> c_row, int alpha );
void decrypt_vigenere( std::vector<int> cipher, std::vector<int> key, std::vector<char>& plaintext );

/* Utility Functions */
double diffclock(clock_t clock1, clock_t clock2);
std::vector <int> read_cipher( std::string in_cipher);
std::vector<int> frequency_counter( std::vector<int> row_string, int& strlength );
float distance_metric(  std::vector<int> frequency, int string_length, 	std::vector<float> eng_freq );

#endif

