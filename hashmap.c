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
    int bucket = hash(hm, word, document_id);
    //printf("bucket: %d\n", bucket);
    struct llnode *ptr = hm->map[bucket];
    //printf("word: %s\n", word);
    //printf("ptr->word in get; %s\n", ptr->word);
    struct docnode *docptr = ptr->doc;
    //printf("doc->doc_id in get; %s\n\n", docptr->document_id);
    //if the ptr->word is null then the list in empty
    //return -1
    if(ptr->word == NULL)
    {
        return -1;
    }

    //goes through the list checking if the word == ptr->word and document_id == ptr->document_id
    while(ptr->next != NULL)
    {
        //printf("1. ptr->word: %s word: %s\n", ptr->word, word);
        if(strcmp(ptr->word, word) == 0)
        {
            docptr = ptr->doc;
            //printf("go in here\n");
            while(docptr != NULL)
            {
                //printf("0. docptr_id: %s document_id: %s\n", docptr->document_id, document_id);
                if(strcmp(docptr->document_id, document_id) == 0)
                {
                    //printf("returns\n");
                    //if they are equal retrun the num_occurence
                    return docptr->num_occurrences;
                }
                docptr = docptr->doc_next;
            }
            
        }
        //iter the ptr
        ptr = ptr->next;
    }
    //printf("2. ptr->word: %s word: %s\n", ptr->word, word);
    if(strcmp(ptr->word, word) == 0)
    {
        docptr = ptr->doc; 
        //printf("check in here\n");
        while(docptr != NULL)
        {
            
            if(strcmp(docptr->document_id, document_id) == 0)
            {
                //if they are equal retrun the num_occurence
                //printf("return here\n");
                return docptr->num_occurrences;
            }
            docptr = docptr->doc_next;
        }
        
    }
   // printf("return -1\n");
    return -1;
    
}

void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences)
{
    setbuf(stdout, NULL);
    int sum = hash(hm, word, document_id);
    struct llnode *ptr = hm->map[sum];
    struct docnode *docptr = ptr->doc;

    //if the word + doc_id combo is not in the linked list
    if(num_occurrences == -1)
    {
        
        //if the linked list is empty
        if(ptr->word == NULL)
        {
            ptr->word = word;    
            docptr->document_id = document_id;  
            docptr->num_occurrences = 1;   
            hm->num_elements += 1;
            ptr->next = NULL;
            docptr->doc_next = NULL;
            printf("1. ptr->word: %s\n", ptr->word);
            printf("1. docptr->doc_id: %s\n", docptr->document_id);
            printf("1. num occurrence: %d\n\n", docptr->num_occurrences);
            return;
        }
        //checks to see if the word is already in the list but not the doc_id
        while(ptr->next != NULL)
        {   
            if(strcmp(ptr->word, word) == 0)
            {
                docptr = ptr->doc;
                while(docptr != NULL)
                {
                    printf("2. docptr: %s\n", docptr->document_id);
                    docptr = docptr->doc_next;
                }
                docptr = malloc(sizeof(struct docnode));
                docptr->document_id = document_id;
                docptr->num_occurrences = 1; 
                docptr->doc_next = NULL;
                hm->num_elements += 1;
                printf("2. ptr->word: %s word: %s\n", ptr->word, word);
                printf("2. docptr->doc_id: %s\n", docptr->document_id);
                printf("2. num occurrence: %d\n\n", docptr->num_occurrences);
                return;
            }
            
            ptr = ptr->next;
        }
       //checks the last node
        if(strcmp(ptr->word, word) == 0)
        {
            docptr = ptr->doc;
            while(docptr != NULL)
            {
                printf("3. docptr: %s\n", docptr->document_id);
                docptr = docptr->doc_next;
            }
            docptr = malloc(sizeof(struct docnode));
            docptr->document_id = document_id;
            docptr->num_occurrences = 1; 
            docptr->doc_next = NULL;
            hm->num_elements += 1;
            printf("3. ptr->word: %s word: %s\n", ptr->word, word);
            printf("3. docptr->doc_id: %s\n", docptr->document_id);
            printf("3. num occurrence: %d\n\n", docptr->num_occurrences);
            return;
        }
        //else then add the word to the end of the list
        else
        {

            printf("4.5. ptr->word: %s\n", ptr->word);
            printf("4.5. docptr->doc_id: %s\n", docptr->document_id);
            printf("4.5. num occurrence: %d\n\n", docptr->num_occurrences);
            ptr->next = malloc(sizeof(struct llnode));
            ptr = ptr->next;
            docptr = ptr->doc;
            ptr->word = word;
            ptr->next = NULL;
            hm->num_elements += 1;
            docptr = malloc(sizeof(struct docnode*));
            docptr->document_id = document_id;
            docptr->num_occurrences = 1; 
            docptr->doc_next = NULL;
            printf("4. ptr->word: %s\n", ptr->word);
            printf("4. docptr->doc_id: %s\n", docptr->document_id);
            printf("4. num occurrence: %d\n\n", docptr->num_occurrences);
        }
    }
    //else the word is already in the list
    else
    {
        while(ptr->next != NULL)
        {
            if(strcmp(ptr->word, word) == 0)
            {
                docptr = ptr->doc;
                //printf("ptr->word: %s word: %s\n", ptr->word, word);
                while(docptr->doc_next != NULL)
                {
                    //printf("0. docptr_id: %s document_id: %s\n", docptr->document_id, document_id);
                    printf("5. docptr: %s\n", docptr->document_id);
                    if(strcmp(docptr->document_id, document_id) == 0)
                    {
                        docptr->num_occurrences = docptr->num_occurrences + 1;
                        hm->num_elements += 1;
                        printf("5. ptr->word: %s\n word: %s", ptr->word, word);
                        printf("5. docptr->doc_id: %s\n", docptr->document_id);
                        printf("5. num occurrence: %d\n\n", docptr->num_occurrences);
                        return;
                    }
                    docptr = docptr->doc_next;
                    
                }
                //printf("1. docptr_id: %s document_id: %s\n", docptr->document_id, document_id);
                docptr = ptr->doc;
                if(strcmp(docptr->document_id, document_id) == 0)
                {
                    docptr->num_occurrences = docptr->num_occurrences + 1;
                    hm->num_elements += 1;
                    printf("6. ptr->word: %s\n word: %s", ptr->word, word);
                    printf("6. docptr->doc_id: %s\n", docptr->document_id);
                    printf("6. num occurrence: %d\n\n", docptr->num_occurrences);
                    return;
                }
            }
            //iters the ptr
            ptr = ptr->next;   
        }
        //checks the last one
        if(strcmp(ptr->word, word) == 0)
        {
            //printf("ptr->word: %s word: %s\n", ptr->word, word);
            docptr = ptr->doc;
            while(docptr->doc_next != NULL)
            {
                printf("7. docptr: %s\n", docptr->document_id);
                //printf("3. docptr_id: %s document_id: %s\n", docptr->document_id, document_id);   
                if(strcmp(docptr->document_id, document_id) == 0)
                {
                    docptr->num_occurrences = docptr->num_occurrences + 1;
                    hm->num_elements += 1;
                    printf("7. ptr->word: %s\n word: %s", ptr->word, word);
                    printf("7. docptr->doc_id: %s\n", docptr->document_id);
                    printf("7. num occurrence: %d\n\n", docptr->num_occurrences);
                    return;
                }
                docptr = docptr->doc_next;
                
            }
            //printf("4. docptr_id: %s document_id: %s\n", docptr->document_id, document_id);
            if(strcmp(docptr->document_id, document_id) == 0)
            {
                docptr->num_occurrences = docptr->num_occurrences + 1;
                hm->num_elements += 1;
                printf("8. ptr->word: %s\n word: %s", ptr->word, word);
                printf("8. docptr->doc_id: %s\n", docptr->document_id);
                printf("8. num occurrence: %d\n\n", docptr->num_occurrences);
                return;
            }
        }
    }
    
}


/*
void hm_remove(struct hashmap* hm, char* word, char* document_id)
{
    int bucket = hash(hm, word, document_id);
    int counter = 0;
    struct llnode *ptr = hm->map[bucket];
    struct llnode *ptr2 = hm->map[bucket];
    //first case
    if((strcmp(ptr->word, word) == 0) &&  ptr->doc->document_id != NULL)
    {
        if(strcmp(ptr->doc->document_id, document_id) == 0)
        {
            if(ptr->next == NULL)
            {
                //free(ptr->word);
                //free(ptr->document_id);
                ptr->word = NULL;
                ptr->doc->document_id = NULL;
                ptr->doc->num_occurrences = -1;
                ptr = NULL;
                hm->num_elements -= 1;
                return;
            }
            else
            {
                
                //free(ptr->word);
                //free(ptr->document_id);
                ptr->word = NULL;
                ptr->doc->document_id = NULL;
                ptr->doc->num_occurrences = -1;
                ptr = ptr2->next;
                hm->num_elements -= 1;
                return;
            }
        }
    }
    //middle case
    while(ptr->next != NULL)
    {
        
        if((strcmp(ptr->word, word) == 0) &&  ptr->doc->document_id != NULL)
        {
            
            if(strcmp(ptr->doc->document_id, document_id) == 0)
            {
                //free(ptr->word);
                //free(ptr->document_id);
                ptr->word = NULL;
                ptr->doc->document_id = NULL;
                ptr->doc->num_occurrences = -1;
                ptr2->next = ptr->next;
                hm->num_elements -= 1;
                return;
            }
            
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
        if(strcmp(ptr->doc->document_id, document_id) == 0)
        {
            //free(ptr->word);
            //free(ptr->document_id);
            ptr->word = NULL;
            ptr->doc->document_id = NULL;
            ptr->doc->num_occurrences = -1;
            ptr2->next = NULL;
            hm->num_elements -= 1;
            return;
        }
    }
    else
    {
        printf("Word, document id not found. \n");
        return;
    }
    
}
*/
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

int hash(struct hashmap* hm, char* word, char* document_id)
{
    
    int sum1 = 0;
    int docLength = strlen(document_id);
    //this does nothing... but the seas shell doesn't work
    //unless the vars are all used
    //if we were supposed to add the Document_id to the sum
    //it can easily be added by changing sum1 to sum
    int i;
    for(i = 0; i < docLength; i++)
    {
        sum1 += document_id[i];
    }
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
