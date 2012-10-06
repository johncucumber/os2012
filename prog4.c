#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/types.h>
#include <linux/ipc.h>
#include <linux/msg.h>
#include <sys/types.h>
#include <linux/sem.h>

typedef struct mymsgbuf
{
  long mtype;
  int num;
  int sum;
} mess_t;

void saya(int semid, int qid, struct sembuf lock_res, struct sembuf rel_res, mess_t *buf, int length, char type)
{
	while (buf->num<13)
	{
	  sleep(rand()%3);       
	  if (semop(semid, &lock_res, 1) != -1)
	  {
	    if (msgrcv(qid, buf, length, 1, 0)!=-1)
	    {
	      if (type=='s') 
	      	printf("SON %d - NUM %d: SUM %d\n", getpid(),buf->num, buf->sum);
	      else 
		printf("FATHER - NUM %d: SUM %d\n", buf->num, buf->sum); 
	      if (buf->num<13)
	      {
	       	buf->num++;
		buf->sum*=buf->num;
	      }
              msgsnd(qid, buf, length, 0);
    	      semop(semid, &rel_res, 1); 	     		    
	    }
	  } 
	}
}

int main()
{
  int qid;
  key_t msgkey;
  pid_t pid;

  mess_t buf,buf2;

  int length;

  int i;

  int semid;
  union semun arg;
  struct sembuf lock_res = {0, -1, 0};
  struct sembuf rel_res = {0, 1, 0};

  length = sizeof(mess_t) - sizeof(long);

  msgkey = ftok(".",'m');

  semid = semget(msgkey, 1, 0666 | IPC_CREAT);

  arg.val = 1;
  semctl(semid, 0, SETVAL, arg);

  qid = msgget(msgkey, IPC_CREAT | 0660);

  buf.num=1;
  buf.sum=1;        
  buf.mtype=1;
  
  msgsnd(qid, &buf, length, 0);

  for (i = 0; i < 4; i++)
  {
    pid = fork();
    if(!pid)
    {
      saya(semid,qid,lock_res,rel_res,&buf,length,'s');
      exit(0);
    }
  }
   
  saya(semid,qid,lock_res,rel_res,&buf,length,' ');  

  msgctl(qid, IPC_RMID, 0);
  semctl(semid, 0, IPC_RMID);

  return 0;
}
