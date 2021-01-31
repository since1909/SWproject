#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#define MAXLINE 1024
#define LISTENQ 1024
#define THREAD_NUM 2

#define PRE_LOGIN 1
#define SIGN_IN 2
#define SIGN_UP 3
#define AFTER_LOGIN 4
#define WITHDRAWAL 5
#define PLAYERINFO 6
#define GAME 7

#define OK 1            //userDB check
#define ERROR 0
#define WAITING 0       //playerCount

void *thrfunc(void * arg);

int pw_data, win_data, lose_data;

char  id_tmp[100];
char  pw_tmp[100];
char  pw_cmp[100];
char* win_tmp = "7";
char* lose_tmp = "3";

struct player{
    char id[100];
    char pw[100];
    int win;
    int lose;    
};

struct player p1;
struct player p2;

int p1ready = 0;
int p2ready = 0;

int window_buf;
int excess_stat;
int client_wait;
int client_count = 0;

int main(int argc, char ** argv){
    struct sockaddr_in servaddr, clntaddr;
    int listen_sock;
    int acep_sock[THREAD_NUM];
    int status, addrlen;

    pthread_t tid[THREAD_NUM];
    pid_t pid;

    if(argc != 2){
        perror("Usage : <port>");
        exit(0);
    }
    if((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) <0 ){
        perror("socket() error : ");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if(bind(listen_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("bind() error : ");
        exit(0);
    }

    while(1){
        if(client_count < 2){
            listen(listen_sock, LISTENQ);

            acep_sock[client_count] = accept(listen_sock, (struct sockaddr*)&clntaddr, &addrlen);
            if(acep_sock[client_count] < 0){
                perror("accept() error : ");
                exit(0);
            }

            printf("CONNECT :  %d\n", client_count+1);
            client_count++;
            if((status = pthread_create(&tid[client_count-1], NULL, &thrfunc, (void*)&acep_sock[client_count-1]))!=0){
                perror("pthread_create() error : ");
                exit(0);
            }

        }

        else
            continue;
    }
}

void *thrfunc(void* arg){
    int acep_sock = (int)*((int*) arg);
    while(1){
        read(acep_sock, &window_buf, 4);
        if(window_buf == SIGN_UP){
            chdir("ID");
            read(acep_sock, id_tmp, sizeof(id_tmp)- sizeof(char*));
            if(chdir(id_tmp)){
                excess_stat = OK;
                write(acep_sock, &excess_stat, 4);
                read(acep_sock, pw_tmp, sizeof(pw_tmp)- sizeof(char*));

                mkdir(id_tmp, 0755);
                chdir(id_tmp);
                pw_data = open("password", O_WRONLY | O_CREAT | O_TRUNC, 0755);
                win_data = open("win",O_WRONLY | O_CREAT | O_TRUNC, 0755);
                lose_data = open("lose",O_WRONLY | O_CREAT | O_TRUNC, 0755);
   
                write(pw_data, pw_tmp, strlen(pw_tmp));
                write(win_data, win_tmp, strlen(win_tmp));
                write(lose_data, lose_tmp, strlen(lose_tmp));
   
                close(pw_data);
                close(win_data);
                close(lose_data);
                chdir("..");

                printf("SIGN UP : %s\n", id_tmp);

            }
            else{
                excess_stat = ERROR;
                write(acep_sock, &excess_stat, 4);
                chdir("..");
            }

        }
        else if(window_buf == SIGN_IN){
            chdir("ID");
            read(acep_sock, id_tmp, sizeof(id_tmp)- sizeof(char*));
            if(!chdir(id_tmp))
            {
                if(p1ready == 0 && p2ready == 0){
                    write(acep_sock, &p1ready, 4);              //one client
                    excess_stat = OK;
                    write(acep_sock, &excess_stat, 4);              //id ok
                    strcpy(p1.id, id_tmp);
                    read(acep_sock, pw_tmp, sizeof(pw_tmp)- sizeof(char*));
                    pw_data = open("password", O_RDONLY);           
                    read(pw_data, p1.pw, strlen(pw_tmp));
                    if(!strncmp(p1.pw, pw_tmp, strlen(p1.pw)))
                    {
                        excess_stat = OK;                               //login success
                        write(acep_sock, &excess_stat, 4);
                        
                        win_data = open("win", O_RDONLY);
                        read(win_data, win_tmp, strlen(win_tmp));
                        p1.win = atoi(win_tmp);
                        write(acep_sock, &p1.win, 8);
                        
                        lose_data = open("lose", O_RDONLY);
                        read(lose_data, lose_tmp, strlen(lose_tmp));
                        p1.lose = atoi(lose_tmp);
                        write(acep_sock, &p1.lose, 8);
                        
                        chdir("..");
                        p1ready = 1;
                        printf("SING IN : %s\n", p1.id);
                    }
                    else
                    {
                        excess_stat = ERROR;                        //login fail
                        write(acep_sock, &excess_stat, 4);
                        chdir("..");       
                    }        
                }
                else if(p1ready == 1 && p2ready == 0){
                    write(acep_sock, &p1ready, 4);
                        printf("p1.id == %s, id_tmp == %s\n", p1.id, id_tmp);

                    if(!strncmp(p1.id, id_tmp, strlen(id_tmp))){
                        excess_stat = ERROR;                        //id error
                        write(acep_sock, &excess_stat, 4);
                        chdir("..");           
                    }
                    else{
                        excess_stat = OK;                           //id ok
                        write(acep_sock, &excess_stat, 4);
                        strcpy(p2.id, id_tmp);
                        read(acep_sock, pw_tmp, sizeof(pw_tmp)- sizeof(char*));
                        pw_data = open("password", O_RDONLY);
                        read(pw_data, p2.pw, strlen(pw_tmp));
                        if(!strncmp(p2.pw, pw_tmp, strlen(p2.pw)))
                        {
                            excess_stat = OK;                       //login ok
                            write(acep_sock, &excess_stat, 4);
                            strcpy(p2.pw, pw_tmp);
                            win_data = open("win", O_RDONLY);
                            read(win_data, win_tmp, strlen(win_tmp));
                            p2.win = atoi(win_tmp);
                            write(acep_sock, &p2.win, 8);
                        
                            lose_data = open("lose", O_RDONLY);
                            read(lose_data, lose_tmp, strlen(lose_tmp));
                            p2.lose = atoi(lose_tmp);
                            write(acep_sock, &p2.lose, 8);

                            chdir("..");
                            p2ready = 1;
                            write(acep_sock, &p1, sizeof(p1));
                            printf("SING IN : %s\n", p2.id);
                        }
                        else{
                            excess_stat = ERROR;                    //login fail
                            write(acep_sock, &excess_stat, 4);
                            chdir("..");
                        }       
                    }   
                }
            }
            else{
                write(acep_sock, &p1ready, 4);              //one client
                excess_stat = ERROR;                            //id error
                write(acep_sock, &excess_stat, 4);

            }
        }
        else if(window_buf == WITHDRAWAL){ 
            chdir("ID");
            if(chdir(id_tmp))
            {
                excess_stat = ERROR;
                write(acep_sock, &excess_stat, 4);
            }
            else
            {
                excess_stat = OK;
                write(acep_sock, &excess_stat, 4);
                read(acep_sock, pw_tmp, sizeof(pw_tmp)- sizeof(char*));
                pw_data = open("password", O_RDONLY);
                read(pw_data, pw_cmp, strlen(pw_tmp));
                if(!strncmp(pw_cmp, pw_tmp, strlen(pw_cmp)))
                {
                    excess_stat = OK;
                    write(acep_sock, &excess_stat, 4);
                    close(pw_data);
                    remove("password");
                    remove("win");
                    remove("lose");
                    chdir("..");
                    rmdir(id_tmp);
                }
                else
                {
                    excess_stat = ERROR;
                    write(acep_sock, &excess_stat, 4);
                    chdir("..");        
                }
            }
        }
        else if(window_buf == AFTER_LOGIN){
            read(acep_sock, &id_tmp, strlen(id_tmp));
            printf("SIGN OUT : %s\n", id_tmp);
            if(!strncmp(p1.id, id_tmp, strlen(id_tmp))){
                printf("SIGN OUT : %s\n", p1.id);
                p1 = p2;
                p2ready = 0;
            }
            else if(!strncmp(p2.id, id_tmp, strlen(id_tmp))){
                printf("SIGN OUT : %s\n", p2.id);
                p2ready = 0;
            }
        }
        else if(window_buf == PLAYERINFO){
            while(1){
                if(p2ready == 1)
                {
                    write(acep_sock, &p2, sizeof(p2));
                    break;
                }
            }    
        }
    }

}