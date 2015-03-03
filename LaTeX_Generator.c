#include <stdio.h>
#include <stdlib.h>

main() {
	/*	Function Prototypes	*/
	int DecimalBitLength(int decimal);
	void DecimalToBitstring(int decimal, int bitstring[], int numBits, FILE *outputFile);
	int PerformModulation(int b, int bitstring[], int numElements, int modulus, FILE *outputFile);
	
	/*	Variables	*/
	int b = 7;	// b^n mod m
	int n = 644;
	int m = 645;
	int numBits = DecimalBitLength(n);	// Holds length of bistring of b
	printf("%d\n", numBits);	// Print the number of bits required to represent decimal b
	int result;	// Final result of exponential modulation
	int binary[numBits];	// Array of ints to hold binary representation
	// File	variables
	char outputFilename[] = "Output.tex";
	FILE *outputFile = fopen(outputFilename, "w");
	
	/*	Initial file output	*/
	fprintf(outputFile, "$b$ = %d, $n$ = %d, $m$ = %d\n", b, n, m);

	/*	Solve	*/
	DecimalToBitstring(n, binary, numBits, outputFile);	// Store binary representation of b in array binary
	// outputFile contains text converting to binary
	fprintf(outputFile, "\\\\\n\n");
	result = PerformModulation(b, binary, numBits, m, outputFile);
	fprintf(outputFile, "\n$%d^{%d} \\bmod %d = \\ans{%d}$",
		b, n, m, result);	// output result
	
	exit(EXIT_SUCCESS);
}

/*	Calculate the number of bits requred to represent int decimal	*/
int DecimalBitLength(int decimal) {
	int numberPlaces = 0;
	while(decimal > 0) {
		numberPlaces++;
		decimal /= 2;
	}

	return numberPlaces;
}

/*	Fill bitstring array of length numBits with the binary representaition of decimal	*/
void DecimalToBitstring(int decimal, int bitstring[], const int numBits, FILE *outputFile) {		
	int tmpNumBits = numBits;
	fprintf(outputFile, "\\begin{align*}\n");
	
	while(tmpNumBits > 0) {	
		bitstring[tmpNumBits] = decimal % 2;
		decimal /= 2;	// Divide decimal

		/*	Output tex markup	*/
		fprintf(outputFile, "\t%d / 2 &= %d\t&& %d",
			decimal * 2, decimal, bitstring[tmpNumBits]);
		if(tmpNumBits != 1)
			fprintf(outputFile, "\\\\");
		fprintf(outputFile, "\n");

		tmpNumBits--;	// Decrement array index
	}

	/*	Output complete bitstring	*/
	tmpNumBits = 1;
	fprintf(outputFile, "\\end{align*}\n");
	fprintf(outputFile, "So binary representation of $n$ is $");
	while(tmpNumBits <= numBits) {
		fprintf(outputFile, "%d", bitstring[tmpNumBits]);
		tmpNumBits++;
	}
	fprintf(outputFile, "_2$");
	
	return;	// Exit returning void
}

int PerformModulation(int b, int a[], int numElements, int m, FILE *outputFile) {
	int x = 1;
	int power = b % m;
	int tmp = numElements;
	
	fprintf(outputFile, "$x = %d$\t\\\\\n$\\text{power} = %d$",
		x, power);
	fprintf(outputFile, "\n\n");
		
	while(numElements > 0) {
		fprintf(outputFile, "$a_{%d} = %d$, ",
			tmp - numElements, a[numElements]);
		
		// calculate x
		if(a[numElements] == 1) {
			fprintf(outputFile, "$x = (%d * %d) \\bmod %d = ",
				x, power, m);
			x = (x * power) % m;
			fprintf(outputFile, "%d$; ", x);
		}
		else
			fprintf(outputFile, "$x = %d$; ", x);

		// calculate power
		if(numElements != 1) {
			fprintf(outputFile, "power = $%d^2 \\bmod %d = ", power, m);
			power = (power * power) % m;
			fprintf(outputFile, "%d$\\\\\n", power);
		}
		else
			fprintf(outputFile, "\\\\\n");
	
		// Decrement numElements
		numElements--;
	}

	return x;	// Return final remainder 
}
