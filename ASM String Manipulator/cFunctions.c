//Olivia Patterson
//ID: DK92177
//CMSC 313 Project 4
//This file contains the c functions used in the main program to manipulate the messages
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//Function to print out all the messages in format
void display(char *array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("Message[%d]: %s\n", i, array[i]);
    }
}

//Function to print stats of a selected message
void printStats(char *array[], int index){
    //set the string
    char *str = array[index];

    //error checking
    if (str == NULL){
        printf("Error: Invalid index or null string at index %d.\n", index);
    }

    //initialize variables
    int letters = 0;
    int digits = 0;
    int special = 0;
    int punct = 0;
    int total = 0;

    //loop through and tally up stats
    for (int i = 0; str[i] != '\0'; i++){
        if (isalpha(str[i])){
            letters++;
        }
        else if (isdigit(str[i])){
            digits++;
        }
        else if (str[i] == '!' || str[i] == '?' || str[i] == '.' || str[i] == ',' || str[i] == '\'' || str[i] == ':' || str[i] == ';' || str[i] == '-' || str[i] == '_' || str[i] == ')' || str[i] == '(' || str[i] == '{' || str[i] == '}' || str[i] == '[' || str[i] == ']') {
            punct++;
        }
        else {
            special++;
        }
    }

    //formatted printing of message stats
    total = (letters + digits + special + punct);
    printf("There are a total of %d characters in the text.\n", total);
    printf("There are %d letters.\n", letters);
    printf("There are %d digits.\n", digits);
    printf("There are %d special characters.\n", special);
    printf("There are %d punctuation characters.\n", punct);
}

//Function to read in user's input, validate string, and replace the string at that index
void readIn(char *array[], int index){
    //initialize variables
    int buffer = 256;
    int position = 0;
    char* cmd = malloc(sizeof(char)*buffer);
    int cha;
    int cont = 1;

    //take in user input and save it in cmd
    while (cont == 1){
        cha = fgetc(stdin);
        if (cha == EOF || cha == '\n'){
            cmd[position] = '\0';
            cont = 0;
        }
        else {
            cmd[position] = cha;
        }
        position++;

        //if pos exceeds buffer size, dynamically allocate more memory
        if (position >= buffer){
            cha = fgetc(stdin);
            buffer += 256;
            char* newCMD = realloc(cmd, buffer);
            cmd = newCMD;
        }
    }

    //validate the string
    if(strlen(cmd) > 1 && isupper(cmd[0]) && (cmd[strlen(cmd)-1] == '.' || cmd[strlen(cmd)-1] == '!' || cmd[strlen(cmd)-1] == '?')){
        array[index] = cmd;
    }
    //invalid string
    else {
        printf("Invalid message, keeping current.\n");
    }
}

//This function frees the memory allocated for the messages
void freeUp(char *messages){
    free(messages);
}

//Function to weave a message
void weave(char *array[], int index) {
    //variable initialization
    int length = strlen(array[index]);
    char *original = array[index];

    //print current message
    printf("Current message: %s\n", original);

    //if string <=2, no weaving necessary
    if (length <= 2) {
        printf("Weaved: %s\n", original);
        return;
    }

    //initialize temp variable
    char *temp = (char *) malloc(length + 1);

    //loop that performs message weaving
    int k = 0;
    for (int i = 0; i < length; i += 4) {
        if (i < length) {
            temp[k++] = original[i];
        }
        if (i + 2 < length) {
            temp[k++] = original[i + 2];
        }
        if (i + 1 < length) {
            temp[k++] = original[i + 1];
        }
        if (i + 3 < length) {
            temp[k++] = original[i + 3];
        }
    }
    //null terminate the string
    temp[k] = '\0';
    //overwrite to the newly weaved message
    array[index] = temp;

    //Check if the string in original was dynamically allocated, if yes, free
    if (strcmp(original, "This is the original message.") != 0) {
        free(original);
    }

    //print the weaved string
    printf("Weaved: %s\n", temp);
}

//Function to reverse a message
void reverse(char *array[], int index) {
    int number;

    // Ask the user for a number
    printf("Enter location value: ");
    scanf("%d", &number);

    // Get the string at the specified index
    char *str = array[index];

    // Validate split index
    if (number < 0 || number >= strlen(str)) {
        printf("Error: Invalid split index.\n");
        return;
    }

    // Display the current message
    printf("Current message: %s\n", str);

    // Allocate memory for the reversed string
    char *reversed = (char *) malloc(strlen(str) + 1);

    // Reverse first half of the string
    for (int i = 0; i < number / 2; i++) {
        char temp = str[i];
        str[i] = str[number - i - 1];
        str[number - i - 1] = temp;
    }

    // Reverse the second part of the string
    int len = strlen(str);
    for (int i = number; i < (number + len) / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1 + number];
        str[len - i - 1 + number] = temp;
    }

    // Copy the reversed string to the allocated memory
    strcpy(reversed, str);

    // Update the array at the specified index with the reversed string
    array[index] = reversed;

    // Print the reversed string
    printf("Reversed: %s\n", reversed);
}