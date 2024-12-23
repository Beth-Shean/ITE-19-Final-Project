#include <stdio.h>  //the standard input/output library, to use functions like printf, fgets, and fprintf.
#include <string.h> //for string-handling functions like strlen and strcpy

// Function prototypes, by their names, return types, and parameter types.
int romanToDecimal(char roman[], int *isValid);
void decimalToWords(int number, char words[]);
int performArithmetic(int num1, int num2, char operator);
void processLine(char line[], FILE *outputFile);
char toUpper(char c);

// Main function, entry point of the program
int main()
{
    FILE *inputFile, *outputFile; // pointers for reading from and writing to files
    char line[100];               // string to hold each line from the input file.

    inputFile = fopen("Input.txt", "r");   // Opens the input file in read mode ("r")
    outputFile = fopen("Output.txt", "w"); // Opens the output file in write mode ("w")

    if (inputFile == NULL || outputFile == NULL) // Checks if the files were successfully opened.
    {
        printf("Error opening file!\n"); // If not, it prints an error message
        return 1;                        // exits the program
    }

    // Process each line in the input file
    while (fgets(line, sizeof(line), inputFile)) // Reads the input file line by line into the line array.
    {
        // Trim leading and trailing whitespace
        char *start = line; // Moves the pointer to skip spaces or tabs at the beginning of the line.
        while (*start == ' ' || *start == '\t')
            start++;
        char *end = start + strlen(start) - 1; // Adjusts the end pointer to remove trailing spaces, tabs, and newline
        while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
            *end-- = '\0'; // replacing them with a null terminator, which marks the end of the string.

        // Process only if the trimmed line is not empty
        if (strlen(start) > 0)
        {
            processLine(start, outputFile); // calls the processLine function to write results to the output file.
        }
    }

    fclose(inputFile);  // Closes the input file.
    fclose(outputFile); // Closes the output file.

    printf("Finished processing! Results saved in Output.txt.\n"); // Displays success message
    return 0;                                                      // exits program
}

int romanToDecimal(char roman[], int *isValid)
{
    int values[256] = {0}; // Array to initialize Roman numeral characters to their decimal values.
    values['I'] = 1;       // Assigns the Roman numeral characters to the corresponding decimal values.
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0;     // Stores the sum of the decimal value
    int prevValue = 0; // Tracks the previous Roman numeral's value
    int count = 1;     // Tracks the count of consecutive identical numerals
    *isValid = 1;      // Set to 1 (valid) by default

    for (int i = strlen(roman) - 1; i >= 0; i--) // Loop through the Roman numeral from right to left
    {
        char currentChar = toUpper(roman[i]); // Convert to uppercase

        if (values[currentChar] == 0) // Check if the character is invalid
        {
            *isValid = 0; // Invalid Roman numeral detected, marks as invalid
            return 0;     // Stop processing
        }

        int currentValue = values[currentChar]; // Gets the decimal value of the current Roman numeral character.

        if (currentValue < prevValue) // Checks if smaller value precedes a larger value
        {
            total -= currentValue; // If so, it subtracts it
            count = 1;             // Reset count for new numeral
        }
        else // Otherwise,
        {
            if (currentValue == prevValue) // If the current value equals the previous value
            {
                count++;
                if ((currentValue == 1 || currentValue == 10 || currentValue == 100 || currentValue == 1000) && count > 3) // Check for invalid repetitions
                {
                    *isValid = 0; // Sets the validity flag to 0
                    return 0;     // Stop processing
                }
                if ((currentValue == 5 || currentValue == 50 || currentValue == 500) && count > 1) // Check for invalid repetitions
                {
                    *isValid = 0; // Sets the validity flag to 0
                    return 0;     // Stop processing
                }
            }
            else
            {
                count = 1; // Reset count for new numeral
            }
            total += currentValue; // Add value
        }
        prevValue = currentValue; // Update the previous value, to the current value
    }

    return total; // Return the sum of the decimal value
}

// Custom implementation of toupper
char toUpper(char c)
{
    if (c >= 'a' && c <= 'z') // Check if the character is lowercase
    {
        return c - ('a' - 'A'); // Convert to uppercase by subtracting ASCII difference
    }
    return c; // Return the character unchanged if already uppercase
}

// Function to convert a decimal number to its word equivalent
void decimalToWords(int number, char words[])
{
    if (number == 0) // Checks if number is zero
    {
        strcpy(words, "Zero"); // Writes the word, Zero, to the words array
        return;                // Ends the process
    }

    // Define arrays for units, teens, tens, and thousands words
    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million"};

    int part[4];     // Array to store the parts of the number
    int index = 0;   // Index to keep track of the current position in the part array
    words[0] = '\0'; // Initialize words as an empty string

    if (number < 0) // Checks if number is neagative
    {
        strcat(words, "Negative "); // Writes the word, Negative, to the words array
        number = -number;           // Makes the number positive
    }

    while (number > 0) // Splits the number into groups of 3 digits.
    {
        part[index++] = number % 1000; // Extract the last 3 digits of the number (thousands, hundreds, millions)
        number /= 1000;                // Reduce the number by a factor of 1000 for the next iteration
    }

    for (int i = index - 1; i >= 0; i--) // Loop through each part of the number (thousands, hundreds, millions)
    {
        int num = part[i]; // Get the current part

        if (num >= 100) // If the part is 100 or more
        {
            strcat(words, units[num / 100]); // Appends the hundreds word
            strcat(words, " Hundred ");      // Appends the word "Hundred"
            num %= 100;                      // Processes the remainder
        }

        if (num >= 10 && num < 20) // If the number is between 10 and less than 20 (teen numbers)
        {
            strcat(words, teens[num - 10]); // Appends the word from the "teens" array
            strcat(words, " ");             // Adds a space after the word
        }
        else // Otherwise
        {
            if (num >= 20) // If the number is 20 or more
            {
                strcat(words, tens[num / 10]); // Appends the word from the "tens" array
                strcat(words, " ");            // Adds a space after the word
            }
            num %= 10; // Processes the remainder

            if (num > 0) // If the number in the units place is non-zero (1-9)
            {
                strcat(words, units[num]); // Appends the word from the "unit" array
                strcat(words, " ");        // Adds a space after the word
            }
        }

        if (i > 0 && part[i] > 0) // If the part is not the last part and is greater than zero
        {
            strcat(words, thousands[i]); // Appends the word from the "thousands" array
            strcat(words, " ");          // Adds a space after the word
        }
    }
}

// Function to perform arithmetic operations based on the provided operator
int performArithmetic(int num1, int num2, char operator)
{
    switch (operator) // Switch case to handle different operators
    {
    case '+':               // If the operator is '+'
        return num1 + num2; // Return the sum of num1 and num2
    case '-':               // If the operator is '-'
        return num1 - num2; // Return the difference between num1 and num2
    case '*':               // If the operator is '*'
        return num1 * num2; // Return the product of num1 and num2
    case '/':               // If the operator is '/'
        // If num2 is not zero, return the quotient of num1 divided by num2
        return num2 != 0 ? num1 / num2 : 0; // Avoid division by zero
    default:                                // If the operator is invalid
        return 0;                           // Return 0 as a default case
    }
}

void processLine(char line[], FILE *outputFile)
{
    char roman1[50], roman2[50], operator; // Variables for the two Roman numerals and operator
    int num1, num2, result;                // Variables for decimal values and result
    char resultWords[150];                 // Array for the result in words
    int isValid1, isValid2;                // Flags to check if the Roman numerals are valid

    // Parse the line into two Roman numerals and an operator
    if (sscanf(line, "%s %c %s", roman1, &operator, roman2) == 3)
    {
        // Convert Roman numerals to decimal and check its validity
        num1 = romanToDecimal(roman1, &isValid1);
        num2 = romanToDecimal(roman2, &isValid2);

        // Check if either Roman numeral is invalid
        if (!isValid1 || !isValid2)
        {
            fprintf(outputFile, "Invalid Roman Numeral Input\n"); // Output an error
            return;
        }

        result = performArithmetic(num1, num2, operator); // Perform the arithmetic operation
        decimalToWords(result, resultWords);              // Convert result to words
        fprintf(outputFile, "%s\n", resultWords);         // Write the result to the output file
    }
    else // If the input format is incorrect
    {
        fprintf(outputFile, "Invalid input format\n"); // Output error message
    }
}
