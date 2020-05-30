# include "linklist.h"

/* 此处参数为什么要传二级指针，是因为变量作为参数传递时，并不是传的变量本身，而是变量的一份拷贝，即便这个变量本身也是个指针。
   如果传递的是一级指针，在Init函数里申请堆内存空间后，函数结束，指针丢失，不仅外界拿不到这块空间，还将导致内存泄漏。
*/
int InitLinkList(Node **head)
{
    if (*head != NULL)
    {
        DestroyLinkList(*head);
    }

    *head = (Node *)malloc(sizeof(Node));
    if (*head == NULL)
    {
        return FAIL;
    }
    (*head)->next = NULL;
    return SUCCESS;
} 

Node *GetNode(Node* head, int index)
{
    if (head == NULL)
    {
        return NULL;
    }
    Node *node = head;
    int i = 0;
    while(node)
    {
        if (i == index)
        {
            return node;
        }
        i++;
        node = node->next;
    }
    return NULL;
}             

int AddLinkList(Node *head, T e)    //append
{
    Node *node = NULL; 
    InitLinkList(&node);
    node->data = e;

    if (head == NULL)   //first add
    { 
        head = node;
        return SUCCESS;
    }
    while(head->next != NULL)
    {
        head = head->next;
    }
    head->next = node;
    return SUCCESS;
}      

int DelLinkList(Node* head, T e)
{
    if (head == NULL)
    {
        return FAIL;
    }

    Node *current = head;
    Node *prev = head;
    while(current)
    {
        if (current->data == e) //just support basic type, like int, float, double ,char ... 
        {
            if (head == current)    //delete first node
            {
                if (current->next == NULL)  //just 1 node,and this node is head
                {
                    head = NULL;
                }
                else
                {
                    head = head->next;
                }
            }
            else    //normal
            {
                prev->next = current->next;
            } 
            free(current);
            return SUCCESS;
        }
        prev = current;
        current = current->next;
    }
    return FAIL;    //not found
}  
     
int LinkListLength(Node* head)  //contains head node, so: real length = length - 1
{
    if (head == NULL)   return 0;
    int size = 0;
    while(head)
    {
        size++;
        head = head->next;
    }
    return size;
}     

void DestroyLinkList(Node* head)
{
    if (head == NULL)
    {
        return;
    }
    Node *prev = head;
    Node *current = head;
    while(prev)
    {
        current = current->next;
        free(prev);
        prev = current;
    }
    head = NULL;
    return;
}