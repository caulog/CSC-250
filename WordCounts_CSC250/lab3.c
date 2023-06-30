/**
 * This program reads a text file and prints the top 5 most frequent letters (upper or lower case)
 * and the top 5 most frequent digits that are in the file. If the file has less than 5 unique letters
 * and/or digits the top list is adjusted accordingly. The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Olivia Caulfield {@literal <caulog20@wfu.edu>}
 * @date Sept. 29, 2021
 * @assignment Lab 3
 * @course CSC 250
 **/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* constant definition */
#define MAX_STRING_SIZE 20

/* struct definition */  
struct WordFreq {
    char* word;
    int count;
};

/* function declarations */
int process_characters(char filename[], struct WordFreq **listPtr, int *numPtr);
int search(char word[], struct WordFreq *list, int num);
void sortArray(struct WordFreq *list, int num);
void printArray(char filename[], struct WordFreq *list, int num, char filename1[]);

/* Main function definition*/
int main(int argc, char* argv[])
{
    /* creates variables for alpha list and digit list */
    struct WordFreq *list = NULL;
    int num = 0;
    int status = 1;
    int i;
    
    /*if 3 arguments arent given, prints error*/
    if (argc < 3){
        printf("Usage: %s filename frequency_file \n", argv[0]);
        return 1;
    }

    /*creates array, sorts array, prints array to file, prints unique words*/
    status = process_characters(argv[1], &list, &num);
    sortArray(list, num);
    printArray(argv[2], list, num, argv[1]);

    if (!status){
        printf("%s could not open file %s \n", argv[0], argv[1]);
        return 2;
    }

    for(i = 0; i < num; i++){
        if(list[i].word) free(list[i].word);
    }
    if(list) free(list);

    return 0;
}

/* processes all characters in file to create words */
int process_characters(char filename[], struct WordFreq **listPtr, int *numPtr)
{
    char ch;
    FILE* file_ptr = fopen(filename, "rie");
    char buffer[MAX_STRING_SIZE];

    int loc;
    int n = 0;
    struct WordFreq *tempList = NULL;
    int i;
    buffer[0] = '\0';

    if (file_ptr == 0){
        printf("could not open %s \n", filename);
        return 0; /*file pointer is invalid*/
    }
    
    ch = fgetc(file_ptr);

    /*fills array and updates frequency when same word is found*/
    while (ch != EOF)
    {
        if (isalpha(ch)){
            ch = tolower(ch);
            buffer[n] = ch;
            n++;
            buffer[n] = '\0';
        }

        else if (n > 0){
            loc = search(buffer, *listPtr, *numPtr);
            
            if (loc != -1){
                (*listPtr)[loc].count++;
            }
            
            else{
                 tempList = (struct WordFreq *)malloc((*numPtr+1)*sizeof(struct WordFreq));
                
                for(i = 0; i < *numPtr; i++){
                    tempList[i] = (*listPtr)[i];
                
                }
                tempList[i].word = (char *) malloc(strlen(buffer)+1);
                strcpy(tempList[i].word, buffer);
                tempList[i].count = 1;

                if(*listPtr) free(*listPtr);
                *listPtr = tempList;
                (*numPtr)++;
            }

            buffer[0] = '\0';
            n = 0;
        }

        ch = fgetc(file_ptr);
    }

    fclose(file_ptr);
    return 1;
}

/* searches array for specific character and counts how many times it appears */
int search (char word[], struct WordFreq *list, int num)
{
    int i;
    for (i = 0; i < num; i++){
        if (strcmp(word, list[i].word) == 0){
            return i;
        }
    }
    return -1;
} 

/*sorts array in descending order*/
void sortArray(struct WordFreq *list, int num){
    int i;
    int j;
    int tempCount;
    char *tempChar;
    for (i = 0; i < num; i++){
        for (j = i + 1; j < num; j++){
            if (list[i].count < list[j].count){
                tempCount = list[i].count;
                tempChar = list[i].word;
                list[i].count = list[j].count;
                list[i].word = list[j].word;
                list[j].count = tempCount;
                list[j].word = tempChar;
            }
        }
    }
}

/*prints the array and frequencys to the frequency file*/
void printArray(char filename[], struct WordFreq *list, int num, char filename1[]){
    int count = 0;
    int i;
    FILE* f = fopen(filename, "w");
    
    for(i = 0; i < num; i++){
        fprintf(f, "%s %d\n", list[i].word, list[i].count);
        count++;
    }

    fclose(f);
    printf("%s has %d unique words\n", filename1, count);
}