#include <stdio.h> /* printf */
#include <unistd.h>	/*pid */

#include "uid.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"
#define YELLOW "\033[1;33m"
#define PURPLE "\033[1;35m"

/*****************************************************************************/

#define TEST(name, actual, expected)\
    				printf("%s: %s\n\n", name, actual == expected ?\
     				GREEN"Pass"WHITE : RED"Fail"WHITE)
    
/*****************************************************************************/
						/* Function Declaration */
static void PrintUID(ilrd_uid_t uid);
static void PrintIsSame(int res);
/*****************************************************************************/


int main()
{
    ilrd_uid_t uid1 = {0};
    ilrd_uid_t uid2 = {0};
    ilrd_uid_t uid3 = {0};
    ilrd_uid_t bad = {0};
    
    uid1 = UidCreate();
    PrintUID(uid1);
    
    sleep(1);
    uid2 = UidCreate();
    PrintUID(uid2);
    
    sleep(1);
    uid3 = UidCreate();
    PrintUID(uid3);
    
    bad =  GetBadUid();
    PrintUID(bad);

    PrintIsSame(UidIsSame(uid1, uid3));
    
    PrintIsSame(UidIsSame(uid1, uid1));

    return 0; 
}

static void PrintUID(ilrd_uid_t uid)
{
	printf("timestamp: %ld\n", uid.timestamp);
    printf("pid: %d\n", uid.pid);
    printf("counter: %ld\n", uid.counter);
}

static void PrintIsSame(int res)
{
	if (0 == res)
	{
		printf("UIDs is not the same\n");
	}
	else
	{
		printf("UIDs are identical\n");
	}
}
