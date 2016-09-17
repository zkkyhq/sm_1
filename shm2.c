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
  char buffer[BUFSIZ];
  int shmid;

  //shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
  shmid = shmget((key_t)1234, sizeof(struct PACKET), 0666 | IPC_CREAT);
  if (shmid == -1) 
  {
    fprintf(stderr, "shmget failed\n");
    exit(EXIT_FAILURE);
  }

  shared_memory = shmat(shmid, (void *)0, 0);
  if (shared_memory == (void *)-1) 
  {
    fprintf(stderr, "shmat failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Memory attached at %X\n", (int)shared_memory);

  //shared_stuff = (struct shared_use_st *)shared_memory;
	packet = (struct PACKET *)shared_memory;
  //shared_stuff->written_by_you = 0;
	packet->flag = 0;
  while(running) 
  {
    //while(shared_stuff->written_by_you == 1) 
    //{
    //  sleep(1);
    //  printf("waiting for client...\n");
    //}

	//if (!shared_stuff->written_by_you) 
	if (!packet->flag) 
    {
    	//printf("Enter some text: ");
    	//fgets(buffer, BUFSIZ, stdin);
    	//strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
    	//shared_stuff->written_by_you = 1;

  packet->head.IFrameType = 0x63643200;
  packet->head.IFrameLen = 0x00000A;
  packet->head.ISreamExam = 0xD6;
  packet->head.IExtendLen = 0x00000C;
  packet->head.IExtendCount = 0x01;
  packet->rtc.stuInfoTYpe.LInfoType = 0x02;
  packet->rtc.stuInfoTYpe.LInfoLength = 0x00000C;
  //packet->rtc.stuRtcTime = 0x0800072808100810;
  packet->rtc.stuRtcTime.cYear = 0x10;
  packet->rtc.stuRtcTime.cMonth = 0x08;
  packet->rtc.stuRtcTime.cDay = 0x10;
  packet->rtc.stuRtcTime.cHour = 0x08;
  packet->rtc.stuRtcTime.cMinute = 0x28;
  packet->rtc.stuRtcTime.cSecond = 0x07;
  packet->rtc.stuRtcTime.usMilliSecond = 0;
  packet->rtc.stuRtcTime.usWeek = 2;
  packet->rtc.stuRtcTime.usReserved = 0;
  packet->rtc.stuRtcTime.usMilliValidate = 0;
  strncpy(packet->frame, "abcdefghij", 10);
  packet->flag = 1;		

    	/*if (strncmp(buffer, "end", 3) == 0) {
      	running = 0;
        }*/
	}
    /*printf("Enter some text: ");
    fgets(buffer, BUFSIZ, stdin);

    strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
    shared_stuff->written_by_you = 1;

    if (strncmp(buffer, "end", 3) == 0) {
      running = 0;
    }*/
  }

  if (shmdt(shared_memory) == -1) {
    fprintf(stderr, "shmdt failed\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
