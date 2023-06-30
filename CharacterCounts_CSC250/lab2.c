/**
 * This program reads a text file and prints the top 5 most frequent letters (upper or lower case)
 * and the top 5 most frequent digits that are in the file. If the file has less than 5 unique letters
 * and/or digits the top list is adjusted accordingly. The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Olivia Caulfield {@literal <caulog20@wfu.edu>}
 * @date Sept. 17, 2021
 * @assignment Lab 2
 * @course CSC 250
 **/

#include <ctype.h>
#include <stdio.h>

/* constant definition */
#define MAX_LIST_SIZE 26

/* struct definition */  
struct Char_Freq {
    char ch;
    int count;
};

/* function declarations */
int process_characters(char filename[], struct Char_Freq aList[], int *aPtr, struct Char_Freq dList[], int *dPtr);
int search(char ch, struct Char_Freq list[], int num);
void sortArray(struct Char_Freq list[], int num);
void printArrays(struct Char_Freq aList[], struct Char_Freq dList[], int aNum, int dNum);

/* Main function definition*/
int main(int argc, char* argv[])
{
    /* creates variables for alpha list and digit list */
    struct Char_Freq alphaList[MAX_LIST_SIZE];
    struct Char_Freq digitList[10];
    int alphaNum = 0;
    int digitNum = 0;

    int status = 0;
    if (argc < 2){
        printf("Usage: %s filename \n", argv[0]);
        return 1;
    }

    status = process_characters(argv[1], alphaList, &alphaNum, digitList, &digitNum);

    if (!status){
        printf("%s could not open file %s \n", argv[0], argv[1]);
        return 2;
    }

    /* sorts alpha list and digit list and prints them */
    sortArray(alphaList, alphaNum);
    sortArray(digitList, digitNum);
    printArrays(alphaList, digitList, alphaNum, digitNum);

    return 0;
}

/* processes all characters in file to create alpha and digit arrays */
int process_characters(char filename[], struct Char_Freq aList[], int *aPtr, struct Char_Freq dList[], int *dPtr)
{
    char ch;
    int loc;
    FILE* file_ptr = fopen(filename, "rie");
    int aNum = 0;
    int dNum = 0;

    if (file_ptr == 0){
        return 0; /*file pointer is invalid*/
    }
    
    ch = fgetc(file_ptr);
    while (ch != EOF){

        if (isalpha(ch)){
            ch = tolower(ch);
            loc = search(ch, aList, aNum);
            if (loc != -1){
                aList[loc].count++;
            }
            else{
                aList[aNum].ch = ch;
                aList[aNum].count = 1;
                aNum++;
            }
        }

        if (isdigit(ch)){
            loc = search(ch, dList, dNum);
            if (loc != -1){
                dList[loc].count++;
            }
            else{
                dList[dNum].ch = ch;
                dList[dNum].count = 1;
                dNum++;
            }
        }

        ch = fgetc(file_ptr);
    }
    fclose(file_ptr);
    *aPtr = aNum;
    *dPtr = dNum;

    return 1;
}

/* searches array for specific character and counts how many times it appears */
int search (char ch, struct Char_Freq list[], int num)
{
    int i;
    for (i = 0; i < num; i++){
        if (list[i].ch == ch){
            return i;
        }
    }
    return -1;
}
/* sorts the array in descending count order */
void sortArray(struct Char_Freq list[], int num)
{
    int i;
    int j;
    int tempCount;
    char tempChar;
    for (i = 0; i < num; i++){
        for (j = i + 1; j < num; j++){
            if (list[i].count < list[j].count){
                tempCount = list[i].count;
                tempChar = list[i].ch;
                list[i].count = list[j].count;
                list[i].ch = list[j].ch;
                list[j].count = tempCount;
                list[j].ch= tempChar;
            }
        }
    }
}

/* prints the alpha and digit arrays paying */
void printArrays(struct Char_Freq aList[], struct Char_Freq dList[], int aNum, int dNum)
{
    int i;
    
    /* checks length of array and format prints top 5 alpha (or less) */
    if (aNum > 4){
        printf("alpha top 5 out of %d\n", aNum);
        printf("------------------------------\n");
        for(i = 0; i < 5; i++){
        printf("[%c] %d\n", aList[i].ch, aList[i].count);
        }
    }
    else if (aNum > 0){
        printf("alpha top %d out of %d\n", aNum, aNum);
        printf("------------------------------\n");
        for(i = 0; i < aNum; i++){
            printf("[%c] %d\n", aList[i].ch, aList[i].count);
        }
    }
    else{
        printf("No alpha characters were found.\n------------------------------\n");
    }

    printf("\n");

    /* checks length of array and format prints top 5 digits (or less) */
    if (dNum > 4){
        printf("digit top 5 out of %d\n", dNum);
        printf("------------------------------\n");
        for (i = 0; i < 5; i++){
            printf("[%c] %d\n", dList[i].ch, dList[i].count);
        }
    }
    else if (dNum > 0){
        printf("digit top %d out of %d\n", dNum, dNum);
        printf("------------------------------\n");
        for (i = 0; i < dNum; i++){
            printf("[%c] %d\n", dList[i].ch, dList[i].count);
        }
    }
    else{
        printf("No digit characters were found.\n------------------------------\n");
    }
}