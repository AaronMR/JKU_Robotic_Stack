#include <sys/socket.h>
#include <netinet/in.h>
#include "parser.h"
#include "comStruc.h"
#include "structType_S.hpp"

class AaronMR_S
{
public:
    AaronMR_S(char * aux);
    ~AaronMR_S();
    int create_Socket();
    int connect_Socket();
    socklen_t addr_size;
	int csock;
	pthread_t thread_id;
    int hsock;
    sockaddr_in sadr;
    DataLayout configuration[5];

    static void* SocketHandler( void* args ) ;

    int waitEvent();


    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    socklen_t addrlen;
    struct sockaddr_storage remoteaddr; // client address
    char remoteIP[INET6_ADDRSTRLEN];

    int startThread(int csock);

private:
    int host_port;
    char* host_name;
    char* Subscriber_name;
    char* Publisher_name;
    struct sockaddr_in my_addr;
    int buffer_len;
    char buffer[1024];
    int numProcess;


};



