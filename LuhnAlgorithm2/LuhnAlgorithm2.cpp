// Namespace
using namespace std;

// Standard Includes
#include <iostream>
#include <string>
#include <cstring> // for strlen
#include <cstdlib> // for srand
#include <ctime> // for time

// External Includes
#include "CardData.h" // custom struct

// Constants
const int ITERATIONS = 10000; // in case you want to edit the number of times a card is checked
const int TOTAL_TYPES = 4; // in case you want to add more card types to the array
const CardData CARDTYPE[TOTAL_TYPES] =
{
	CardData("American Express", "3", 15),
	CardData("Visa", "4", 16),
	CardData("MasterCard", "5", 16),
	CardData("Discover", "6", 16)
}; // Constant array of card data

// Enums
enum CrdCard { American, Visa, Master, Discover, ALL }; // enum of all card types
// I could definitely get this to work without the enum, but I will use it because it was requested

// Function Prototypes
string StartupMenu(string="\0");
char GetCheckDigit(char*, int);
char* genCC(CrdCard);
void flipDig(char*);
bool validCC(char*);
// I'd rather use std::vector or std::array than pointer-based arrays, but I will do what is required of me
// If I am to use them, I'd rather they be declared as pointers than arrays, just because I prefer it

int main()
{
	srand(time(NULL)); // set random

	// get the choice of card
	string choice = StartupMenu();
	if (choice == "0")
		return -1;
	CrdCard current_type = (CrdCard)(choice[0] - 49);

	// check if you want to output the cards or just see the results
	bool display_cards = false;
	string input;
	cout << endl <<
		"Display all generated cards [y] [n]: ";
	getline(cin, input);
	if (input == "y")
		display_cards = true;
	cout << endl;

	// generate a number of cards and test if they are still valid after being modified
	// display the cards only if requested
	// dsplays the results at the end
	int valid_cards = 0;
	int invalid_cards = 0;
	for (int i = 0; i < ITERATIONS; i++)
	{
		char* current_card = genCC(current_type);
		flipDig(current_card);
		if (validCC(current_card))
			valid_cards++;
		else
			invalid_cards++;

		if (display_cards)
			cout << endl <<  i+1 << ": " << current_card;

		delete[] current_card;
	}
	cout << endl << endl <<
		"Total Valid Cards: " << valid_cards << endl <<
		"Total Invalid Cards: " << invalid_cards << endl;
	return 0;
}

// added functionality for a default menu argument
string StartupMenu(string default_arg)
{
	string choice = default_arg;
	cout << "Luhn Algorithm 2" << endl;
	// choose between one of the card types, all of them, or exit the program
	while (choice == "\0" || (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" && choice != "0"))
	{
		cout << endl << "Select a card type to generate " << ITERATIONS << " of:" << endl;
		int i;
		for (i = 0; i < TOTAL_TYPES; i++)
		{
			cout << "[" << i+1 << "] " << CARDTYPE[i].name << endl;
		}
		cout <<
			"[" << i+1 << "] ALL" << endl <<
			"[0] Exit" << endl;
		getline(cin, choice);
	}
	return choice;
}

// improved and adapted for c-strings
char GetCheckDigit(char* input, int length)
{
	int digit;
	int check = 0;
	// start from the position before the check digit and move backwards
	for (int i = length - 2; i >= 0; i--)
	{
		// convert character to integer
		digit = input[i] - 48;
		// this is true whenever the difference between the current index and the first is even, which is the same as every other digit
		if ((length - 2 - i) % 2 == 0)
		{
			digit *= 2;
			// subtracting 9 does the same thing as adding the digits for all two digit numbers
			if (digit > 9)
				digit -= 9;
		}
		check += digit;
	}
	// apply the modifications to the sum to get the check digit
	check = ((check * 9) % 10) + 48;
	return check;
}

char* genCC(CrdCard card_type)
{
	// if the CrdCard is ALL (beyond the length of the CardData array), pick a random card type
	if (card_type >= TOTAL_TYPES)
		card_type = (CrdCard)(rand() % TOTAL_TYPES);

	// make a new random card number by setting the prefix, filling the middile with random digits, putting the check digit after that, then null at the end
	char* digits = new char[CARDTYPE[card_type].length + 1];
	for (int i = 0; i < CARDTYPE[card_type].prefix.length(); i++)
		digits[i] = CARDTYPE[card_type].prefix[i];
	for (int i = CARDTYPE[card_type].prefix.length(); i < CARDTYPE[card_type].length - 1; i++)
		digits[i] = (rand() % 10) + 48;
	digits[CARDTYPE[card_type].length - 1] = GetCheckDigit(digits, CARDTYPE[card_type].length);
	digits[CARDTYPE[card_type].length] = '\0';
	return digits;
}

// it is a bit unclear as to which version you want, so you can have both
void flipDig(char* digits)
{
	int rand_index = rand() % strlen(digits);
	// set a random digit to something other than itself
	/*
	digits[rand_index] = (((digits[rand_index] - 48) + (rand() % 9) + 1) % 10) + 48;
	*/
	// set a random digit to a number from 0 to 9
	digits[rand_index] = (rand() % 10) + 48;
}

bool validCC(char* digits)
{
	// get the new check digit
	char check_digit = GetCheckDigit(digits, strlen(digits));
	// if the new and old check digits match, it is still valid
	if (check_digit == digits[strlen(digits) - 1])
		return true;
	else
		return false;
}