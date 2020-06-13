#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

int main(int argc, char **argv) {
    
    int max_misspelled = 5;
    char *misspelled[max_misspelled];
    char teststr[5] = "test1";

    //create hashtable
    hashmap_t hashtable[HASH_SIZE];

    //load dictionary
    load_dictionary(argv[2], hashtable);

    //open file with test input
    FILE *fp = fopen(argv[1], "r");
 
    if (fp == NULL)
    {
       printf("fp is NULL");
    }    

    int num_misspelled = check_words(fp, hashtable, misspelled);
    
    printf("\nNumber of misspelled words: %d\n\n", num_misspelled);
    printf("\nMisspelled words: \n");
    for (int i = 0; i < num_misspelled; i++)
    {
       printf("\n%s", misspelled[i]);
    }    
    printf("\n\n");
    return 0;
}
