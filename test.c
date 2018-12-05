#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashmap.h"

char str[1000];
char arr[1000];
char arr2[1000];
int printList(struct hashmap* hm);

int main(void)
{
        struct hashmap *hm = hm_create(15);
        
        FILE *file;
        FILE *file1;
        FILE *file2;

        char* d1 = (char*)malloc(sizeof(char) *5);
        char* d2 = (char*)malloc(sizeof(char) *5);
        char* d3 = (char*)malloc(sizeof(char) *5);
        d1 = "D1";
        d2 = "D2";
        d3 = "D3";

        file = fopen("D1.txt", "r");
        if(file == NULL)
        {
                printf("file not found");
                return 0;
        }
        file1 = fopen("D2.txt", "r");
        if(file1 == NULL)
        {
                printf("file not found");
                return 0;
        }
        file2 = fopen("D3.txt", "r");
        if(file2 == NULL)
        {
                printf("file not found");
                return 0;
        }

        int num;
        while(fgets(str, 1000, file) != NULL)
        {
                //setbuf(stdout, NULL);
                //printf("test");
                char *token = strtok(str, " \t\n");
                
                
                while(token != NULL)
                {
                        char *temp = (char*)malloc(sizeof(char)*strlen(token) + 1);
                        strcpy(temp, token);
                        printf("Temp: %s\n", temp);
                        num = hm_get(hm, temp, d1);
                        printf("Num: %d\n", num);
                        hm_put(hm, temp, d1, num);
                        //free(temp);
                        token = strtok(NULL, " \t\n");
                }
        }
        
        while(fgets(arr, 1000, file1) != NULL)
        {
                char *token = strtok(arr, " ");
                while(token != NULL)
                {
                        char *temp = (char*)malloc(sizeof(char)*strlen(token) + 1);
                        strcpy(temp, token);
                        printf("Temp: %s\n", temp);
                        num = hm_get(hm, temp, d2);
                        printf("Num: %d\n", num);
                        hm_put(hm, temp, d2, num);
                        //free(temp);
                        token = strtok(NULL, " ");
                }
        }
        
        while(fgets(arr2, 1000, file2) != NULL)
        {
                char *token = strtok(arr2, " ");
                

                while(token != NULL)
                {
                        char *temp = (char*)malloc(sizeof(char)*strlen(token) + 1);
                        strcpy(temp, token);
                        printf("Temp: %s\n", temp);
                        num = hm_get(hm, temp, d3);
                        printf("num: %d\n", num);
                        hm_put(hm, temp, d3, num);
                        //free(temp);
                        token = strtok(NULL, " ");
                }
        }
        
        //hm_remove(hm, "rocks","D2");
        printList(hm);
        hm_destroy(hm);
        return 0;
}

int printList(struct hashmap* hm)
{
        //goes through the hashmap and prints the entire list
        int i;
        for(i = 0; i < 15; i++)
        {
                
                struct llnode *ptr = hm->map[i];
                struct docnode *docptr = ptr->doc;
                while(ptr->next != NULL)
                {
                        printf("Test: %d %s \n", i,ptr->word);
                        while(docptr->doc_next != NULL)
                        {
                                printf("        doc: %s\n", docptr->document_id);
                                printf("        num: %d\n", docptr->num_occurrences);
                                //printf("doc->next: %s\n", docptr->doc_next);
                                docptr = docptr->doc_next;
                        }
                        printf("        doc: %s\n", docptr->document_id);
                        printf("        num: %d\n", docptr->num_occurrences);
                        ptr = ptr->next;
                } 
                
                printf("%d %s \n", i,ptr->word);
                while(docptr->doc_next != NULL)
                {
                        printf("        doc: %s\n", docptr->document_id);
                        printf("        num: %d\n", docptr->num_occurrences);
                        //printf("doc->next: %s\n", docptr->doc_next);
                        docptr = docptr->doc_next;
                }
                printf("        doc: %s\n", docptr->document_id);
                printf("        num: %d\n", docptr->num_occurrences);
                printf("\n");  
                
        }
        printf("num elements: %d \n", hm->num_elements);

        return 0;
}
