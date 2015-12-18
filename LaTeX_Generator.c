/* Performs exponential modulation of unsigned integers
 * Each step is written to the local file Output.tex
 *
 * The binary conversion of the power is stored as a linked list
 */

// Preprocessor directives
#include <stdio.h>
#include <stdlib.h>

// Define node for linked list
struct node {
	char bit;		// Holds node's value
	struct node *prev;	// Points to prev node
	struct node *next;	// Points to next node
};

// Function prototypes
struct node* MakeNode(char value, struct node *before);	// Create new node
void DeleteList(struct node *head);	// Delete linked list
void DecimalToBitstring(unsigned decimal, struct node *head, FILE *outputFile);	// Convert [decimal] to bitstring
unsigned PerformModulation(unsigned b, struct node *tail, unsigned m, FILE *outputFile);	// Ouput exponential modulation

// Main method
main()
{
	unsigned b, n, m;	// b^n mod m
	unsigned result;	// Final result of exponential modulation
	struct node *head;	// Pointer directed at first node in list
	char outputFilename[] = "Output.tex";
	FILE *outputFile = fopen(outputFilename, "w");

	head = MakeNode('z', NULL);	// Create first node with prev = NULL

	// Get user input
	printf("Enter b, n, and m: ");
	scanf("%u %u %u", &b, &n, &m);

	// Initial file output
	fprintf(outputFile, "\\(b\\) = %d, \\(n\\) = %d, \\(m\\) = %d\n", b, n, m);

	// Store n's binary representation in list
	DecimalToBitstring(n, head, outputFile);

	// outputFile now contains steps of binary conversion
	fprintf(outputFile, "\\\\\n\n");

	result = PerformModulation(b, head, m, outputFile);
	fprintf(outputFile, "\nSo \\(%d^{%d} \\bmod %d = \\ans{%d}\\)", // Write final result
		b, n, m, result);

	// Free memory used by list
	DeleteList(head);

	// Exit cleanly
	exit(EXIT_SUCCESS);
}

// Method to create new node
struct node* MakeNode(char value, struct node *before)
{
	struct node *newNode;	// Create new node pointer
	newNode = (struct node*)malloc(sizeof(struct node));	// Allocate memory for new node

	if(newNode == NULL)
	{	// Insufficient memory
		printf("Node creation failed.\n");
		return NULL;	// Return with NULL pointer
	}

	newNode->bit = value;		// Store num as node's value
	newNode->prev = before;		// Point back in list
	newNode->next = NULL;		// Next element is NULL

	return newNode;
}

// Delete all nodes in list
void DeleteList(struct node *head)
{
	struct node *ptr;	// New node pointer
	ptr = head->next;	// Point to first node

	// Free memory up to last node
	while(ptr->next)
	{
		free(head);	// Delete node
		head = ptr;	// Advance head
		ptr = ptr->next;// Advance ptr
	}

	// Free final node
	free(ptr);
}

// Storebitstring representation of decimal in linked list
// Returns length of that binary representation
void DecimalToBitstring(unsigned decimal, struct node *head, FILE *outputFile)
{
	struct node *ptr = head;	// Point new pointer at head

	fprintf(outputFile, "\\begin{align*}\n");	// Begin TeX align environment

	// Loop while decimal is greater than 0
	while(decimal != 0)
	{
		ptr->bit = '0' + (decimal % 2);	// Store remainder as char in list
		decimal /= 2;			// Divide decimal

		// Output tex markup
		fprintf(outputFile, "\t%d / 2 &= %d\t&& %c",
			decimal * 2, decimal, ptr->bit);
		if(decimal >= 1)	// More than one iteration left
			fprintf(outputFile, "\\\\");
		fprintf(outputFile, "\n");

		ptr->next = MakeNode('z', ptr);	// Create new end of list
		ptr = ptr->next;		// Point to end of list
	}

	// Go to most significant bit
	ptr = ptr->prev;

	// Output complete bitstring
	fprintf(outputFile, "\\end{align*}\n");
	fprintf(outputFile, "So binary representation of \\(n\\) is \\(");

	while(ptr)	// Loop backwards until beginning of list
	{
		fprintf(outputFile, "%c", ptr->bit);	// Print out bit
		ptr = ptr->prev;			// Go to next node
	}
	fprintf(outputFile, "_2\\)");

	return;	// Exit returning void
}

// Write process of calculating x and power to output file
// Return final remainder
unsigned PerformModulation(unsigned b, struct node *head, unsigned m, FILE *outputFile)
{
	struct node *ptr;	// Create new pointer, point at start of list
	unsigned x = 1;		// Instantiate x with value 1
	unsigned power = b % m;	// Power = b mod m
	int currentPosition;	// Used for subscript of a in TeX output


	// Write initial values of x, power
	fprintf(outputFile, "\\(x = %d\\)\t\\\\\n\\(\\text{power} = %d\\)\n",
		x, power);
	// Begin center and tabular environments
	fprintf(outputFile, "\\begin{center}\n\\begin{tabular}{l r|l r|l r}\n");

	// Loop through list backwards
	ptr = head;		// Now pointing at LSB
	currentPosition = 0;	// Begin at a_0

	while(ptr->next)
	{

		// Output value of a_currentPosition
		fprintf(outputFile, "\t\\(a_{%d} =\\)&\t\\(%c\\)&\t",
			currentPosition, ptr->bit);

		// Calculate x
		if(ptr->bit == '1')
		{
			fprintf(outputFile, "\\(x = (%d * %d) \\bmod %d =\\)&\t",
				x, power, m);
			x = (x * power) % m;
			fprintf(outputFile, "\\(%d\\)&\t", x);
		}
		else
			fprintf(outputFile, "\\(x =\\)&\t \\(%d\\)&\t", x);

		// Calculate power
		if(ptr->next->next)
		{
			fprintf(outputFile, "power = \\(%d^2 \\bmod %d =\\)&\t", power, m);
			power = (power * power) % m;
			fprintf(outputFile, "\\(%d\\)\\\\\n", power);
		}
		else
			fprintf(outputFile, "\\\\\n");

		currentPosition++;	// Increment currentPosition
		ptr = ptr->next;	// Advance to next node
	}
	fprintf(outputFile, "\\end{tabular}\n\\end{center}");	// End center environment

	return x;	// Return final remainder
}
