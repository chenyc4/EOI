# ifndef __LINKLIST__H__
# define __LINKLIST__H__  

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<malloc.h>


#define SUCCESS 0
#define FAIL    -1

typedef int T;   

typedef struct Node
{
    T data;
    struct Node *next;
}Node;

int InitLinkList(Node **head);  

Node *GetNode(Node* head, int index);             

int AddLinkList(Node *head, T e);      

int DelLinkList(Node* head, T e);  
     
int LinkListLength(Node* head);     

void DestroyLinkList(Node* head);   

# endif
