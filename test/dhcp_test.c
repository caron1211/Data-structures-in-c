#include <stdio.h> /*printf*/
#include <string.h> /*memset*/
#include <math.h> /*memset*/

#include "dhcp.h"
#include "utilities.h"

int CmpInt(const void *new_data, const void *curr_data);
int IntIsMatch(const void *data1, void *data2);

void InsertTest();

void PrintIp(unsigned char * res);
void  FirstTest();
void  SecondTest();


int main ()
{
    InsertTest();
    FirstTest();
     SecondTest();

   return 0;
}

    unsigned char ip [IPV] = {192,168,111,0};
    unsigned char to_alloc1 [IPV] = {192,168,111,0};
    unsigned char res [IPV] = {0};

void InsertTest()
{
    

    dhcp_t * dhcp = DhcpCreate(ip, 30);

    DhcpAllocateIp(dhcp, to_alloc1, res);
    PrintIp(res);

    DhcpAllocateIp(dhcp, to_alloc1, res);
    PrintIp(res);

    DhcpAllocateIp(dhcp, to_alloc1, res);
    PrintIp(res);

    DhcpAllocateIp(dhcp, to_alloc1, res);
    PrintIp(res);

    DhcpFreeIp(dhcp, to_alloc1);

    printf("count : % d\n", DhcpCountFree(dhcp));

    DhcpDestroy(dhcp);
}

void  SecondTest()
{
    int i = 0;
    int status = 0; 

    unsigned char ip[IPV] = {10, 20, 30, 0};
    unsigned char requested_id[IPV] = {10, 20, 30, 1};
    unsigned char allocted[IPV] = {0};
    
    dhcp_t *dhcp = DhcpCreate(ip, 24); 
    
    printf(CYAN"\t dhcp Test 2\n\n"WHITE);
    
    TEST_IS_EQUAL("count when empty Test", DhcpCountFree(dhcp), pow(2, 8)-3); 

    for (i = 23; i < 254; i++)
    {
        requested_id[3] = i;    
        DhcpAllocateIp(dhcp, requested_id, allocted); 
        TEST_IS_EQUAL("DhcpAllocateIp Test", allocted[3], i);  
        printf("inserted = %d \n", i); 
        memset(allocted, 0, 4); 
    }

    TEST_IS_EQUAL("count when full", DhcpCountFree(dhcp), 0); 

    requested_id[3] = 250; 
    status = DhcpAllocateIp(dhcp, requested_id, allocted);

    TEST_IS_EQUAL("insert when full", status, FAIL);  
    TEST_IS_EQUAL("try to remove special number 0", DhcpFreeIp(dhcp, ip), FAIL);

    ip[3] = 250; 
    TEST_IS_EQUAL("try to remove 250", DhcpFreeIp(dhcp, ip), OK);
    TEST_IS_EQUAL("try to remove 250 again", DhcpFreeIp(dhcp, ip), DOUBLE_FREE);

    DhcpDestroy(dhcp); 
}


void FirstTest()
{
    unsigned char ip[IPV] = {1, 1, 1, 0};
    
    unsigned char requested_id[IPV] = {1, 1, 1, 1};
    unsigned char allocted[IPV] = {0};
    
    dhcp_t *dhcp = DhcpCreate(ip, 24); 
    
	printf(CYAN"\t dhcp Test\n\n"WHITE); 
	
	TEST_IS_EQUAL("dhcpCreate Test", (dhcp ? 1:0), 1);  
    TEST_IS_EQUAL("count Test", DhcpCountFree(dhcp), pow(2, 8)-3);  

    DhcpAllocateIp(dhcp, requested_id, allocted); 
    TEST_IS_EQUAL("DhcpAllocateIp Test", allocted[0], 1);  

    memset(allocted, 0, 4); 

    DhcpAllocateIp(dhcp, requested_id, allocted);
    TEST_IS_EQUAL("DhcpAllocateIp Test", allocted[3], 2);  

    memset(allocted, 0, 4); 

    DhcpAllocateIp(dhcp, requested_id, allocted);
    TEST_IS_EQUAL("DhcpAllocateIp Test", allocted[3], 3);  

    memset(allocted, 0, 4); 
    requested_id[3] = 75; 
    DhcpAllocateIp(dhcp, requested_id, allocted);
    TEST_IS_EQUAL("DhcpAllocateIp Test", allocted[3], 75);  

    TEST_IS_EQUAL("count Test before Remove", DhcpCountFree(dhcp), 249);  

    DhcpFreeIp(dhcp, allocted); 

    TEST_IS_EQUAL("count Test After Remove", DhcpCountFree(dhcp), 250); 
    TEST_IS_EQUAL("DOUBLE_FREE Test",  DhcpFreeIp(dhcp, allocted), DOUBLE_FREE); 

    DhcpDestroy(dhcp); 
}


void PrintIp(unsigned char * res)
{
    size_t i = 0;

    for (i = 0; i < IPV; i++)
    {
        printf("%d.", res[i]);
    }
    printf("\n");
}