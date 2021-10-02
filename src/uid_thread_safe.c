#include <assert.h>	/*assert */
#include <unistd.h>	/*pid */
#include <pthread.h> /*mutex */

#include "uid.h"

pthread_mutex_t g_lock;

ilrd_uid_t UidCreate(void)
{
	static size_t counter = 0;
	ilrd_uid_t uid = {0};
	time_t seconds;

	if (pthread_mutex_init(&g_lock, NULL) != 0) 
	{
        printf("\n mutex init has failed\n");
		return GetBadUid();
    }


    seconds = time(&seconds);
    
	uid.timestamp = seconds;
	uid.counter = counter;
	uid.pid = getpid();

	pthread_mutex_lock(&g_lock); 
	++counter;
	pthread_mutex_unlock(&g_lock);

	pthread_mutex_destroy(&g_lock);

	return (uid);
}


ilrd_uid_t GetBadUid(void)
{
	size_t counter = 0;
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
