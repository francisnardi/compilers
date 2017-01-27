/*-----------------------------------------------------------------------------------------------*/
// 0
/*-----------------------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_DEPRECATE
/*-----------------------------------------------------------------------------------------------*/
// 1
/*-----------------------------------------------------------------------------------------------*/
//basic libraries needed for this project
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ManagementStack.h"
#include "ParserLibrary.h"
/*-----------------------------------------------------------------------------------------------*/
// 2
/*-----------------------------------------------------------------------------------------------*/
using namespace std;

// variables declarations
int PRIME = 569;
int MAP[256];

// lines counter
int line_counter = 0;

// errors counter
int err_count = 0;

int state = INITIAL_STATE;
Stack TokenStack;
FILE* fp;
ofstream outs;
/*-----------------------------------------------------------------------------------------------*/
// 3
/*-----------------------------------------------------------------------------------------------*/
int main(void)
{
	initialize();	// does all the sort of initial things
	proc();	// starts the reading of the input

	fclose(fp);
	outs.close();   // closes the file
	cout << "file closed" << endl;
	exit(0);

} // this is the end of the program
/*-----------------------------------------------------------------------------------------------*/
// 4
/*-----------------------------------------------------------------------------------------------*/
void ungettoken()
{
	TokenStack.MoveDown();
}
/*-----------------------------------------------------------------------------------------------*/
// 5 --- OK
/*-----------------------------------------------------------------------------------------------*/
void proc(void)
{
	int token, id;
	lex(&token, &id);	// Get 'begin'

	if (!(token == SPECIAL_WORD && id == BEGIN))
	{
		cout << "Error: line " << line_counter << ". Program must start with 'begin'." << endl;
		outs << "Error: line " << line_counter << ". Program must start with 'begin'." << endl;
		err_count++;
	}

	statementlist();

	lex(&token, &id);	// Get 'end'
	if (!(token == SPECIAL_WORD && id == END))
	{
		cout << "Error: line " << line_counter << ". Program must terminate with 'end'." << endl;
		outs << "Error: line " << line_counter << ". Program must terminate with 'end'." << endl;
		err_count++;
	}

	if (err_count == 0)
	{
		cout << "Program compiled successfully - 0 errors." << endl;
		outs << "Program compiled successfully - 0 errors." << endl;
	}
	else
	{
		cout << "Compile unsuccessful - " << err_count << " errors." << endl;
		outs << "Compile unsuccessful - " << err_count << " errors." << endl;
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 6 --- OK
/*-----------------------------------------------------------------------------------------------*/
void statementlist()
{
	int token, id;
	lex(&token, &id);

	if (token == DELIMITER && id == '{')
	{
		statementlist();
		lex(&token, &id);
		if (token != DELIMITER || id != '}')	// needs to end in }
		{
			cout << "Error: line " << line_counter << ". Expected '}'. " << endl;
			outs << "Error: line " << line_counter << ". Expected '}'. " << endl;
			err_count++;
			ungettoken();
		}
		lex(&token, &id);
		if (token == SPECIAL_WORD && id == END){
			ungettoken();
			return;
		}
		else {
			ungettoken();
			statementlist();
		}
	}
	else
	{
		ungettoken();
		statement();
		lex(&token, &id);
		if (token == DELIMITER && id == ';')
		{
			ungettoken();
			statementlist();
		}
		else {
			ungettoken();
			return;
		}
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 7 --- OK
/*-----------------------------------------------------------------------------------------------*/
void enclosedstatements(){
	int token, id;
	lex(&token, &id);

	if (token == DELIMITER){
		if (id != '{'){
			cout << "Error: line " << line_counter << ". Expected '{'. " << endl;
			outs << "Error: line " << line_counter << ". Expected '{'. " << endl;
			err_count++;
		}
		statementlist();
	}
	else{
		ungettoken();
		statement();
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 8 --- OK
/*-----------------------------------------------------------------------------------------------*/
void statement()
{
	int token, id;
	lex(&token, &id);

	switch (token)
	{
	case SPECIAL_WORD:
		if (id == INTEGER | id == LONG){
			ungettoken();
			declarestatement();
		}
		if (id == IF)
		{
			ungettoken();
			ifstatement();
		}
		else if (id == GOTO)
		{
			ungettoken();
			gotostatement();
		}
		else if (id == PRINT)
		{
			ungettoken();
			printstatement();
		}
		break;

	case IDENTIFIER:
		lex(&token, &id);
		if (token == OPERATOR && id == AS)
		{
			//ungettoken();
			ungettoken();
			assignstatement();
		}
		else if (token == DELIMITER && id == CL)
		{
			//ungettoken();
			ungettoken();
			labelstatement();
		}
		else if (token == OPERATOR && id == IN)
		{
			//ungettoken();
			ungettoken();
			incstatement();
		}
		else if (token == OPERATOR && id == DC)
		{
			//ungettoken();
			ungettoken();
			decstatement();
		}
		break;

	default:
		cout << "Error: line " << line_counter << ". Unrecognized identifier.\n";
		outs << "Error: line " << line_counter << ". Unrecognized identifier.\n";
		err_count++;
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 9 --- OK
/*-----------------------------------------------------------------------------------------------*/
void declarestatement(){

	int token, id;
	lex(&token, &id);

	if (token != SPECIAL_WORD && (id != INTEGER || id != LONG)){
		cout << "Error: line " << line_counter << ". DECLARE statement expects is an integer or a long." << endl;
		outs << "Error: line " << line_counter << ". DECLARE statement expects is an integer or a long." << endl;
		err_count++;
	}
	variablelist();
}
/*-----------------------------------------------------------------------------------------------*/
// 10 --- OK
/*-----------------------------------------------------------------------------------------------*/
void variablelist(){

	int token, id;
	lex(&token, &id);

	if (token != IDENTIFIER){
		cout << "Error: line " << line_counter << "Expected identifier.\n";
		outs << "Error: line " << line_counter << "Expected identifier.\n";
		err_count++;
		ungettoken();
	}

	lex(&token, &id);
	if (token == DELIMITER && token == ','){
		variablelist();
	}

	else {
		ungettoken();
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 11 --- OK
/*-----------------------------------------------------------------------------------------------*/
void assignstatement()
{
	int token, id;
	lex(&token, &id);

	if (token != IDENTIFIER){
		cout << "Error: line " << line_counter << ". Expected identifier." << endl;
		outs << "Error: line " << line_counter << ". Expected identifier." << endl;
		err_count++;
		ungettoken();
	}
	lex(&token, &id);
	
	if (token != OPERATOR && id != AS){
		cout << "Error: line " << line_counter << ". ASSIGN statement must be composed by '='." << endl;
		outs << "Error: line " << line_counter << ". ASSIGN statement must be composed by '='." << endl;
		err_count++;
		ungettoken();
	}
	expression();
}
/*-----------------------------------------------------------------------------------------------*/
// 12 --- OK
/*-----------------------------------------------------------------------------------------------*/
void expression()
{
	int token, id;
	lex(&token, &id);

	if (token != NUMBER && token != IDENTIFIER)
	{
		cout << "Error: line " << line_counter << ". Expected number or identifier." << endl;
		outs << "Error: line " << line_counter << ". Expected number or identifier." << endl;
		err_count++;
		ungettoken();
	}
	
	lex(&token, &id);
	
	if (token == OPERATOR && (id == PL || id == MN || id == DV || id == MT))
	{
		expression();
	}
	else
	{
		ungettoken();
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 13 --- OK
/*-----------------------------------------------------------------------------------------------*/
void ifstatement()
{
	int token, id;
	lex(&token, &id);

	if (token == SPECIAL_WORD && id == IF) {
		cond();
		lex(&token, &id);

		if (token != SPECIAL_WORD || id != THEN)
		{
			cout << "Error: line " << line_counter << ". Expected 'then'." << endl;
			outs << "Error: line " << line_counter << ". Expected 'then'." << endl;
			err_count++;
		}
		enclosedstatements();
		lex(&token, &id);
		if (token == SPECIAL_WORD && id == ELSE)
			enclosedstatements();
		else
			ungettoken();
	}
	else
	{
		cout << "Error: line " << line_counter << ". This is an IF statement." << endl;
		outs << "Error: line " << line_counter << ". This is an IF statement." << endl;
		err_count++;
		ungettoken();
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 14 --- OK
/*-----------------------------------------------------------------------------------------------*/
void gotostatement()
{
	int token, id;
	lex(&token, &id);

	if (token != SPECIAL_WORD || id != GOTO){
		cout << "Error: line " << line_counter << ". This is a GOTO statement." << endl;
		outs << "Error: line " << line_counter << ". This is a GOTO statement." << endl;
		err_count++;
	}

	if (token != IDENTIFIER || token != NUMBER)
	{
		cout << "Error: line " << line_counter << ". GOTO statement expects an identifier." << endl;
		outs << "Error: line " << line_counter << ". GOTO statement expects an identifier." << endl;
		err_count++;
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 15 --- OK
/*-----------------------------------------------------------------------------------------------*/
void incstatement()
{
	int token, id;
	lex(&token, &id);

	if (token != IDENTIFIER){
		cout << "Error: line " << line_counter << ". INC statement expects an identifier." << endl;
		outs << "Error: line " << line_counter << ". INC statement expects an identifier." << endl;
		err_count++;
	}

	lex(&token, &id);

	if (token != OPERATOR || id != IN){
		cout << "Error: line " << line_counter << ". INC statement must be composed by '++'." << endl;
		outs << "Error: line " << line_counter << ". INC statement must be composed by '++'." << endl;
		err_count++;
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 16 --- OK
/*-----------------------------------------------------------------------------------------------*/
void decstatement()
{
	int token, id;
	lex(&token, &id);

	if (token != IDENTIFIER){
		cout << "Error: line " << line_counter << ". DEC statement expects an identifier." << endl;
		outs << "Error: line " << line_counter << ". DEC statement expects an identifier." << endl;
		err_count++;
	}

	lex(&token, &id);

	if (token != OPERATOR || id != DC){
		cout << "Error: line " << line_counter << ". DEC statement must be composed by '--'." << endl;
		outs << "Error: line " << line_counter << ". DEC statement must be composed by '--'." << endl;
		err_count++;
	}
}

/*-----------------------------------------------------------------------------------------------*/
// 17 --- OK
/*-----------------------------------------------------------------------------------------------*/
void printstatement()
{
	int token, id;
	lex(&token, &id);

	if (token != SPECIAL_WORD || id != PRINT){
		cout << "Error: line " << line_counter << ". This is a PRINT statement." << endl;
		outs << "Error: line " << line_counter << ". This is a PRINT statement." << endl;
		err_count++;
	}

	if (token != IDENTIFIER || token != NUMBER)
	{
		cout << "Error: line " << line_counter << ". PRINT statement expects identifier or number." << endl;
		outs << "Error: line " << line_counter << ". PRINT statement expects identifier or number." << endl;
		err_count++;
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 18 --- OK
/*-----------------------------------------------------------------------------------------------*/
void labelstatement()
{
	int token, id;
	lex(&token, &id);

	if (token != IDENTIFIER){
		cout << "Error: line " << line_counter << ". LABEL statement expects that IDENTIFIER comes first." << endl;
		outs << "Error: line " << line_counter << ". LABEL statement expects that IDENTIFIER comes first." << endl;
		err_count++;
	}

	lex(&token, &id);

	if (token != DELIMITER || id != CL){
		cout << "Error: line " << line_counter << ". LABEL statement expects that COLON comes second." << endl;
		outs << "Error: line " << line_counter << ". LABEL statement expects that COLON comes second." << endl;
		err_count++;
	}

	lex(&token, &id);

	if (token == DELIMITER && (id == ';' || id == '}') || (token == SPECIAL_WORD && id == END)){
		ungettoken();
	}
	else {
		ungettoken();
		statement();
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 19 --- OK
/*-----------------------------------------------------------------------------------------------*/
void cond()
{
	int token, id;
	lex(&token, &id);

	if (token == NUMBER && (id == 0 || id == 1))
		return;

	else {
		ungettoken();
		expression();
		lex(&token, &id);	// gets operator
		if (token == OPERATOR && (id == EQ || id == NE || id == LS || id == GR ||
			id == LE || id == GE))
		{
			expression();
		}
		else {
			cout << "Error: line " << line_counter << ". Improper expression on right side of conditional." << endl;
			cout << "Error: line " << line_counter << ". Improper expression on right side of conditional." << endl;
			err_count++;
		}
	}
}
/*-----------------------------------------------------------------------------------------------*/
// 20
/*-----------------------------------------------------------------------------------------------*/
int hash_table(char *identifier)
{
	int hash_value, id_value = 0, idx = 0;

	while (identifier[idx] != '\0')
	{
		id_value = id_value + identifier[idx] * 256 + identifier[idx + 1];
		idx++;
		if (identifier[idx] == '\0') break;
		idx++;
	}

	hash_value = id_value % PRIME;

	while (Symbol_table[hash_value].ptr != '\0')	// trying to find an empty table entry
	{
		if (strcmp(Symbol_table[hash_value].ptr, identifier) == 0)	// the identifier already exists
			return hash_value;

		if (hash_value < 1000)	// arrays' length limitation
			hash_value++;
		else
			hash_value = 0;
	}

	Symbol_table[hash_value].ptr = new char[10];
	strcpy(Symbol_table[hash_value].ptr, identifier);
	return hash_value;
}
/*-----------------------------------------------------------------------------------------------*/
// 21
/*-----------------------------------------------------------------------------------------------*/
void initialize(void)
{
	outs.open("result.txt");
	cout << "Output will be stored at 'result.txt'\n";

	char filename[64];
	printf("Please type the name of the file: ");
	fgets(filename, 63, stdin);
	filename[strlen(filename) - 1] = '\0';

	if ((fp = fopen(filename, "r")) == NULL)
	{
		cerr << "Program input file not found " << filename << endl;
		exit(1);
	}

	int i;

	// ASCII table reservation
	for (i = 0; i < 256; i++)
		MAP[i] = ILLEGAL;

	// letters reservation
	for (i = 0; i < 26; i++)
	{
		MAP['a' + i] = LETTER;
		MAP['A' + i] = LETTER;
	}

	// digits reservation
	for (i = 0; i < 10; i++)
		MAP['0' + i] = DIGIT;

	// operator and delimiter identification
	MAP['+'] = OPERATOR;
	MAP['-'] = OPERATOR;
	MAP['='] = OPERATOR;
	MAP['<'] = OPERATOR;
	MAP['>'] = OPERATOR;
	MAP['!'] = OPERATOR;
	MAP[':'] = DELIMITER;
	MAP[';'] = DELIMITER;
	MAP[' '] = DELIMITER;
	MAP['\t'] = DELIMITER;
	MAP['{'] = DELIMITER;
	MAP['}'] = DELIMITER;
	MAP['\n'] = NEWLINE;

	// it does hashing of special w. into symbol table
	int idx;
	idx = hash_table("begin");
	Symbol_table[idx].is_sw = true;
	BEGIN = idx;

	idx = hash_table("end");
	Symbol_table[idx].is_sw = true;
	END = idx;

	idx = hash_table("if");
	Symbol_table[idx].is_sw = true;
	IF = idx;

	idx = hash_table("else");
	Symbol_table[idx].is_sw = true;
	ELSE = idx;

	idx = hash_table("then");
	Symbol_table[idx].is_sw = true;
	THEN = idx;

	idx = hash_table("goto");
	Symbol_table[idx].is_sw = true;
	GOTO = idx;

	idx = hash_table("print");
	Symbol_table[idx].is_sw = true;
	PRINT = idx;

	idx = hash_table("integer");          //updated G2 INTEGER
	Symbol_table[idx].is_sw = true;
	PRINT = idx;

	idx = hash_table("long");             //updated G2 LONG
	Symbol_table[idx].is_sw = true;
	PRINT = idx;
}
/*-----------------------------------------------------------------------------------------------*/
// 22
/*-----------------------------------------------------------------------------------------------*/
void lex(int *token, int *id)
{
	if (TokenStack.GetNext() == 1)	// if a previous token is on the stack
	{
		TokenStack.MoveUp();
		TokenStack.GetID(id);
		TokenStack.GetToken(token);
		return;
	}
	else	// gets new token from the input
	{
		int c, ch, temptoken, tempid;
		int id_length = 0, num_length = 0;

		/*identifiers no more than 10 in length*/
		/*numbers with at most 5 digits*/
		char ID_buf[10], NUM_buf[5];

		cout.setf(ios::left);
		outs.setf(ios::left);

		c = getc(fp);
		ch = MAP[c];
		while (c != EOF)
		{
			if (state == INITIAL_STATE)
			{
				// Buffers and counters initialization
				id_length = 0;
				num_length = 0;
				for (int k = 0; k < 10; k++)
					ID_buf[k] = '\0';
				for (int k = 0; k < 5; k++)
					NUM_buf[k] = '\0';
			}

			switch (ch)
			{
			case LETTER:
				if (state == INITIAL_STATE || state == ID_STATE)
				{
					state = ID_STATE;
					id_length++;
					/*identifiers should have no more than 10 in length*/
					if (id_length > 10)
					{
						cout << "identifiers should have no more than 10 in length" << endl;
						outs << "identifiers should have no more than 10 in length" << endl;
						exit(1);
					}
					ID_buf[id_length - 1] = c;
				}
				else
				{
					cout << "any illegal char was found!" << endl;
					outs << "any illegal char was found!" << endl;
					exit(1);
				}
				break;

			case DIGIT:
				if (state == INITIAL_STATE || state == NUMBER_STATE)
				{
					state = NUMBER_STATE;
					/*numbers must be with at most 5 digits*/
					num_length++;
					if (num_length > 5)
					{
						cout << "numbers must be with at most 5 digits" << endl;
						outs << "numbers must be with at most 5 digits" << endl;
						exit(1);
					}
					NUM_buf[num_length - 1] = c;
				}
				if (state == ID_STATE)
				{
					/*identifiers should have no more than 10 in length*/
					id_length++;
					if (id_length > 10)
					{
						cout << "Identifier exceeded 10 character limit." << endl;
						outs << "Identifier exceeded 10 character limit." << endl;
						exit(1);
					}
					ID_buf[id_length - 1] = c;
				}
				break;

			case NEWLINE:
				line_counter++;

			case DELIMITER:
				if (state == ID_STATE)
				{
					int hash = hash_table(ID_buf);
					*id = hash;
					if (Symbol_table[hash].is_sw)
					{
						// SPECIAL WORD found
						cout << setw(15) << "SPECIAL WORD" << setw(10) << hash
							<< setw(15) << ID_buf << endl;
						outs << setw(15) << "SPECIAL WORD" << setw(10) << hash
							<< setw(15) << ID_buf << endl;
						*token = SPECIAL_WORD;
					}
					else
					{
						// IDENTIFIER found
						cout << setw(15) << "IDENTIFIER" << setw(10) << hash
							<< setw(15) << ID_buf << endl;
						outs << setw(15) << "IDENTIFIER" << setw(10) << hash
							<< setw(15) << ID_buf << endl;
						*token = IDENTIFIER;
					}
					TokenStack.Push(token, id);

					if (c == ';')		// we need to keep track of ';'
					{
						temptoken = DELIMITER;
						tempid = ';';
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();

						cout << setw(15) << "DELIMITER " << setw(10) << c << setw(5) << char(c) << endl;
						outs << setw(15) << "DELIMITER " << setw(10) << c << setw(5) << char(c) << endl;
					}

					state = INITIAL_STATE;

					return;
				}
				else if (state == NUMBER_STATE)
				{
					// NUMBER found
					cout << setw(15) << "NUMBER " << setw(10) << NUM_buf << endl;
					outs << setw(15) << "NUMBER " << setw(10) << NUM_buf << endl;
					*id = atoi(NUM_buf);
					*token = NUMBER;
					TokenStack.Push(token, id);

					if (c == ';')		// we need to keep track of ';'
					{
						temptoken = DELIMITER;
						tempid = ';';
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();

						cout << setw(15) << "DELIMITER " << setw(10) << c << setw(5) << char(c) << endl;
						outs << setw(15) << "DELIMITER " << setw(10) << c << setw(5) << char(c) << endl;
					}

					state = INITIAL_STATE;

					return;
				}

				if (c == ':' || c == ';' || c == '{' || c == '}')
				{
					cout << setw(15) << "DELIMITER " << setw(10) << c << setw(5) << char(c) << endl;
					outs << setw(15) << "DELIMITER " << setw(10) << c << setw(5) << char(c) << endl;
					*id = c;
					*token = DELIMITER;
					TokenStack.Push(token, id);

					state = INITIAL_STATE;

					return;
				}

				state = INITIAL_STATE;
				break;

			case ILLEGAL:
				state = INITIAL_STATE;
				// Announce lexical error
				cout << "any illegal char was found!" << endl;
				outs << "any illegal char was found!" << endl;
				exit(1);
				break;

			case OPERATOR:
				if (state == ID_STATE)
				{
					// Announce identifier
					cout << setw(15) << "IDENTIFIER" << setw(10) << hash_table(ID_buf)
						<< setw(15) << ID_buf << endl;
					outs << setw(15) << "IDENTIFIER" << setw(10) << hash_table(ID_buf)
						<< setw(15) << ID_buf << endl;

					*token = IDENTIFIER;
					*id = hash_table(ID_buf);
					TokenStack.Push(token, id);
				}
				else if (state == NUMBER_STATE)
				{
					// Announce NUMBER
					cout << setw(15) << "NUMBER " << setw(10) << NUM_buf << endl;
					outs << setw(15) << "NUMBER " << setw(10) << NUM_buf << endl;

					*token = NUMBER;
					*id = (atoi(NUM_buf));
					TokenStack.Push(token, id);
				}
				state = INITIAL_STATE;

				switch (c)
				{
				case '+':
					c = getc(fp);
					if (c == '+')	// It means that actually it is an increment, ++
					{
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< IN << setw(5) << "++" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< IN << setw(5) << "++" << endl;

						temptoken = OPERATOR;
						tempid = IN;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					else			// It means that it is a simple + instead of an increment
					{
						ungetc(c, fp);
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< PL << setw(5) << "+" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< PL << setw(5) << "+" << endl;

						temptoken = OPERATOR;
						tempid = PL;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					return;

				case '-':
					c = getc(fp);
					if (c == '-')	// It means that actually it is a decrement, --
					{
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< DC << setw(5) << "--" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< DC << setw(5) << "--" << endl;

						temptoken = OPERATOR;
						tempid = DC;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					else			// It means that it is a simple - instead of a decrement
					{
						ungetc(c, fp);
						// Announce OPERATOR
						cout << setw(15) << "OPERATOR" << setw(10)
							<< MN << setw(5) << "-" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< MN << setw(5) << "-" << endl;

						temptoken = OPERATOR;
						tempid = MN;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					return;

				case '=':
					c = getc(fp);
					if (c == '=')	// It means that actually it is a comparison, ==
					{
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< EQ << setw(5) << "==" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< EQ << setw(5) << "==" << endl;

						temptoken = OPERATOR;
						tempid = EQ;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					else			// It means that it is an assign
					{
						ungetc(c, fp);
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< AS << setw(5) << "=" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< AS << setw(5) << "=" << endl;

						temptoken = OPERATOR;
						tempid = AS;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					return;

				case '>':
					c = getc(fp);
					if (c == '=')	// It means that actually it is a greater or equal to signal, >=
					{
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< GE << setw(5) << ">=" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< GE << setw(5) << ">=" << endl;

						temptoken = OPERATOR;
						tempid = GE;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					else			// It is a simple greater than signal, >
					{
						ungetc(c, fp);
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< GR << setw(5) << ">" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< GR << setw(5) << ">" << endl;

						temptoken = OPERATOR;
						tempid = GR;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					return;

				case '<':
					c = getc(fp);
					if (c == '=')	// It means that actually it is a less or equal to signal, <=

					{
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< LE << setw(5) << "<=" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< LE << setw(5) << "<=" << endl;

						temptoken = OPERATOR;
						tempid = LE;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					else			// It is a simple less than signal, <
					{
						ungetc(c, fp);
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< LS << setw(5) << "<" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< LS << setw(5) << "<" << endl;

						temptoken = OPERATOR;
						tempid = LS;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					return;

				case '!':
					c = getc(fp);
					if (c == '=')	// It means that actually it is a not equal signal, !=
					{
						// OPERATOR found
						cout << setw(15) << "OPERATOR" << setw(10)
							<< NE << setw(5) << "!=" << endl;
						outs << setw(15) << "OPERATOR" << setw(10)
							<< NE << setw(5) << "!=" << endl;

						temptoken = OPERATOR;
						tempid = NE;
						TokenStack.Push(&temptoken, &tempid);
						TokenStack.MoveDown();
					}
					else
					{
						ungetc(c, fp);
						// Announce error
						cout << "the char wasn't recognized!" << endl;
						outs << "the char wasn't recognized!" << endl;
						exit(1);
					}
					return;
				} // End switch 2
			} // End switch 1

			c = getc(fp);
			ch = MAP[c];

		} // End while
	} // End else
}
/*-----------------------------------------------------------------------------------------------*/