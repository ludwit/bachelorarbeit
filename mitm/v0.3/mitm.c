#include "header.h"

void * connection_handeler(void *ptr) 
{
    pthread_detach(pthread_self());

    int recvfd, sendfd;
    char buf1[BYTES];
    char buf2[BYTES];
    char buf3[BYTES];
    char buf4[BYTES]; 
    ssize_t nbytes1;
    ssize_t nbytes2;
    ssize_t nbytes3;
    ssize_t nbytes4;
    struct threadargs *args = ptr;

    recvfd = args->recvfd;
    sendfd  = args->sendfd;

    for (; ;) {

        /* recv message */
        if ( (nbytes1 = recv(recvfd, &buf1, BYTES, 0)) < 1) {
            if (nbytes1 == 0) {
                printf("thread: connection closed\n");
                break;
            } else if (errno == EBADF) {
                break; // when other thread closes connection
            } else {
                perror("thread: error reciving bytes");
                break;
            }
        }

        /* forward message */
        if (send(sendfd, &buf1, nbytes1, 0) < 0) {
            if (errno == EBADF) { 
                break; // when other thread closes connection
            } else {
                perror("thread: error forwarding bytes");
                break;
            }
        }

        /* recv message */
        if ( (nbytes1 = recv(recvfd, &buf1, BYTES, 0)) < 1) {
            if (nbytes1 == 0) {
                printf("thread: connection closed\n");
                break;
            } else if (errno == EBADF) {
                break; // when other thread closes connection
            } else {
                perror("thread: error reciving bytes");
                break;
            }
        }

        /* forward message */
        if (send(sendfd, &buf1, nbytes1, 0) < 0) {
            if (errno == EBADF) { 
                break; // when other thread closes connection
            } else {
                perror("thread: error forwarding bytes");
                break;
            }
        }

        /* recv message */
        if ( (nbytes1 = recv(recvfd, &buf1, BYTES, 0)) < 1) {
            if (nbytes1 == 0) {
                printf("thread: connection closed\n");
                break;
            } else if (errno == EBADF) {
                break; // when other thread closes connection
            } else {
                perror("thread: error reciving bytes");
                break;
            }
        }

        /* forward message */
        if (send(sendfd, &buf1, nbytes1, 0) < 0) {
            if (errno == EBADF) { 
                break; // when other thread closes connection
            } else {
                perror("thread: error forwarding bytes");
                break;
            }
        }
        /* recv message */
        if ( (nbytes1 = recv(recvfd, &buf1, BYTES, 0)) < 1) {
            if (nbytes1 == 0) {
                printf("thread: connection closed\n");
                break;
            } else if (errno == EBADF) {
                break; // when other thread closes connection
            } else {
                perror("thread: error reciving bytes");
                break;
            }
        }

        /* forward message */
        if (send(sendfd, &buf1, nbytes1, 0) < 0) {
            if (errno == EBADF) { 
                break; // when other thread closes connection
            } else {
                perror("thread: error forwarding bytes");
                break;
            }
        }

        /* JETZT GEHTS RUND */

                // recv message
        if ( (nbytes1 = recv(recvfd, &buf1, BYTES, 0)) < 1) {
            if (nbytes1 == 0) {
                printf("thread: connection closed\n");
                break;
            } else if (errno == EBADF) {
                break; // when other thread closes connection
            } else {
                perror("thread: error reciving bytes");
                break;
            }
        }

        if ( (nbytes2 = recv(recvfd, &buf2, BYTES, 0)) < 1) {
            if (nbytes2 == 0) {
                printf("thread: connection closed\n");
                break;
            } else if (errno == EBADF) {
                break; // when other thread closes connection
            } else {
                perror("thread: error reciving bytes");
                break;
            }
        }

                // forward message //
        if (send(sendfd, &buf2, nbytes2, 0) < 0) {
            if (errno == EBADF) { 
                break; // when other thread closes connection
            } else {
                perror("thread: error forwarding bytes");
                break;
            }
        }

                // forward message //
        if (send(sendfd, &buf1, nbytes1, 0) < 0) {
            if (errno == EBADF) { 
                break; // when other thread closes connection
            } else {
                perror("thread: error forwarding bytes");
                break;
            }
        }
        //*/
    }

    /* termination routine */
    printf("thread: exited\n");
    close(recvfd);
    close(sendfd);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int listenfd, clientfd, proxyfd, ret;
    struct sockaddr_in thisaddr, proxyaddr;
    pthread_t wasteid;
    struct threadargs clientsideargs, proxysideargs;

    /* set up listening side socket */

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("listening socket could not be created");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("listening socket cannot reuse address");
    }

    if (setsockopt(listenfd, SOL_TCP, TCP_MAXSEG, &(int){BYTES}, sizeof(int)) < 0) {
        perror("listening socket mss could not be set");
        exit(EXIT_FAILURE);
    }

    bzero(&thisaddr, sizeof(thisaddr));
    thisaddr.sin_family = AF_INET;
    thisaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    thisaddr.sin_port = htons(1080);

    if (bind(listenfd, (struct sockaddr *) &thisaddr, sizeof(thisaddr)) < 0) {
        perror("could not bind address to listening socket");
        exit(EXIT_FAILURE);
    }

    /* set up proxy side address struct */

    bzero(&proxyaddr, sizeof(proxyaddr));
    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    proxyaddr.sin_port = htons(8080);

    /* listen for incoming connections */
    if (listen(listenfd, 500) < 0) {
        perror("error listening for incoming connections");
        exit(EXIT_FAILURE);
    }

    for (; ;) {

        // wait for client to connect
        if ( (clientfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0) {
            perror("error accepting incoming connection");
            continue;
        }

        printf("main: new client connection\n");

        /* open socket to proxy server, configure and connect to it */
        if ( (proxyfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("proxy socket could not be created");
            close(clientfd);
            continue;
        }

        if (setsockopt(proxyfd, SOL_TCP, TCP_MAXSEG, &(int){BYTES}, sizeof(int)) < 0) {
            perror("proxy socket mss could not be set");
            close(clientfd);
            close(proxyfd);
            continue;
        }

        if (connect(proxyfd, (struct sockaddr *) &proxyaddr, sizeof(proxyaddr)) < 0) {
            perror("error accepting");
        }

        /* start thread to handle client side connection */

        bzero(&clientsideargs, sizeof(clientsideargs));
        clientsideargs.recvfd = clientfd;
        clientsideargs.sendfd = proxyfd;

        if ( (ret = pthread_create(&wasteid, NULL, &connection_handeler, &clientsideargs)) != 0) {
            errno = ret;
            perror("Error creating client handler thread");
            close(clientfd);
            continue;
        }

        /* start thread to handle proxy side connection */

        bzero(&proxysideargs, sizeof(proxysideargs));
        proxysideargs.recvfd = proxyfd;
        proxysideargs.sendfd = clientfd;

        if ( (ret = pthread_create(&wasteid, NULL, &connection_handeler, &proxysideargs)) != 0) {
            errno = ret;
            perror("Error creating proxy handler thread");
            close(clientfd);
            continue;
        }
    }

    // never gets here
    exit(EXIT_SUCCESS);
}



