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

void string2hexString(char* input, char* output, int size)
{
    int loop;
    int i, j;  
    i=0;
    loop=0;

    for (j = 0; j < size; j++) {
 
        sprintf((char*)(output+i),"%02X", input[loop]);
        loop+=1;
        i+=2;
    }
    //insert NULL at the end of the output string
    output[i++] = '\0';
}

void * forwarder (void *ptr)
{
    char buf[BYTES];
    char print_buf[2*BYTES];
    int recvfd, sendfd;
    int nbytes;

    struct threadargs *args = ptr;

    recvfd = args->recvfd;
    sendfd = args->sendfd;
    

    for (; ;) {
        nbytes = recv(recvfd, &buf, BYTES, 0);

        if (nbytes) {
            send(sendfd, buf, nbytes, 0);

            string2hexString(&buf, &print_buf, nbytes);
            printf("message:\n%s\n", print_buf);
            bzero(&print_buf, sizeof(print_buf));
        } else {
            perror("scheiße\n");
        }
    }
}

int main (int argc, char **argv)
{
    int listenfd, connfd, proxyfd, mss;
    struct sockaddr_in servaddr, proxyaddr;
    pthread_t client_forward, proxy_forward;
    struct threadargs client_args;
    struct threadargs proxy_args;

    // Server socket setup
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // make reusable
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    mss = BYTES;

    // set mss
    setsockopt(listenfd, IPPROTO_TCP, TCP_MAXSEG, &mss, sizeof(mss));


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(1080);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    // proxy socket setup
    proxyfd = socket(AF_INET, SOCK_STREAM, 0);

    // set mss
    setsockopt(proxyfd, IPPROTO_TCP, TCP_MAXSEG, &mss, sizeof(mss));

    bzero(&proxyaddr, sizeof(proxyaddr));
    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    proxyaddr.sin_port = htons(8080);

    // wait for connection
    listen(listenfd, 1);

    // connection inncoming
    connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

    // connect proxy
    connect(proxyfd, (struct sockaddr *) &proxyaddr, sizeof(proxyaddr));

    // filling struct for clients_forwarder
    bzero(&client_args, sizeof(client_args));
    client_args.recvfd = connfd;
    client_args.sendfd = proxyfd;

    // start thread to fwd client messages
    pthread_create(&client_forward, NULL, &forwarder, &client_args);

    // filing args struct
    bzero(&proxy_args, sizeof(proxy_args));
    proxy_args.recvfd = proxyfd;
    proxy_args.sendfd = connfd;

    //sart tehtehred
    pthread_create(&proxy_forward, NULL, &forwarder, &proxy_args);

    //wait for them
    while(1){};
}