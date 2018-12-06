#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "hashmap.h"

char str[1000];
char arr[1000];
char arr2[1000];
char str2[200];
char arr3[2];
char num[5];
int bucket;
char* d1;
char* d2;
char* d3;
struct hashmap *hm;
int printList(struct hashmap* hm);
void read_query(void);
void prompt(void);

int main(void)
{
        setbuf(stdout, NULL);
        //gets user input
        printf("How many buckets do you want?\n");
        fgets(num, 5, stdin);
        bucket = atoi(num);
        //creates the hashmap with buckets from user
        hm = hm_create(15);

        FILE *file;
        FILE *file1;
        FILE *file2;

        d1 = (char*)malloc(sizeof(char) *3);
        d2 = (char*)malloc(sizeof(char) *3);
        d3 = (char*)malloc(sizeof(char) *3);

        /*
         * ASSUMES that there are three documents 
         */
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
        //file 1 added to the hashmap
        while(fgets(str, 1000, file) != NULL)
        {
                char *token = strtok(str, " \t\n");
                
                
                while(token != NULL)
                {
                        char *temp = (char*)malloc(sizeof(char)*strlen(token) + 1);
                        strcpy(temp, token);
                        num = hm_get(hm, temp, d1);
                        hm_put(hm, temp, d1, num);
                        token = strtok(NULL, " \t\n");
                }
        }
        //file 2 added to the hashmap
        while(fgets(arr, 1000, file1) != NULL)
        {
                char *token = strtok(arr, " ");
                while(token != NULL)
                {
                        char *temp = (char*)malloc(sizeof(char)*strlen(token) + 1);
                        strcpy(temp, token);
                        num = hm_get(hm, temp, d2);
                        hm_put(hm, temp, d2, num);
                        token = strtok(NULL, " ");
                }
        }
        //file 3 added to the hashmap
        while(fgets(arr2, 1000, file2) != NULL)
        {
                char *token = strtok(arr2, " ");
                

                while(token != NULL)
                {
                        char *temp = (char*)malloc(sizeof(char)*strlen(token) + 1);
                        strcpy(temp, token);
                        num = hm_get(hm, temp, d3);
                        hm_put(hm, temp, d3, num);
                        token = strtok(NULL, " ");
                }
        }
        
        
        rmStopWord(hm);
        fclose(file);
        fclose(file1);
        fclose(file2);
        prompt();
        //printList(hm);
        hm_destroy(hm);
        free(d1);
        free(d2);
        free(d3);
        return 0;
}

void prompt(void)
{
        //setbuf(stdout, NULL);
        printf("enter S to search or X to exit\n");
        char c;
        c = getchar();
        if(c == 83 || c == 115)
        {
                read_query();
                prompt();
        } 
        else if(c == 120 || c == 88)
        {
                exit(0);
        }
        while(c != 88 || c != 120 || c != 83 || c != 115)
        {
                c = fgetc(stdin);
                if(c == 83 || c == 115)
                {
                        read_query();
                        prompt();
                }
                else if(c == 88 || c == 120)
                {
                        return;
                }
        }
        
}


void read_query(void)
{
        getchar();
        printf("words to search for\n");
        fgets(str2, 200, stdin);
        int i = strlen(str2)-1;
        if(str2[i] == '\n') 
        {
                str2[i] = '\0';
        }

        char *token;
        double rank1 = 0;
        double rank2 = 0;
        double rank3 = 0;
        token = strtok(str2, " ");
        //goes through the query and passes them
        //through the rank method and gets there score
        while(token != NULL)
        {
                rank1 += rank(hm, token, d1);
                rank2 += rank(hm, token, d2);
                rank3 += rank(hm, token, d3);
                token = strtok(NULL, " ");
        }
        /*
        *  ASSUMES three files
        */
        //if rank1 is highest
        if(rank1 >= rank2 && rank1 >= rank3)
        {
                if(rank2 >= rank3)
                {
                        printf("%s: %f\n", d1, rank1);
                        printf("%s: %f\n", d2, rank2);
                        printf("%s: %f\n", d3, rank3);
                }
                else
                {
                        printf("%s: %f\n", d1, rank1);
                        printf("%s: %f\n", d3, rank3);
                        printf("%s: %f\n", d2, rank2);
                }
        }
        //if rank2 is highest
        else if(rank2 >= rank3)
        {
                if(rank3 >= rank1)
                {
                        printf("%s: %f\n", d2, rank2);
                        printf("%s: %f\n", d3, rank3);
                        printf("%s: %f\n", d1, rank1);
                }
                else if(rank1 >= rank3)
                {
                        printf("%s: %f\n", d2, rank2);
                        printf("%s: %f\n", d1, rank1);
                        printf("%s: %f\n", d3, rank3);
                }
        }
        //if rank3 is highest
        else if(rank3 >= rank1 && rank3 >= rank2)
        {
                if(rank1 >= rank2)
                {
                        printf("%s: %f\n", d3, rank3);
                        printf("%s: %f\n", d1, rank1);
                        printf("%s: %f\n", d2, rank2);
                }
                else
                {
                        printf("%s: %f\n", d3, rank3);
                        printf("%s: %f\n", d2, rank2);
                        printf("%s: %f\n", d1, rank1);
                }
        }

}

int printList(struct hashmap* hm)
{
        //goes through the hashmap and prints the entire list
        int i;
        for(i = 0; i < 15; i++)
        {
                
                struct llnode *ptr = hm->map[i];
                struct docnode *docptr = ptr->doc;
                if(ptr->word != NULL)
                {
                        while(ptr != NULL)
                        {
                                printf("Test: %d %s \n", i,ptr->word);
                                docptr = ptr->doc;
                                while(docptr != NULL)
                                {
                                        printf("        doc: %s\n", docptr->document_id);
                                        printf("        num: %d\n", docptr->num_occurrences);
                                        docptr = docptr->doc_next;
                                }
                                printf("        df: %d\n", ptr->document_frequency);
                                ptr = ptr->next;
                        } 
                }
                 
                
        }
        printf("num elements: %d \n", hm->num_elements);

        return 0;
}
