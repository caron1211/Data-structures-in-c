#include <stdlib.h> /* malloc, free */
#include <stdio.h> /*printf*/
#include <assert.h> /*assert*/

typedef struct node node_t;

#define RED "\33[1;31m"
#define GREEN "\33[1;32m"
#define WHIT "\33[0m"
#define MAG "\33[1;96m"

#define TEST_IS_TRUE(test_name, result)\
			(result ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))
			
#define TEST_IS_FALSE(test_name, result)\
			(!result ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))
			
#define TEST_IS_EQUAL(test_name, expected, real)\
			(real == expected ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))
			
struct node 
{
	int data;
	node_t *next;
};

node_t *Flip(node_t *head);
int HasLoop(const node_t *head);
node_t *FindIntersection(node_t *head1, node_t *head2);

static void prinNode(node_t *head);
static size_t CountElements(node_t *head);
static node_t *Move(node_t *start, size_t step);

static void TestFlip();
static void TestHasLoop();
static void TestFindIntersection();


int main ()
{
	
    TestFlip();
	TestHasLoop();
	TestFindIntersection();
    
   
	return 0;
}

static void TestFlip()
{
	node_t* head = NULL; 
	node_t* second = NULL; 
	node_t* third = NULL; 
  
    head = (node_t*)malloc(sizeof(node_t)); 
    second = (node_t*)malloc(sizeof(node_t)); 
    third = (node_t*)malloc(sizeof(node_t)); 
  
    head->data = 1; 
    head->next = second;
    
    second->data = 2;
    second->next = third;
    
    third->data = 3;
    third->next = NULL;
    
    printf(MAG"Print before flip:\n"WHIT);
    prinNode(head);
    head = Flip(head);
    
    printf(MAG"Print after flip:\n"WHIT);
    prinNode(head);
    
    head = Flip(head);
    
    free(head);
    free(second);
    free(third);
}

static void TestHasLoop()
{
	node_t* head = NULL; 
	node_t* second = NULL; 
	node_t* third = NULL; 
 
    head = (node_t*)malloc(sizeof(node_t)); 
    second = (node_t*)malloc(sizeof(node_t)); 
    third = (node_t*)malloc(sizeof(node_t)); 
  
  
    head->data = 1; 
    head->next = second;
    
    second->data = 2;
    second->next = third;
    
    third->data = 3;
    third->next = NULL;
    
    TEST_IS_FALSE("check no loop ", HasLoop(head));
    
	third->next = head;
	
	TEST_IS_TRUE("check has loop ", HasLoop(head));
	
	free(head);
    free(second);
    free(third);
}

static void TestFindIntersection()
{
	node_t* head1 = NULL; 
	node_t* second = NULL; 
	node_t* third = NULL; 
 
 	node_t* head2 = NULL; 
 	node_t* res = NULL; 
 	
    head1 = (node_t*)malloc(sizeof(node_t)); 
    second = (node_t*)malloc(sizeof(node_t)); 
    third = (node_t*)malloc(sizeof(node_t));
    
    head2 = (node_t*)malloc(sizeof(node_t)); 
    
    head1->data = 1; 
    head1->next = second;
    
    second->data = 2;
    second->next = third;
    
    third->data = 3;
    third->next = NULL;
    
    head2->data = 1; 
    head2->next = second;
    
    res = FindIntersection(head2, head1);
    TEST_IS_EQUAL("Check Intersection", res->data, 2);
    
    head2->next = NULL;
    res = FindIntersection(head2, head1);
    TEST_IS_EQUAL("Check no Intersection", res, NULL);
    
    free(head1);
    free(second);
    free(third);
    free(head2);
}

static void prinNode(node_t *head) 
{
	node_t* curr = NULL;
	
	assert(NULL != head);
	
	curr = head;
	while (curr != NULL) 
	{
       printf(" %d",curr->data);
       curr = curr->next;
   	}
	
	printf("\n");
}

static size_t CountElements(node_t *head)
{
	size_t count = 0;
	
	assert(NULL != head);
	
	while(head != NULL)
	{
		count++;
		head = head->next;
		
	}
	
	return count;
}

static node_t *Move(node_t *start, size_t step)
{
	size_t i = 0;
	
	assert(NULL != start);
	
	for (i = 0; i < step; i++) { 
        if (start == NULL) 
        { 
            return NULL; 
        } 
        start = start->next; 
    } 
	return start;
}

node_t *Flip(node_t *head)
{
	
	node_t* prev = NULL;
	node_t* curr = NULL;
	node_t* next = NULL;
	
	assert(NULL != head);
	
	curr = head;
	
    while (curr != NULL) 
    {
        next = curr->next;
        curr->next = prev;
 
        prev = curr;
        curr = next;
    }
	
	head = prev;
	
	return head;
}

int HasLoop(const node_t *head)
{
	node_t *slow_p = NULL;
	node_t *fast_p = NULL;
	
	assert(NULL != head);
	
	slow_p = (node_t *)head;
	fast_p = (node_t *)head;
	
 	while ((NULL != slow_p) && (NULL != fast_p) && (NULL != fast_p->next))
 	{
		slow_p = slow_p->next;
		fast_p = fast_p->next->next;
		if (slow_p == fast_p) 
		{
			return 1;
		}
	}
    return 0;
}

node_t *FindIntersection(node_t *head1, node_t *head2)
{
	size_t count1 = 0;
	size_t count2 = 0;
	size_t delta = 0;
	
	assert(NULL != head1);
	assert(NULL != head2);
	
	count1 = CountElements(head1);
	count2 = CountElements(head2);
	
	if(count1 > count2)
	{
		delta = count1 -count2;
		head1 = Move(head1, delta);
	}
	else
	{
		delta = count2 -count1;
		head2 =Move(head2, delta);
	}
	
	while (NULL != head1 && NULL != head2) 
	{  
        if (head1 == head2)
        { 
            return head1;
        }
        head1 = head1->next; 
        head2 = head2->next; 
    } 
  
    return NULL; 
}
