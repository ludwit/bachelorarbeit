#include	<netinet/in.h> 
#include	<arpa/inet.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<sys/uio.h>
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>
#include    <pthread.h> 
#include    <ctype.h>
#include    <sys/select.h>
#include    <sys/time.h>
#include    <termios.h>
#include    <sys/socket.h>
#include    <netinet/tcp.h>

#define BYTES 1460

struct threadargs {
    int recvfd;
    int sendfd;
};