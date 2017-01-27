/*-----------------------------------------------------------------------------------------------*/
// 0
/*-----------------------------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
/*-----------------------------------------------------------------------------------------------*/
// 1
/*-----------------------------------------------------------------------------------------------*/
struct no
{
	int token;
	int id;
	no* next;
	no* prev;
};
/*-----------------------------------------------------------------------------------------------*/
// 2
/*-----------------------------------------------------------------------------------------------*/
class Stack
{
public:

	// It is the constructor
	Stack();

	// It is the destructor
	~Stack();

	// The top of the stack receives a token/id
	void Push(int *token, int *id);

	// It returns the token pointed to by current
	void GetToken(int *token);

	// It returns the id pointed to by current
	void GetID(int *id);

	// It moves current up one no
	void MoveUp(void);

	// It moves current down one no
	void MoveDown(void);

	// It returns 0 if current points to last no;
	// It returns 1 if there is another no after current
	int  GetNext(void);

	// It deletes all nos
	void MakeEmpty(void);
/*-----------------------------------------------------------------------------------------------*/
// 3
/*-----------------------------------------------------------------------------------------------*/
private:
	no* head;		
	// top of the stack
	
	no* current;
	// selected no
	
	int length;
	// it is the counter of nos in stack
};
/*-----------------------------------------------------------------------------------------------*/
// 4
/*-----------------------------------------------------------------------------------------------*/
Stack::Stack()
{
	head = NULL;
	current = NULL;
	length = 0;
}
/*-----------------------------------------------------------------------------------------------*/
// 5
/*-----------------------------------------------------------------------------------------------*/
Stack::~Stack()
{
	MakeEmpty();
}
/*-----------------------------------------------------------------------------------------------*/
// 6
/*-----------------------------------------------------------------------------------------------*/
void Stack::Push(int *token, int *id)
{
	no* temp = new no;
	temp->token = *token;
	temp->id = *id;

	if (head == NULL)	// It means we are at the beginning of the stack
	{
		head = temp;
		temp->prev = NULL;
		temp->next = NULL;
		current = head;
	}
	else	
	{
		temp->prev = current;
		temp->next = NULL;
		current->next = temp;
		current = current->next;
	}
	length++;
}
/*-----------------------------------------------------------------------------------------------*/
// 7
/*-----------------------------------------------------------------------------------------------*/
void Stack::GetToken(int *token)
{
	*token = current->token;
}
/*-----------------------------------------------------------------------------------------------*/
// 8
/*-----------------------------------------------------------------------------------------------*/
void Stack::GetID(int *id)
{
	*id = current->id;
}
/*-----------------------------------------------------------------------------------------------*/
// 9
/*-----------------------------------------------------------------------------------------------*/
void Stack::MoveUp()
{
	current = current->next;
}
/*-----------------------------------------------------------------------------------------------*/
// 10
/*-----------------------------------------------------------------------------------------------*/
void Stack::MoveDown()
{
	current = current->prev;
}
/*-----------------------------------------------------------------------------------------------*/
// 11
/*-----------------------------------------------------------------------------------------------*/
int Stack::GetNext()
{
	if (current == NULL)
		return 0;
	if (current->next == NULL)
		return 0;

	return 1;
}
/*-----------------------------------------------------------------------------------------------*/
// 12
/*-----------------------------------------------------------------------------------------------*/
void Stack::MakeEmpty()
{
	current = head;
	while (head != NULL)
	{
		head = current->next;
		delete current;
		current = head;
	}
	length = 0;
}
/*-----------------------------------------------------------------------------------------------*/