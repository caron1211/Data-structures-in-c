#include <assert.h>	/*assert */
#include <unistd.h>	/*pid */

#include "uid.h"


ilrd_uid_t UidCreate(void)
{
	static size_t counter = 0;
	ilrd_uid_t uid = {0};
	time_t seconds; 

    seconds = time(&seconds);
    
	uid.timestamp = seconds;
	uid.counter = counter;
	uid.pid = getpid();
	
	++counter;
	return (uid);
}


ilrd_uid_t GetBadUid(void)
{
	static size_t counter = 0;
	ilrd_uid_t uid = {0};
    
	uid.timestamp = 0;
	uid.counter = counter;
	uid.pid = getpid();
	
	return (uid);
}


int UidIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return (uid1.counter == uid2.counter && uid1.pid == uid2.pid && uid1.timestamp == uid2.timestamp);
	
}
