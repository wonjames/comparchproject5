#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
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
        //malloc space for doc and set the doc_id to NULL and the next to NULL
        new_hm->map[i]->doc = malloc(sizeof(struct docnode));
        new_hm->map[i]->doc->document_id = NULL;
        new_hm->map[i]->doc->doc_next = NULL;
    }
    return new_hm;
    
}

double rank(struct hashmap *hm, char* word, char* document_id)
{
    int bucket = hash(hm, word);
    struct llnode *ptr = hm->map[bucket];
    struct docnode *docptr = ptr->doc;
    //inverse document frequency
    double idf = 0;
    //tf-idf sum
    double sum = 0;
    //document frequency
    double df;

    //searches for the word
    while(ptr != NULL)
    {
        if(strcmp(ptr->word, word) == 0)
        {
            //df is how many documents have the word
            df = (double)ptr->document_frequency; 
            //gets the inverse function of the document frequency
            idf = log10(3.0/df);
            //sets docptr to the ptr->doc
            //makes it point to the right docnode linked list
            docptr = ptr->doc;
            while(docptr != NULL)
            {
                if(strcmp(document_id, docptr->document_id) == 0)
                {
                    //times the idf by the number of times it appears in the document
                    sum = idf * docptr->num_occurrences;
                    return sum;
                }
                docptr = docptr->doc_next;
            }
        }
        ptr = ptr->next;
    }
    return 0;
}


int hm_get(struct hashmap* hm, char* word, char* document_id)
{
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
    //if it wasn't one those loops then in it wasn't in the hashmap already
    //return -1
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
        //add to the head
        if(ptr->word == NULL)
        {
            ptr->word = word;    
            ptr->document_frequency = 1;
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
            //checks to see if the ptr->word is equal to word
            if(strcmp(ptr->word, word) == 0)
            {
                //sets docptr + dociter to point to the right node
                docptr = ptr->doc;
                dociter = ptr->doc;
                while(docptr != NULL)
                {
                    docptr = docptr->doc_next;
                    //dociter is one behind
                    if(count >= 1)
                    {
                        dociter = dociter->doc_next;
                    }
                    count++;
                }
                //adds 1 to the doc_freq since a new doc is added
                ptr->document_frequency += 1;
                //malloc space for the docptr
                //initiallize the attributes
                docptr = malloc(sizeof(struct docnode));
                docptr->document_id = document_id;
                docptr->num_occurrences = 1; 
                docptr->doc_next = NULL;
                dociter->doc_next = docptr;
                hm->num_elements += 1;
                return; 
            }
            //iter the ptrs
            ptr = ptr->next;
            if(num >= 1)
            {
                ptr2 = ptr2->next;
            }
            num++;
        }

        //else then add the word to the end of the list
        //malloc memory for ptr and docptr
        ptr = malloc(sizeof(struct llnode));
        ptr->word = word;
        ptr->next = NULL;
        ptr->document_frequency = 1;
        ptr2->next = ptr;
        hm->num_elements += 1;
        docptr = ptr->doc;
        docptr = malloc(sizeof(struct docnode));
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
            //checks to see if the word equals ptr->word
            if(strcmp(ptr->word, word) == 0)
            {
                docptr = ptr->doc;
                while(docptr != NULL)
                {
                    //checks to see if docptr->doc_id is equal to doc_id
                    if(strcmp(docptr->document_id, document_id) == 0)
                    {
                        //add 1 to the num_occurence
                        docptr->num_occurrences = docptr->num_occurrences + 1;
                        hm->num_elements += 1;
                        //free the word that entered in since its not being used
                        free(word);
                        return;
                    }
                    //iter docptr
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
    //goes through the entire hashmap and finds the nodes
    //with the frequency of three and calls remove
    
    for(i = 0; i < 15; i++)
    {
        struct llnode *ptr = hm->map[i];

        if(ptr->word != NULL)
        {
            while(ptr != NULL)
            {
                //ASSUMES that only 3 docs in the directory
                if(ptr->document_frequency == 3)
                {
                    //printf("check");
                    hm_remove(hm, ptr->word);
                }
                ptr = ptr->next;
            }  
        }
        
            
    }
    
}

void hm_remove(struct hashmap* hm, char* word)
{
    setbuf(stdout, NULL);
    int bucket = hash(hm, word);
    int counter = 0;
    struct llnode *ptr = hm->map[bucket];
    struct llnode *ptr2 = hm->map[bucket];
    struct docnode *docptr = ptr->doc;

    //first case
    if(strcmp(ptr->word, word) == 0)
    {
        //if theres no other nodes in the list
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
        //if theres atleast one other node in the list
        else
        {
            ptr = ptr2->next;
            docptr = ptr2->doc;
            //free the memory
            free(docptr);
            free(ptr2->word);
            free(ptr2);
            //set the attributes to null
            docptr = NULL;
            //ptr2->word = NULL;
            ptr2 = NULL;
            hm->map[bucket] = ptr;
            hm->num_elements -= 1;
            return;
        }
        
    }
    //middle case
    while(ptr->next != NULL)
    {
        //if its the same word free the memory
        if((strcmp(ptr->word, word) == 0))
        {
            ptr2->next = ptr->next;
            docptr = ptr->doc;
            //free memory
            free(docptr);
            free(ptr->word);
            free(ptr);
            //set to null
            docptr = NULL;
            ptr->word = NULL;
            ptr = NULL;

            hm->num_elements -= 1;
            return;
            
        }
        //iters the ptrs
        //keeps ptr2 one behind
        ptr = ptr->next;
        if(counter > 0)
        {
            ptr2 = ptr2->next;
        }
        counter++;
    }
    //last case
    if(strcmp(ptr->word, word) == 0)
    {
        docptr = ptr->doc;
        //free memory
        free(docptr);
        free(ptr->word);
        free(ptr);
        //sets to null
        docptr = NULL;
        ptr->word = NULL;
        ptr = NULL;
        hm->num_elements -= 1;
        return;
        
    }
    //the word was not in the list
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
        struct llnode *temp = hm->map[i];
        struct docnode *docptr = ptr->doc;
        struct docnode *docptr2 = ptr->doc;
        if(ptr->word != NULL)
        {
            //free the ptrs
            while(ptr != NULL)
            {
                temp = ptr->next;
                //frees the docptrs
                while(docptr != NULL)
                {
                    docptr2 = docptr->doc_next;
                    free(docptr);
                    docptr = NULL;
                    docptr = docptr2;
                }
                free(ptr->word);
                free(ptr);
                ptr = NULL;
                ptr = temp;
            }

        }
        
    }
    //free the hashmap
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
