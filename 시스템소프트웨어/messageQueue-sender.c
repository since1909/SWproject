#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define SIZE 124
/* 메시지 형식을 저장하는 type 멤버를 우선순위를 저장하는 용도로 사용한다. */
struct {
    long type;
    char data[SIZE];
} msg_data;

int main()
{
    int msqid, data_len;
    char buffer[SIZE];
    /* 1) 1234 키의 메시지 큐 생성 */
    if((msqid = msgget((key_t)1234, IPC_CREAT | 0666)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    while(1) {
    /* 2) 표준입력장치로부터 데이터 입력 */
        printf("input data => ");
        scanf("%s" , buffer);
        //fflush(stdin);
    /*3) 입력받은 내용이 ‘quit’이면 while 문 벗어남 */
        if(!strcmp(buffer, "quit"))
            break;
    /*4) 표준입력장치로부터 우선순위 입력 */
        printf("input priority => ");
        scanf("%ld", &msg_data.type);
        strcpy(msg_data.data, buffer); 
       //fflush(stdin);
    /*5) msg_data 메시지를 msqid 메시지 큐에 전송 */
        if((msgsnd(msqid, &msg_data, strlen(msg_data.data), 0)) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }
    exit(0);
}