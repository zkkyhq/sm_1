#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>

#include "shm_com.h"
#include "mem.h"

int main()
{
  int running = 1;
  void *shared_memory = (void *)0;
  //struct shared_use_st *shared_stuff;
  struct PACKET *packet;
  int shmid;

  srand((unsigned int)getpid());
  //shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
  shmid = shmget((key_t)1234, sizeof(struct PACKET), 0666 | IPC_CREAT);
  if (shmid == -1) {
    fprintf(stderr, "shmget failed\n");
    exit(EXIT_FAILURE);
  }
  shared_memory = shmat(shmid, (void *)0, 0);

  if (shared_memory == (void *)-1) {
    fprintf(stderr, "shmat failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Memory attached at %X\n", (int)shared_memory);

  //hared_stuff = (struct shared_use_st *)shared_memory;
  //shared_stuff->written_by_you = 0;
  packet = (struct PACKET *)shared_memory;
  packet->flag = 0;
  FILE *fp = fopen("test.264", "w");
  FILE *fp2 = fopen("test.index", "w");
  struct PACKET_I rev_pac;
  memset(&rev_pac, 0, sizeof(struct PACKET_I)); 
  unsigned short framecnt = 0;
  unsigned int frolen = 0;
  unsigned int curlen = 0;
  while(running) 
  {
    //if (shared_stuff->written_by_you) 
    //{
    //  printf("You wrote: %s", shared_stuff->some_text);

    //  sleep( rand() % 4 ); /* make the other process wait for us ! */
    //  shared_stuff->written_by_you = 0;

    //  if (strncmp(shared_stuff->some_text, "end", 3) == 0) {
    //   running = 0;
     // }
    //}
	if (packet->flag) 
	{
  rev_pac.head.IFrameType = packet->head.IFrameType;
  rev_pac.head.IFrameLen = packet->head.IFrameLen;
  rev_pac.head.ISreamExam = packet->head.ISreamExam;
  rev_pac.head.IExtendLen = packet->head.IExtendLen;
  rev_pac.head.IExtendCount = packet->head.IExtendCount;
  rev_pac.rtc.stuInfoTYpe.LInfoType = packet->rtc.stuInfoTYpe.LInfoType;
  rev_pac.rtc.stuInfoTYpe.LInfoLength = packet->rtc.stuInfoTYpe.LInfoLength ;
  rev_pac.rtc.stuRtcTime.cYear = packet->rtc.stuRtcTime.cYear;
  rev_pac.rtc.stuRtcTime.cMonth = packet->rtc.stuRtcTime.cMonth;
  rev_pac.rtc.stuRtcTime.cDay = packet->rtc.stuRtcTime.cDay;
  rev_pac.rtc.stuRtcTime.cHour = packet->rtc.stuRtcTime.cHour;
  rev_pac.rtc.stuRtcTime.cMinute = packet->rtc.stuRtcTime.cMinute;
  rev_pac.rtc.stuRtcTime.cSecond = packet->rtc.stuRtcTime.cSecond;
  rev_pac.rtc.stuRtcTime.usMilliSecond = packet->rtc.stuRtcTime.usMilliSecond;
  rev_pac.rtc.stuRtcTime.usWeek = packet->rtc.stuRtcTime.usWeek;
  rev_pac.rtc.stuRtcTime.usReserved = packet->rtc.stuRtcTime.usReserved;
  rev_pac.rtc.stuRtcTime.usMilliValidate = packet->rtc.stuRtcTime.usMilliValidate;
  strncpy(rev_pac.frame, packet->frame, rev_pac.head.IFrameLen);
  frolen+=curlen;
  curlen = 12+rev_pac.head.IExtendLen+rev_pac.head.IFrameLen;
		  //fwrite(&rev_pac, sizeof(rev_pac),1,fp);/*存在字节对齐问题*/
fwrite(&rev_pac, curlen ,1,fp);
struct INDEX_INFO index;
index.cHour = packet->rtc.stuRtcTime.cHour;
index.cMinute = packet->rtc.stuRtcTime.cMinute;
index.cSecond = packet->rtc.stuRtcTime.cSecond;
index.offset = frolen;
fwrite(&index, sizeof(index) ,1,fp2);
		packet->flag = 0;
		break;
	}
		
  }
  
  if (shmdt(shared_memory) == -1) 
  {
    fprintf(stderr, "shmdt failed\n");
    exit(EXIT_FAILURE);
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) 
  {
    fprintf(stderr, "shmctl(IPC_RMID) failed\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

