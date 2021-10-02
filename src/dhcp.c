#include <stdlib.h> /*malloc*/
#include <assert.h> /* assert */
#include <string.h> /*memcpy */
#include <math.h> /*pow */

#include "dhcp.h"
#include "trie.h"

#define BITS_IN_BYTE 8
#define BITS 32


static size_t GetIp(unsigned char * req);
static void BuildIp(dhcp_t *dhcp, size_t data, unsigned char allocted[IPV]);
static void AllocSpecificIP (dhcp_t *dhcp);
static int IsSpecialIp(dhcp_t *dhcp, size_t ip);

struct dhcp 
{
	unsigned char subnet_ip[IPV]; 
    size_t num_of_subnet_bits;
    trie_t *trie;
};

dhcp_t *DhcpCreate(const unsigned char subnet_ip[IPV], size_t num_of_subnet_bits)
{
    dhcp_t *dhcp = NULL;
    trie_t *trie = NULL;
   
    dhcp = malloc(sizeof(dhcp_t));
    if (dhcp == NULL)
    {
        return NULL;
    }

    trie = TrieCreate(BITS - num_of_subnet_bits);
    if (trie == NULL)
    {
        free(dhcp);
        return NULL;
    }

    dhcp->trie = trie;
    dhcp->num_of_subnet_bits = num_of_subnet_bits;
    memcpy(dhcp->subnet_ip, subnet_ip, IPV);

    AllocSpecificIP(dhcp);

    return dhcp;
}

void DhcpDestroy(dhcp_t *dhcp)
{
    TrieDestroy(dhcp->trie);
    dhcp->trie = NULL;
    free(dhcp);
}

dhcp_status_t DhcpAllocateIp(dhcp_t *dhcp, const unsigned char requested_id[IPV], unsigned char allocted[IPV])
{
    size_t to_alloc = 0;

    trie_status_t status = SUCCESS; 

    to_alloc = GetIp((unsigned char *)requested_id);


    status = TrieNextAvailable(dhcp->trie, &to_alloc);

    if((status != SUCCESS))
    {
        return status;
    }

    BuildIp(dhcp, to_alloc, allocted);

    status = TrieInsert(dhcp->trie, to_alloc);

    return status;
}

size_t DhcpCountFree(dhcp_t *dhcp)
{
    size_t all_options = 0;
    size_t num_of_host_bits = 0;

    num_of_host_bits = BITS - dhcp->num_of_subnet_bits;

    all_options = pow(2, num_of_host_bits);

    return (all_options - TrieCount(dhcp->trie));
}

dhcp_status_t DhcpFreeIp(dhcp_t *dhcp, unsigned char ip[IPV])
{
    size_t to_remove = 0;
    trie_status_t status = SUCCESS;

    to_remove = GetIp((unsigned char *)ip);

    if (IsSpecialIp(dhcp, to_remove))
    {
        return (FAIL);
    }
    status = TrieRemove(dhcp->trie, to_remove);

    if (status == PATH_INVALID)
    {
        return (DOUBLE_FREE);
    }

    return (OK);
}

static size_t GetIp(unsigned char * req)
{
    size_t data = 0;
    size_t i = 0;

    for (i = 0; i < IPV; i++)
    {
        data <<= BITS_IN_BYTE;
        data |= req[i];
    }
    
    return data;
}

static void BuildIp(dhcp_t *dhcp, size_t data, unsigned char allocted[IPV])
{
    size_t final_ip = 0;
    size_t i = 0;
    unsigned char *subnet = dhcp->subnet_ip;

    for (i = 0; i < IPV; ++i)
    {
        final_ip <<= BITS_IN_BYTE;

        final_ip |= subnet[i];

    }

    final_ip |= data;

     for (i = IPV; i > 0; i--)
    {
        allocted[i - 1] = *(char *)&final_ip;

        final_ip >>= BITS_IN_BYTE;
    }
}

static void AllocSpecificIP (dhcp_t *dhcp)
{
    size_t first = 0;
    size_t second = pow(2, dhcp->num_of_subnet_bits) -1;
    size_t third =  pow(2, dhcp->num_of_subnet_bits) -2;

    TrieInsert(dhcp->trie, first);
    TrieInsert(dhcp->trie, second);
    TrieInsert(dhcp->trie, third);
}

static int IsSpecialIp(dhcp_t *dhcp, size_t ip)
{
    return (((ip == pow(2, dhcp->num_of_subnet_bits) - 1) ||
            (ip == pow(2, dhcp->num_of_subnet_bits) - 2) ||
            (ip == 0)) ? 1 : 0)  ; 
}   