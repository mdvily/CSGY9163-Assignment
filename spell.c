#include <stdlib.h>
#include "dictionary.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


bool check_word(const char* word, hashmap_t hashtable[])
{
   int bucket = hash_function(word);
   hashmap_t cursor = hashmap[bucket];

   while (cursor != NULL)
   {
      if (word == cursor->word)
      {
         return true;
      }
      cursor = cursor->next;
   }
   
   bucket = hash_function(word);
   cursor = hashmap[bucket];

   while(cursor != NULL)
   {
      if (tolower(word) == cursor->word)
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
   char word[LENGTH];

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
   
   //Read one word from dictionary file at a time
   fscanf(fp, "%s", word);
   
   while(word != EOF)
   {
      hashmap_t new_node;
      new_node->next = NULL;
      new_node->word = word;
      bucket = hash_function(word);
      if (hashtable[bucket] == NULL)
      {
         hashtable[bucket] = new_node;
      }
      else
      {
         new_node->next = hashtable[bucket];
         hashtable[bucket] = new_node;
      }
   }
   fclose(fp);
}


int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
   int num_misspelled = 0;
   //Source reference for getline() usage: https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline--
   char* line = NULL;
   size_t line_buf_size = 0;
   ssize_t line_size;
   char* word;

   //Get the first line
   line_size = getline(&line, &line_buf_size, fp);

   //Loop through the file
   while(line != EOF)
   {
      //Source reference for use of strtok(): https://stackoverflow.com/questions/4513316/split-string-in-c-every-white-space
      word = strtok(line, " ");
      while (word != NULL)
      {
         //TO DO: Remove punctuation from beginning and end of word
         if (!check_word(word, hashtable))
         {
	    misspelled[num_misspelled] = word;
	    num_misspelled++;
         }
         word = strtok(NULL, " ");
      }
      //Get next line
      line_size = getline(&line, &line_buf_size, fp)
   }

   //Clean up
   free(line);
   line = NULL;
   fclose(fp);

   return num_misspelled;
}
