#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
struct hashmap* hm_create(int num_buckets)
{
    
    struct hashmap *new_hm = (struct hashmap*)malloc(sizeof(struct hashmap));
    new_hm->map = malloc(num_buckets * sizeof(struct llnode*));
    new_hm->num_buckets = num_buckets;
    new_hm->num_elements = 0;
    int i;
    for(i = 0; i < num_buckets; i++)
    {
        //malloc space for the node and set the word and next to null
        new_hm->map[i] = malloc(sizeof(struct llnode));
        new_hm->map[i]->word = NULL;
        new_hm->map[i]->next = NULL;
        new_hm->map[i]->document_frequency = 0;
        new_hm->map[i]->doc = malloc(sizeof(struct docnode));
        new_hm->map[i]->doc->document_id = NULL;
        new_hm->map[i]->doc->doc_next = NULL;
    }
    return new_hm;
    
}

int hm_get(struct hashmap* hm, char* word, char* document_id)
{
    //setbuf(stdout, NULL);
    //gets the hash key
    int bucket = hash(hm, word);
    struct llnode *ptr = hm->map[bucket];
    struct docnode *docptr = ptr->doc;

    //if the ptr->word is null then the list in empty
    //return -1
    if(ptr->word == NULL)
    {
        return -1;
    }

    //goes through the list checking if the word == ptr->word and document_id == ptr->document_id
    while(ptr != NULL)
    {
        if(strcmp(ptr->word, word) == 0)
        {
            docptr = ptr->doc;
            while(docptr != NULL)
            {
                if(strcmp(docptr->document_id, document_id) == 0)
                {
                    //if they are equal retrun the num_occurence
                    return docptr->num_occurrences;
                }
                docptr = docptr->doc_next;
            }
            
        }
        //iter the ptr
        ptr = ptr->next;
    }
    return -1;
    
}

void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences)
{
    setbuf(stdout, NULL);
    int sum = hash(hm, word);
    struct llnode *ptr = hm->map[sum];
    struct llnode *ptr2 = hm->map[sum];
    struct docnode *docptr = ptr->doc;
    struct docnode *dociter = ptr->doc;
    int count = 0;
    int num = 0;

    //if the word + doc_id combo is not in the linked list
    if(num_occurrences == -1)
    {
        
        //if the linked list is empty
        if(ptr->word == NULL)
        {
            ptr->word = word;    
            ptr->document_frequency = 1;
           // printf("1. df: %d\n", ptr->document_frequency);
            docptr->document_id = document_id;  
            docptr->num_occurrences = 1;   
            hm->num_elements += 1;
            ptr->next = NULL;
            docptr->doc_next = NULL;
            return;
        }
        //checks to see if the word is already in the list but not the doc_id
        while(ptr != NULL)
        {   
            if(strcmp(ptr->word, word) == 0)
            {
                docptr = ptr->doc;
                dociter = ptr->doc;
                while(docptr != NULL)
                {
                    docptr = docptr->doc_next;
                    //one behind
                    if(count >= 1)
                    {
                        dociter = dociter->doc_next;
                    }
                    count++;
                }
                ptr->document_frequency += 1;
               // printf("2. df: %d\n", ptr->document_frequency);
               // printf("word: %s\n", word);
                docptr = malloc(sizeof(struct docnode));
                docptr->document_id = document_id;
                docptr->num_occurrences = 1; 
                docptr->doc_next = NULL;
                dociter->doc_next = docptr;
                hm->num_elements += 1;
                return; 
            }
            ptr = ptr->next;
            if(num >= 1)
            {
                ptr2 = ptr2->next;
            }
            num++;
        }


        //else then add the word to the end of the list
        ptr = malloc(sizeof(struct llnode));
        ptr->word = word;
        ptr->next = NULL;
        ptr->document_frequency = 1;
        ptr2->next = ptr;
        hm->num_elements += 1;
        docptr = ptr->doc;
        docptr = malloc(sizeof(struct docnode*));
        docptr->document_id = document_id;
        docptr->num_occurrences = 1; 
        docptr->doc_next = NULL;
        ptr->doc = docptr;
    }
    //else the word is already in the list
    else
    {
        while(ptr != NULL)
        {
            if(strcmp(ptr->word, word) == 0)
            {
                docptr = ptr->doc;
                while(docptr != NULL)
                {
                    if(strcmp(docptr->document_id, document_id) == 0)
                    {
                        docptr->num_occurrences = docptr->num_occurrences + 1;
                        hm->num_elements += 1;
                        return;
                    }
                    docptr = docptr->doc_next;
                }
            }
            //iters the ptr
            ptr = ptr->next;   
        }
    }
}

void rmStopWord(struct hashmap* hm)
{
    int i;
    for(i = 0; i < 15; i++)
    {
        struct llnode *ptr = hm->map[i];
        //struct docnode *docptr = ptr->doc;

        while(ptr != NULL)
        {
            if(ptr->document_frequency == 3)
            {
                hm_remove(hm, ptr->word);
            }
            ptr = ptr->next;
        }  
            
    }
}

void hm_remove(struct hashmap* hm, char* word)
{
    int bucket = hash(hm, word);
    int counter = 0;
    struct llnode *ptr = hm->map[bucket];
    struct llnode *ptr2 = hm->map[bucket];
    struct docnode *docptr = ptr->doc;
    //struct docnode *docptr2 = ptr->doc;
    //first case
    if(strcmp(ptr->word, word) == 0)
    {
       
        if(ptr->next == NULL)
        {
            free(docptr);
            free(ptr->word);
            free(ptr);
            docptr = NULL;
            ptr->word = NULL;
            ptr = NULL;
            hm->num_elements -= 1;
            return;
        }
        else
        {
            ptr = ptr2->next;
            ptr2 = ptr2->next;
            free(ptr2);
            hm->num_elements -= 1;
            return;
        }
        
    }
    //middle case
    while(ptr->next != NULL)
    {
        
        if((strcmp(ptr->word, word) == 0))
        {
            //free(ptr->word);
            //free(ptr->document_id);
            docptr = ptr->doc;
            docptr->num_occurrences = 0;
            ptr2->next = ptr->next;
            ptr->next = NULL;
            free(ptr);
            hm->num_elements -= 1;
            return;
            
        }
        ptr = ptr->next;
        if(counter > 0)
        {
            ptr2 = ptr2->next;
        }
        counter++;
    }
    //last case
    if((strcmp(ptr->word, word) == 0) && ptr->doc->document_id != NULL)
    {
        ptr->word = NULL;
        ptr->doc->document_id = NULL;
        ptr->doc->num_occurrences = -1;
        ptr2->next = NULL;
        hm->num_elements -= 1;
        return;
        
    }
    else
    {
        printf("Word, document id not found. \n");
        return;
    }
    
}

void hm_destroy(struct hashmap* hm)
{
    //frees the hashmap
    int i;
    for(i = 0; i < 15; i++)
    {
        struct llnode *ptr = hm->map[i];
        
        if(ptr->word != NULL)
        {
            
            free(ptr);
        }
        
    }
    free(hm);
    printf("Memory freed \n");

}

int hash(struct hashmap* hm, char* word)
{
    int i;
    int sum = 0;
    int length = strlen(word);
    //adds the ASCII value of the word
    for(i = 0; i < length; i++)
    {
        sum += word[i];
    }
    sum = sum%hm->num_buckets;
    return sum;
    
}
