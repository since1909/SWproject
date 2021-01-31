#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct sockaddr_in servaddr;
int sockfd;

int HEIGHT = 24;
int WEIDTH = 80;
int xPos;
int yPos;

WINDOW *window1;
WINDOW *window2;

char *prelogin_menu[] = { "SIGN IN" , "SIGN UP", "EXIT"};
char *signIN_menu[] = { "SIGN IN" , "BACK"};
char *signUP_menu[] = { "SIGN UP", "BACK"};
char *afterlogin_menu[] = {"PLAY", "SIGN OUT", "WITHDRAWAL"};
char *withdrawal_menu[] = {"WITHDRAWAL", "BACK"};
char *playerINFO_menu1 = "OK";
char *playerINFO_menu2 = "WAITING";
char *game_menu = "EXIT";


#define PRE_LOGIN 1
#define SIGN_IN 2
#define SIGN_UP 3
#define AFTER_LOGIN 4
#define WITHDRAWAL 5
#define PLAYERINFO 6
#define GAME 7

int cursor;
int  game_UPDOWN;
int  game_LeftRight;

void cursorf4(int index);
void cursorf3(int index);
void cursorf2(int index);
void cursorf1(int index);
void print_window1(int index);
void print_window2(int index);
void print_just_menu(int index);
void input_ID_PW(int index);
void user_DB(int index);

char  id_tmp[100];
char  pw_tmp[100];
char* win_tmp = "7";
char* lose_tmp = "3";

int window_buf;
int excess_stat;
int client_wait;
int client_count;

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

void user_DB(int index)
{
    if(index == SIGN_UP)
    {
        window_buf = SIGN_UP;
        write(sockfd, &window_buf, 4);
        write(sockfd, &id_tmp, strlen(id_tmp));
        read(sockfd, &excess_stat, 4);
        write(sockfd, &pw_tmp, strlen(pw_tmp));
        refresh();
        if(excess_stat)
        {
            wrefresh(window2);
            mvwprintw(window2, 7, 0, ">>>Welcome to OTHELLO World! (Press any key)");
            wrefresh(window2);

            wgetch(window2);
            print_window1(PRE_LOGIN);
            print_window2(PRE_LOGIN);

        }
        else
        {
            mvwprintw(window2, 7, 0, ">>> %s has already exist in DB! (Press any key)", id_tmp );
            wrefresh(window2);
            
            wgetch(window2);            
            print_window1(SIGN_UP);
            print_just_menu(SIGN_UP);
            input_ID_PW(SIGN_UP);
        }
    }
    else if(index == SIGN_IN)
    {
        window_buf = SIGN_IN;
        write(sockfd, &window_buf, 4);
        write(sockfd, &id_tmp, strlen(id_tmp));
        read(sockfd, &p1ready, 4);              //client count
        if(p1ready == 0 && p2ready == 0){                           // client waiting
            read(sockfd, &excess_stat, 4);          
            if(excess_stat)                         //id correct
            {
                strcpy(p1.id, id_tmp);
                write(sockfd, &pw_tmp, strlen(pw_tmp));
                read(sockfd, &excess_stat, 4);     
                if(excess_stat){                       //pw correct
                    strcpy(p1.pw, pw_tmp);
                    read(sockfd, &p1.win, 8);
                    read(sockfd, &p1.lose, 8);
                    p1ready = 1;
                    print_window1(AFTER_LOGIN);
                    print_window2(AFTER_LOGIN);

                }
                else                                   //pw error
                {
                    mvwprintw(window2, 7, 0, ">>>Login error (Press any key)");
                    wrefresh(window2);
                    wgetch(window2);                   
                    print_window1(SIGN_IN);
                    print_just_menu(SIGN_IN);
                    input_ID_PW(SIGN_IN);
                }        
            }
            else                                           //id error
            {
                mvwprintw(window2, 7, 0, ">>>Login error (Press any key)");
                wrefresh(window2);
                wgetch(window2);            
                print_window1(SIGN_IN);
                print_just_menu(SIGN_IN);
                input_ID_PW(SIGN_IN);
            }
        }
        else if(p1ready == 1 && p2ready == 0){
            read(sockfd, &excess_stat, 4);          
            refresh;
            if(excess_stat)                         //id correct
            {
                strcpy(p2.id, id_tmp);
                write(sockfd, &pw_tmp, strlen(pw_tmp));
                read(sockfd, &excess_stat, 4);     
                if(excess_stat){
                    strcpy(p2.pw, pw_tmp);           //pw correct
                    read(sockfd, &p2.win, 8);
                    read(sockfd, &p2.lose, 8);
                    p2ready = 1;            
                    read(sockfd, &p1, sizeof(p1));
                    print_window1(AFTER_LOGIN);
                    print_window2(AFTER_LOGIN);
                }
                else                                   //pw error
                {
                    mvwprintw(window2, 7, 0, ">>>>>>Login error (Press any key)");
                    wrefresh(window2);
                    wgetch(window2);                   
                    print_window1(SIGN_IN);
                    print_just_menu(SIGN_IN);
                    input_ID_PW(SIGN_IN);
                }        
            }
            else                                           //id error
            {
                mvwprintw(window2, 7, 0, ">>>>Login error (Press any key)");
                wrefresh(window2);
                wgetch(window2);            
                print_window1(SIGN_IN);
                print_just_menu(SIGN_IN);
                input_ID_PW(SIGN_IN);
            }
        }
        else{
            read(sockfd, &excess_stat, 4);
        }
    }
    else if(index == WITHDRAWAL)
    {
        window_buf = WITHDRAWAL;
        write(sockfd, &window_buf, 4);
        read(sockfd, &excess_stat, 4);
        if(!excess_stat)
        {
            mvwprintw(window2, 7, 0, ">>>Sign out error (Press any key)");            
            wrefresh(window2);
            wgetch(window2);
            print_window1(WITHDRAWAL);
            print_just_menu(WITHDRAWAL);
            input_ID_PW(WITHDRAWAL);
        }
        else
        {
            write(sockfd, &pw_tmp, strlen(pw_tmp));
            read(sockfd, &excess_stat, 4);
            if(excess_stat)
            {
                wrefresh(window1);
                wrefresh(window2);
                mvwprintw(window2, 7, 0, ">>>Bye Bye~ (Press any key)");
                wrefresh(window2);
                wgetch(window2);
                print_window1(PRE_LOGIN);            
                print_window2(PRE_LOGIN);                           
            }
            else
            {
                mvwprintw(window2, 7, 0, ">>>Sign out error (Press any key)");            
                wrefresh(window2);
                wgetch(window2);            
                print_window1(WITHDRAWAL);
                print_just_menu(WITHDRAWAL);
                input_ID_PW(WITHDRAWAL);
            }
        }
    }
}


void print_window1(int index){
    window1 = newwin(16, 80, 0, 0);                    //newwin(height, width, xPos, yPos)

    wbkgd(window1, COLOR_PAIR(1));
    
    curs_set(0);
    if(index == PRE_LOGIN)
    {
        mvwprintw(window1, 6, 28, "System Software Practice");
        mvwprintw(window1, 8, 36, "OTHELLO");
        mvwprintw(window1, 13, 69, "2017000000");
        mvwprintw(window1, 14, 69, "Gildong Hong");
    }
    else if(index == SIGN_IN)
    {
        mvwprintw(window1, 4, 37, "SIGN IN");
        mvwprintw(window1, 7, 27, "ID : ");
        mvwprintw(window1, 9, 21, "PASSWARD : ");
    }
    else if(index == SIGN_UP)
    {
        mvwprintw(window1, 4, 37, "SIGN UP");
        mvwprintw(window1, 7, 27, "ID : ");
        mvwprintw(window1, 9, 21, "PASSWARD : ");
    }
    else if(index == AFTER_LOGIN)
    {
        mvwprintw(window1, 6, 20, "PLAYER ID : ");
        mvwprintw(window1, 6, 32, "%s", id_tmp);
    }
    else if(index == WITHDRAWAL)
    {
        mvwprintw(window1, 4, 35, "WITHDRAWAL");
        mvwprintw(window1, 7, 20, "PLAYER ID : ");
        mvwprintw(window1, 7, 32, "%s", id_tmp);
        mvwprintw(window1, 9, 21, "PASSWARD : ");
    }
    else if(index == PLAYERINFO)
    {
        if(p2ready == 0){
            mvwprintw(window1, 4, 10, "PLAYER1 ID : ");
            mvwprintw(window1, 4, 23, "%s", p1.id);
            mvwprintw(window1, 7, 13, "STATISTICS");
            mvwprintw(window1, 9, 6, "WIN : %d / LOSE : %d (%lf%) ", p1.win, p1.lose, (double)((double)p1.win/(p1.win+p1.lose))*100);
            
            mvwprintw(window1, 4, 48, "PLAYER2 ID : ");
            mvwprintw(window1, 7, 53, "STATISTICS");
            mvwprintw(window1, 9, 46, "WIN :  / LOSE :   (  %) ");
            wrefresh(window1);
        }
        else if(p2ready == 1){
            mvwprintw(window1, 4, 10, "PLAYER1 ID : ");
            mvwprintw(window1, 4, 23, "%s", p1.id);
            mvwprintw(window1, 7, 13, "STATISTICS");
            mvwprintw(window1, 9, 6, "WIN : %d / LOSE : %d (%lf%) ", p1.win, p1.lose, (double)((double)p1.win/(p1.win+p1.lose))*100);
            
            mvwprintw(window1, 4, 48, "PLAYER2 ID : ");
            mvwprintw(window1, 4, 61, "%s", p2.id);
            mvwprintw(window1, 7, 53, "STATISTICS"); 
            mvwprintw(window1, 9, 46, "WIN : %d / LOSE : %d (%lf%) ", p2.win, p2.lose, (double)((double)p2.win/(p2.win+p2.lose))*100);            
            wrefresh(window1);
        }
    }
    else if(index == GAME)
    {
        window1 = newwin(24, 60, 0, 0);
        wbkgd(window1, COLOR_PAIR(1));
        mvwprintw(window1, 5, 17, "+---+---+---+---+---+---+");
        mvwprintw(window1, 6, 17, "|   |   |   |   |   |   |");
        mvwprintw(window1, 7, 17, "+---+---+---+---+---+---+");
        mvwprintw(window1, 8, 17, "|   |   |   |   |   |   |");
        mvwprintw(window1, 9, 17, "+---+---+---+---+---+---+");
        mvwprintw(window1, 10, 17, "|   |   | O | X |   |   |");
        mvwprintw(window1, 11, 17, "+---+---+---+---+---+---+");
        mvwprintw(window1, 12, 17, "|   |   | X | O |   |   |");
        mvwprintw(window1, 13, 17, "+---+---+---+---+---+---+");
        mvwprintw(window1, 14, 17, "|   |   |   |   |   |   |");
        mvwprintw(window1, 15, 17, "+---+---+---+---+---+---+");
        mvwprintw(window1, 16, 17, "|   |   |   |   |   |   |");
        mvwprintw(window1, 17, 17, "+---+---+---+---+---+---+");
        wmove(window1, 10, 27);
        curs_set(0);

        wrefresh(window1);
        cursorf4(GAME);
    }
    wrefresh(window1);                                  
}

void print_window2(int index){
    window2 = newwin(8, 80, 16, 0);
    keypad(window2, TRUE);

    wbkgd(window2, COLOR_PAIR(2));
    
    xPos = 18;

    if(index == PRE_LOGIN)
    {
        for(int i = 0; i<3; i++)
        {
            if(i == cursor)
            {
                wattron(window2, A_REVERSE);
                mvwprintw(window2, 4, xPos, "%s", prelogin_menu[i]);
                wattroff(window2, A_REVERSE);
            }
            else
                mvwprintw(window2, 4, xPos, "%s", prelogin_menu[i]);
            xPos += 20; 
        }
        cursorf3(index);

    }
    else if(index == SIGN_IN)
    {
        for(int i = 0; i<2; i++)
        {
            if(i == cursor)
            {
                wattron(window2, A_REVERSE);
                mvwprintw(window2, 4, xPos, "%s", signIN_menu[i]);
                wattroff(window2, A_REVERSE);
            }
            else
                mvwprintw(window2, 4, xPos, "%s", signIN_menu[i]);
            xPos += 40; 
        }
        cursorf2(index);
    }
    else if(index == SIGN_UP)
    {
        for(int i = 0; i<2; i++)
        {
            if(i == cursor)
            {
                wattron(window2, A_REVERSE);
                mvwprintw(window2, 4, xPos, "%s", signUP_menu[i]);
                wattroff(window2, A_REVERSE);
            }
            else
                mvwprintw(window2, 4, xPos, "%s", signUP_menu[i]);
            xPos += 40; 
        }
        cursorf2(index);
    }
    else if(index == AFTER_LOGIN)
    {
        for(int i = 0; i<3; i++)
        {
            if(i == cursor)
            {
                wattron(window2, A_REVERSE);
                mvwprintw(window2, 4, xPos, "%s", afterlogin_menu[i]);
                wattroff(window2, A_REVERSE);
            }
            else
                mvwprintw(window2, 4, xPos, "%s", afterlogin_menu[i]);
            xPos += 20; 
        }
        cursorf3(index);

    }
    else if(index == WITHDRAWAL)
    {
        for(int i = 0; i<2; i++)
        {
            if(i == cursor)
            {
                wattron(window2, A_REVERSE);
                mvwprintw(window2, 4, xPos, "%s", withdrawal_menu[i]);
                wattroff(window2, A_REVERSE);
            }
            else
                mvwprintw(window2, 4, xPos, "%s", withdrawal_menu[i]);
            xPos += 40; 
        }
        cursorf2(index);
    }
    else if(index == PLAYERINFO)
    {
        wattron(window2, A_REVERSE);
        mvwprintw(window2, 4, 39, "%s", "OK");
        wattroff(window2, A_REVERSE);

        cursorf1(index);
    }
    else if(index == GAME)
    {
        window2 = newwin(24, 20, 0, 60);
        keypad(window2, TRUE);

        wbkgd(window2, COLOR_PAIR(2));
        curs_set(0);
        wattron(window2, A_REVERSE);        
        mvwprintw(window2, 18, 8, "%s", game_menu);
        wattroff(window2, A_REVERSE);

        cursorf1(GAME);
    }
    wrefresh(window2);
}

void print_just_menu(int index){
    window2 = newwin(8, 80, 16, 0);
    keypad(window2, TRUE);
    wbkgd(window2, COLOR_PAIR(2));
    if(index == SIGN_IN)
    {
        xPos = 18;
        for(int i = 0 ; i < 2 ; i++)
        {
            mvwprintw(window2, 4, xPos, "%s", signIN_menu[i]);
            xPos += 40;
        }
    }
    else if(index == SIGN_UP)
    {
        xPos = 18;
        for(int i = 0 ; i < 2 ; i++)
        {
            mvwprintw(window2, 4, xPos, "%s", signUP_menu[i]);
            xPos += 40;
        }
    }
    else if(index == WITHDRAWAL)
    {
        xPos = 18;
        for(int i = 0 ; i < 2 ; i++)
        {
            mvwprintw(window2, 4, xPos, "%s", withdrawal_menu[i]);
            xPos += 40;
        }
    }
    else if(index == GAME)
    {
       
        window2 = newwin(24, 20, 0, 60);
        wbkgd(window2, COLOR_PAIR(2));
        mvwprintw(window2, 18, 8, "%s", game_menu);
    }
    else if(index == PLAYERINFO){
        mvwprintw(window2, 4, 35, "%s", "WAITING");
        window_buf = PLAYERINFO;
        write(sockfd, &window_buf, 4);
        wrefresh(window2);
    }
    wrefresh(window2);
}

void cursorf4(int index)
{
    int input;
    keypad(window1, TRUE);
    curs_set(1);

    while(1)
    {
        input = wgetch(window1);
        noecho();
        if(input == KEY_LEFT)
        {
            if(game_LeftRight == 19)
                game_LeftRight = 39;
            else
                game_LeftRight -= 4;
        }
        else if(input == KEY_RIGHT)
        {
            if(game_LeftRight == 39)
                game_LeftRight = 19;
            else
                game_LeftRight += 4;
        }
        else if(input == KEY_UP)
        {
            if(game_UPDOWN == 6)
                game_UPDOWN = 16;
            else 
                game_UPDOWN -= 2;
        }
        else if(input == KEY_DOWN)
        {
            if(game_UPDOWN == 16)
                game_UPDOWN = 6;
            else
                game_UPDOWN += 2;
        }
        else if(input == 'x')
        {
            if(index == GAME)
            {
                echo();
                print_window2(index);
            }
        }
        wmove(window1, game_UPDOWN, game_LeftRight);
    }
}

void cursorf3(int index){
    //int cursor = 0;
    
    int input;
    while(1)
    {
        input = wgetch(window2);
        if(input == KEY_LEFT)
        {        
            if(cursor == 0)
                cursor = 2;
            else
                --cursor;
        }
        else if(input == KEY_RIGHT)
        {       
            if(cursor == 2)
                cursor = 0;
            else
               ++cursor;       
        } 
        else if(input == 10)
        {
            if(index == PRE_LOGIN)
            {
                if(cursor == 0){
                    print_window1(SIGN_IN);
                    print_just_menu(SIGN_IN);
                    input_ID_PW(SIGN_IN);
                    //print_window2(SIGN_IN);
                }
                else if(cursor == 1){
                    cursor = 0;
                    print_window1(SIGN_UP);
                    print_just_menu(SIGN_UP);
                    input_ID_PW(SIGN_UP);
                    //print_window2(SIGN_UP);

                }
                else if(cursor == 2){
                    endwin();
                    exit(0);
                }
            }
            else if(index == AFTER_LOGIN)
            {
                window_buf = AFTER_LOGIN;
                if(cursor == 0){
                    clear();
                    cursor = 0;
                    if(p2ready == 0){
                        print_window1(PLAYERINFO);
                        print_just_menu(PLAYERINFO);
                        read(sockfd, &p2, sizeof(p2));
                        p2ready = 1;
                        print_window1(PLAYERINFO);
                        print_window2(PLAYERINFO);
                    }
                    else if(p2ready == 1){
                        print_window1(PLAYERINFO);
                        print_window2(PLAYERINFO);
                    }
                }
                else if(cursor == 1){
                    clear();
                    cursor = 0;
                    write(sockfd, &window_buf, 4);
                    if(id_tmp == p1.id){
                        write(sockfd, &p1.id, sizeof(p1.id));
                        p1 = p2;
                        p2ready = 0;
                    }
                    else if(id_tmp == p2.id){
                        write(sockfd, &p2.id, sizeof(p2.id));
                        p2ready = 0;
                    }
                    print_window1(PRE_LOGIN);
                    print_window2(PRE_LOGIN);
                }
                else if(cursor == 2){
                    clear();
                    cursor = 0;
                    print_window1(WITHDRAWAL);
                    print_just_menu(WITHDRAWAL);
                    input_ID_PW(WITHDRAWAL);
                    //print_window2(WITHDRAWAL);
                }
            }
        }
        print_window2(index);
    }
    
}

void cursorf2(int index){    
    int input;
    while(1)
    {  
        input = wgetch(window2);
        if(input == KEY_LEFT)
        {        
            if(cursor == 0)
                cursor = 1;
            else
                --cursor;
        }
        else if(input == KEY_RIGHT)
        {       
            if(cursor == 1)
                cursor = 0;
            else
               ++cursor;       
        } 
        else if(input == 10)
        {
            if(index == SIGN_IN)
            {
                if(cursor == 0){
                    user_DB(SIGN_IN);
                }
                else if(cursor == 1){
                    clear();
                    cursor = 0;
                    print_window1(PRE_LOGIN);
                    print_window2(PRE_LOGIN);
                }
            }
            else if(index == SIGN_UP)
            {
                if(cursor ==0){
                    user_DB(SIGN_UP);
                }
                else if(cursor == 1){
                    clear();
                    cursor = 0;
                    print_window1(PRE_LOGIN);
                    print_window2(PRE_LOGIN);
                }
            }
            else if(index == WITHDRAWAL)
            {
                if(cursor ==0){
                    user_DB(WITHDRAWAL);
                }
                else if(cursor == 1){
                    clear();
                    cursor = 0;
                    print_window1(AFTER_LOGIN);
                    print_window2(AFTER_LOGIN);
                }
            }
        }
        //print_window1(index);
        print_window2(index);
    }
}

void cursorf1(int index){
    int input;
    while(1)
    {
        input = wgetch(window2);
        if(input == 10)
        {
            if(index == PLAYERINFO)
            {
                print_just_menu(GAME);
                print_window1(GAME);
            }
            else if(index == GAME)
            {
                print_window1(AFTER_LOGIN);
                print_window2(AFTER_LOGIN);
            }
        }
        else
            continue;
        print_window1(index);
        print_window2(index);
    }
}

void input_ID_PW(int index){
    if(index == WITHDRAWAL)
    {
        curs_set(1);
        keypad(window1, TRUE);
    
        wmove(window1, 9, 32);
        wgetstr(window1,pw_tmp);
    
        curs_set(0);
 
        mvwprintw(window1, 9, 32, "%s", pw_tmp);

        print_window2(index);
        cursorf2(index);
    }
    else if(index == SIGN_IN){
        
        curs_set(1);
        keypad(window1, TRUE);
    
        wmove(window1, 7, 32);
        wgetstr(window1,id_tmp);
    
        wmove(window1, 9, 32);
        wgetstr(window1,pw_tmp);
    
        curs_set(0);
 
        mvwprintw(window1, 7, 32, "%s", id_tmp);
        mvwprintw(window1, 9, 32, "%s", pw_tmp);

        print_window2(index);   
        cursorf2(index);
    }
    else
    {
        curs_set(1);
        keypad(window1, TRUE);
    
        wmove(window1, 7, 32);
        wgetstr(window1,id_tmp);
    
        wmove(window1, 9, 32);
        wgetstr(window1,pw_tmp);
    
        curs_set(0);
 
        mvwprintw(window1, 7, 32, "%s", id_tmp);
        mvwprintw(window1, 9, 32, "%s", pw_tmp);

        print_window2(index);   
        cursorf2(index);
    }
}

int main(int argc, char**argv){
    
    if(argc != 3){
        perror("Usage :  <IP>, <PORT>");
        exit(0);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket() error : ");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    servaddr.sin_port = htons(atoi(argv[2]));

    if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect() error : ");
        exit(0);
    }
    
    initscr();
    cursor = 0;
    game_LeftRight = 27;
    game_UPDOWN = 10;
    if(has_colors() == FALSE){
        puts("Terminal does not supprot colors!");
        endwin();
        return 1;
    } else{
        start_color();
        init_pair(1, COLOR_MAGENTA, COLOR_WHITE);          //init_pair(number, letter_color, background_color)
        init_pair(2, COLOR_WHITE, COLOR_MAGENTA);
    }
    
    refresh();

    window1 = newwin(16, 80, 0, 0);                    //newwin(height, width, xPos, yPos)
    window2 = newwin(8, 80, 16, 0);
    mkdir("ID", 0755);
    keypad(window2, TRUE);

    print_window1(1);
    print_window2(1);
    
    wrefresh(window1);                                  //refresh == show GUI
    wrefresh(window2);
}