/***
 * refer to: conio.h of windows version
 * Copyright: written by Cai Jun(nickche3000@163.com) 2017- All rights reserved.
 * You can use it anywhere, please keep the above description.
*/
#ifndef TINY_CONIO_H
#define TINY_CONIO_H
 
#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
 
static struct termios old, options;

void initTermios(int echo)
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    options = old; /* make new settings same as old settings */
 
    //Get the current options for the port...
    tcgetattr(0, &options);
 
    //Set the baud rates
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
 
    //Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);
 
    //8 data bits
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    //CSTOPB -- 2 stop bits (1 otherwise)
 
    //1 stop bit
    options.c_cflag &= ~CSTOPB;
 
    //no parity
    options.c_cflag &= ~PARENB;
 
    //use rts/cts flow control
    options.c_cflag &= (~CRTSCTS);
 
    //disable software flow control
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
 
    //raw type input
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 
    //raw type output
    options.c_oflag &= ~OPOST;
 
    //disable 0d->0xa mapping
    options.c_iflag &= ~(INLCR | ICRNL | IGNCR);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_cc[VTIME] = 0;//40; //40; /* Set timeout of 4 seconds */
    options.c_cc[VMIN] = 1;  //0; /* must readed 1 char, then return */
    //Set the new options for the port...
    //tcsetattr(fd, TCSANOW, &options);
    tcsetattr(0, TCSAFLUSH, &options);
    tcflush(0, TCIOFLUSH);
}

void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}
 
void clearTermios(void)
{
    old.c_lflag &=  ECHO;
    tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

char getch(void)
{
    return getch_(0);
}

char getche(void)
{
    return getch_(1);
}

#endif // __linux__
#endif