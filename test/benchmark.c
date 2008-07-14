/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *  memqd -  A distributed memory queue message system.
 *
 *       http://code.google.com/p/memqd/
 *
 *
 *  Authors:
 *      roast <roast@php.net>
 *
 *  $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <libmemcached/memcached.h>

void *bench_new(int queue, int iq, int type);

int main(int argc,char **argv)
{
       int number = 0;
	   int queue = 1;
	   int qnum = 0;
	   int iq = 0;
	   int type = 0;
	   int i = 0;
       pid_t pid;
	   
	   	
       struct timeval tv1,tv2;

       if(argc < 4)
       {
			printf("Usage:./benchmark thread_number queue_number  test_type(1:write,other:read) \n");
            exit(255);
       }

       number = qnum = atoi(argv[1]);
	   iq =  atoi(argv[2]);
	   type = atoi(argv[3]);
	   
       gettimeofday(&tv1,NULL);

       for(i = 0; i < number; i++)
       {
			pid = fork();
            if (pid < 0)
            {
               perror("fork error:");
               continue;
            }

            if (pid == 0)
            {
                usleep(1);
                break;
            }

			queue++;
       }


       if (pid == 0)
       {
			bench_new(queue, iq, type);
			return 0;
       }
       else
       {
            while(1)
            {
            	if (--number == 0)
                   break;
            }

            waitpid(0, NULL, 0);
       }

       gettimeofday(&tv2,NULL);

       printf("time is %f,conn is %f persecond\n",((tv2.tv_sec-tv1.tv_sec)+(tv2.tv_usec-tv1.tv_usec)/1000000.0),qnum/((tv2.tv_sec-tv1.tv_sec)+(tv2.tv_usec-tv1.tv_usec)/1000000.0)*iq);

       return 0;
}


void *bench_new(int queue, int iq, int type)
{						
		memcached_server_st *servers = NULL;
		memcached_st *memc;
		memcached_return rc;
		
		char key[7];
		char value[10];

		int i = 0;
		
		memcached_server_st *memcached_servers_parse (char *server_strings);
		memc= memcached_create(NULL);

		servers= memcached_server_list_append(servers, "127.0.0.1", 11212, &rc);
		rc= memcached_server_push(memc, servers);
		
		if (rc != MEMCACHED_SUCCESS)
			return NULL;

		sprintf(key, "queue%d", queue);

		/* printf("%s\n", key); */
		
		for(i = 0 ; i < iq; i++)
		{
			sprintf(value, "value%d", i);
							
			if (type == 1)	/* Test set */
				memcached_set(memc, key, strlen(key), value, strlen(value), (time_t)0, (uint32_t)0);	
			else			/* Test get */
				memcached_get (memc, key, strlen(key), (size_t *)strlen(value), (uint32_t)0, &rc);
		}			
			

       return NULL;

}
