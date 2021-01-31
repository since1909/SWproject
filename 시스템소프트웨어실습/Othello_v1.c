#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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
char *playerINFO_menu = "OK";
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

char id[100];
char pw[100];
int win;
int lose;
char  id_tmp[100];
char  pw_tmp[100];
char* win_tmp = "7";
char* lose_tmp = "3";

void user_DB(int index)
{
    int pw_data, win_data, lose_data;

    if(index == SIGN_UP)
    {
        chdir("ID");
        if(chdir(id_tmp))
        {
            mvwprintw(window2, 7, 0, ">>>Welcome to OTHELLO World! (Press any key)");
            wrefresh(window2);

            strcpy(id, id_tmp);
            strcpy(pw, pw_tmp);
            mkdir(id, 0755);
            chdir(id);
            pw_data = open("password", O_WRONLY | O_CREAT | O_TRUNC, 0755);
            win_data = open("win",O_WRONLY | O_CREAT | O_TRUNC, 0755);
            lose_data = open("lose",O_WRONLY | O_CREAT | O_TRUNC, 0755);
   
            write(pw_data, pw, strlen(pw));
            write(win_data, win_tmp, strlen(win_tmp));
            write(lose_data, lose_tmp, strlen(lose_tmp));
   
            close(pw_data);
            close(win_data);
            close(lose_data);
            chdir("..");

            wgetch(window2);
            print_window1(PRE_LOGIN);
            print_window2(PRE_LOGIN);

        }
        else
        {
            mvwprintw(window2, 7, 0, ">>> %s has already exist in DB! (Press any key)", id_tmp );
            wrefresh(window2);
            chdir("..");
            
            wgetch(window2);            
            print_window1(SIGN_UP);
            print_just_menu(SIGN_UP);
            input_ID_PW(SIGN_UP);
        }
    }
    else if(index == SIGN_IN)
    {
        chdir("ID");
        if(!chdir(id_tmp))
        {
            pw_data = open("password", O_RDONLY);
            read(pw_data, pw, strlen(pw_tmp));
            if(!strncmp(pw, pw_tmp, strlen(pw)))
            {
                win_data = open("win", O_RDONLY);
                read(win_data, win_tmp, strlen(win_tmp));
                win = atoi(win_tmp);
                lose_data = open("lose", O_RDONLY);
                read(lose_data, lose_tmp, strlen(lose_tmp));
                lose = atoi(lose_tmp);
                chdir("..");
                print_window1(AFTER_LOGIN);
                print_window2(AFTER_LOGIN);
            }
            else
            {
                mvwprintw(window2, 7, 0, ">>>Login error (Press any key)");
                wrefresh(window2);
                wgetch(window2);            
                chdir("..");       
                print_window1(SIGN_IN);
                print_just_menu(SIGN_IN);
                input_ID_PW(SIGN_IN);
            }        
        }
        else
        {
            mvwprintw(window2, 7, 0, ">>>Login error (Press any key)");
            wrefresh(window2);
            wgetch(window2);            
            chdir("..");            
            print_window1(SIGN_IN);
            print_just_menu(SIGN_IN);
            input_ID_PW(SIGN_IN);
        }
    }
    else if(index == WITHDRAWAL)
    {
        chdir("ID");
        if(chdir(id_tmp))
        {
            mvwprintw(window2, 7, 0, ">>>Sign out error (Press any key)");            
            wrefresh(window2);
            wgetch(window2);
            //chdir("..");
            print_window1(WITHDRAWAL);
            print_just_menu(WITHDRAWAL);
            input_ID_PW(WITHDRAWAL);
        }
        else
        {
            pw_data = open("password", O_RDONLY);
            read(pw_data, pw, strlen(pw_tmp));
            if(!strncmp(pw, pw_tmp, strlen(pw)))
            {
                close(pw_data);
                remove("password");
                remove("win");
                remove("lose");
                chdir("..");
                rmdir(id_tmp);
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
                chdir("..");        
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
        mvwprintw(window1, 4, 27, "PLAYER ID : ");
        mvwprintw(window1, 4, 39, "%s", id_tmp);
        mvwprintw(window1, 7, 36, "STATISTICS");
        mvwprintw(window1, 9, 26, "WIN : %d / LOSE : %d (%lf%) ", win, lose, (double)((double)win/(win+lose))*100);
        wrefresh(window1);
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
                if(cursor == 0){
                    clear();
                    cursor = 0;
                    print_window1(PLAYERINFO);
                    print_window2(PLAYERINFO);
                }
                else if(cursor == 1){
                    clear();
                    cursor = 0;
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

int main(int argc, int *argv[]){
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