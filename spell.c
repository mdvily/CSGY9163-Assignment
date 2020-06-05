#include <stdlib.h>
#include "dictionary.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

bool check_word(const char* word, hashmap_t hashtable[])
{
   int i = 0;
   int bucket = hash_function(word);
   hashmap_t cursor = hashtable[bucket];

   char lowercase_word[strlen(word)];

   while (cursor != NULL)
   {
      if (!strcmp(word, cursor->word))
      {
         return true;
      }
      cursor = cursor->next;
   }
 

   //Convert string to lower case. Reference: https://stackoverflow.com/questions/2661766/how-do-i-lowercase-a-string-in-c/2661788
   for(i = 0; i < strlen(word); i++)
   {
      lowercase_word[i] = tolower(word[i]);
   }
   lowercase_word[i] = '\0';
   
   bucket = hash_function(lowercase_word);
   cursor = hashtable[bucket];

   while(cursor != NULL)
   {
      if (!strcmp(lowercase_word, cursor->word))
      {
         return true;
      }
      cursor = cursor->next;
   }
   
   return false;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
   int i, bucket;
   FILE *fp;
   //char*  word;
   char word[1024];

   //Initialize hash table
   for (i = 0; i < HASH_SIZE; i++)
   {
      hashtable[i] = NULL; //??hashtable[i]->word??
   }
   
   //Open dictionary file
   fp = fopen(dictionary_file, "r");
   if (fp == NULL){
      return false;
   }
   
   //Source reference for fscanf use: https://stackoverflow.com/questions/16400886/reading-from-a-file-word-by-word
   while(fscanf(fp, " %1023s", word) == 1)
   {
      hashmap_t new_node = malloc(sizeof(node));
      new_node->next = NULL;
      strcpy(new_node->word, word); //Source: https://www.programiz.com/c-programming/library-function/string.h/strcpy
      bucket = hash_function(word);
      if (hashtable[bucket] == NULL)
      {
         hashtable[bucket] = new_node;
	 new_node->next = NULL;
      }
      else
      {
         new_node->next = hashtable[bucket];
         hashtable[bucket] = new_node;
      }
   }
   fclose(fp);
   //free(new_node);
}


int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
   int test;
   int num_misspelled = 0;
   //Source reference for getline() usage: https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline--
   char* line = NULL;
   size_t line_buf_size = 0;
   ssize_t line_size;
   char* word;

   //Get the first line
   line_size = getline(&line, &line_buf_size, fp);

   word = strtok(line, " .,?!-\n");
   //Loop through the file
   while(line_size >= 0)
   {
      //Source reference for use of strtok(): https://stackoverflow.com/questions/4513316/split-string-in-c-every-white-space
      while (word != NULL)
      {
         //TO DO: Remove punctuation from beginning and end of word
         if (!check_word(word, hashtable))
         {
	    misspelled[num_misspelled] = word;
	    num_misspelled++;
         }
         word = strtok(NULL, " .,?!-\n");
      }
      //Get next line
      line = NULL;
      line_size = getline(&line, &line_buf_size, fp);
   }

   //Clean up
   free(line);
   line = NULL;
   fclose(fp);

   return num_misspelled;
}
