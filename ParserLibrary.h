/*-----------------------------------------------------------------------------------------------*/
// 0
/*-----------------------------------------------------------------------------------------------*/
/*first the types of lexemes*/
#define	LETTER			10
#define	DIGIT			11
#define	OPERATOR		12
#define	DELIMITER		13
#define	NEWLINE			14
#define	ILLEGAL			15
#define	SPECIAL_WORD	16
#define	NUMBER			17
#define	IDENTIFIER		18
/*-----------------------------------------------------------------------------------------------*/
// 1
/*-----------------------------------------------------------------------------------------------*/
/*second the states*/
#define	INITIAL_STATE	100
#define	ID_STATE		101
#define	NUMBER_STATE	102
/*-----------------------------------------------------------------------------------------------*/
// 2
/*-----------------------------------------------------------------------------------------------*/
/*last the symbols and relational operators*/
#define	PL	/* + */		346	
#define	MN	/* - */		347	
#define	MT	/* * */		348
#define	DV	/* / */		349
#define	AS	/* = */		350	
#define	EQ	/* == */	351	
#define	LS	/* < */		352
#define	LE	/* <= */	353
#define	GR	/* > */		354		
#define	GE	/* >= */	355		
#define	NE	/* != */	356		
#define	IN	/* ++ */	357		
#define	DC	/* -- */	358	
#define	CL	/* : */		359
#define	SC	/* ; */		360
#define CM  /* , */     361
#define LB  /* { */     362
#define RB  /* } */     363
/*-----------------------------------------------------------------------------------------------*/
// 3
/*-----------------------------------------------------------------------------------------------*/
struct SYMBOL
{
	char *ptr;
	bool is_sw;

} Symbol_table[1000];
/*-----------------------------------------------------------------------------------------------*/
// 4
/*-----------------------------------------------------------------------------------------------*/
// sets the special words
int BEGIN, END, IF, THEN, ELSE, GOTO, PRINT;
int INTEGER, LONG; //updated G2
/*-----------------------------------------------------------------------------------------------*/
// 5
/*-----------------------------------------------------------------------------------------------*/
// does whatever it is necessary so that the actual intended program starts
void initialize(void);
/*-----------------------------------------------------------------------------------------------*/
// 6
/*-----------------------------------------------------------------------------------------------*/
// takes the input and does recursive descent parsing
void proc(void);
/*-----------------------------------------------------------------------------------------------*/
// 7
/*-----------------------------------------------------------------------------------------------*/
// returns the next token and the next id of the input program
void lex(int *token, int *id);
// obs: when ungettoken is used, returns the next token from off the stack
/*-----------------------------------------------------------------------------------------------*/
// 8
/*-----------------------------------------------------------------------------------------------*/
// returns the token to the stack
void ungettoken(void);
/*-----------------------------------------------------------------------------------------------*/
// 9
/*-----------------------------------------------------------------------------------------------*/
void statementlist(void);             // updated G2
void enclosedstatements(void);        // updated G2
void statement(void);
void declarestatement(void);          // updated G2
void variablelist(void);              // updated G2
void assignstatement(void);
void expression(void);
void ifstatement(void);
void gotostatement(void);
void incstatement(void);
void decstatement(void);
void printstatement(void);
void labelstatement(void);
void cond(void);
/*-----------------------------------------------------------------------------------------------*/
// 10
/*-----------------------------------------------------------------------------------------------*/
// Puts an IDENTIFIER into the Symbol Table
int hash_table(char *identifier);
/*-----------------------------------------------------------------------------------------------*/