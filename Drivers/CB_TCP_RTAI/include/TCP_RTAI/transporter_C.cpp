#include "transporter_C.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include "pack2.hpp"

#include <fcntl.h>


#include <sys/poll.h>





//##############################################################################
/*----------------------------------------------------------------------
 Portable function to set a socket into nonblocking mode.
 Calling this on a socket causes all future read() and write() calls on
 that socket to do only as much as they can immediately, and return
 without waiting.
 If no data can be read or written, they return -1 and set errno
 to EAGAIN (or EWOULDBLOCK).
 Thanks to Bjorn Reese for this code.
----------------------------------------------------------------------*/
int setNonblocking(int fd)
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}

//##############################################################################

int exitClient(int hsock);

transporter_C::transporter_C(char * aux)
{

    int counter = 0;


    parseFile parser(aux);
    parser.Parse(configuration);

    host_port = atoi(configuration[0].PORT_RTAI.data());
    host_name = (char*)configuration[0].IP_RTAI.data();
    Subscriber_name = (char*)configuration[0].Subscriber.data();
    Publisher_name = (char*)configuration[0].Publisher.data();

    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(host_port);

    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = inet_addr(host_name);


    //----------------Configure type of struct to send--------------------------
    if(configuration[0].Node2RTAI.compare("Twist") == 0)
    {
        structToSend = new struct_Twist;
        structToSend->set_Subscriber((char*)configuration[0].Subscriber.data());

    }else if(configuration[0].Node2RTAI.compare("odom") == 0)
    {
        structToSend = new struct_Odometry;
        structToSend->set_Subscriber((char*)configuration[0].Subscriber.data());

    }else if(configuration[0].Node2RTAI.compare("Joy") == 0)
    {
        structToSend = new struct_Joy;
        structToSend->set_Subscriber((char*)configuration[0].Subscriber.data());

    }else if(configuration[0].Node2RTAI.compare("Pose") == 0)
    {
        structToSend = new struct_Pose;
        structToSend->set_Subscriber((char*)configuration[0].Subscriber.data());

    }
    else if(configuration[0].Node2RTAI.compare("posWheels") == 0)
    {
        structToSend = new struct_posWheels;
        structToSend->set_Subscriber((char*)configuration[0].Subscriber.data());

    }

    //---------------configure type of struct to receive--------------------------
    if(configuration[0].RTAI2Node.compare("Twist") == 0)
    {
        structToRecv = new struct_Twist;
        structToRecv->set_Publisher((char*)configuration[0].Publisher.data());

    }else if(configuration[0].RTAI2Node.compare("odom") == 0)
    {
        structToRecv = new struct_Odometry;
        structToRecv->set_Publisher((char*)configuration[0].Publisher.data());

    }else if(configuration[0].RTAI2Node.compare("Joy") == 0)
    {
        structToRecv = new struct_Joy;
        structToRecv->set_Publisher((char*)configuration[0].Publisher.data());
    }else if(configuration[0].RTAI2Node.compare("Pose") == 0)
    {
        structToRecv = new struct_Pose;
        structToRecv->set_Publisher((char*)configuration[0].Publisher.data());
    }
    else if(configuration[0].RTAI2Node.compare("posWheels") == 0)
    {
        structToRecv = new struct_posWheels;
        structToRecv->set_Publisher((char*)configuration[0].Publisher.data());
    }

    configuration[0].mutex = PTHREAD_MUTEX_INITIALIZER;
    configuration[0].canRecv = false;
    configuration[0].canSend = false;

    counter = counter + 1;

    buffer_len = 1024;
    memset(buffer, '\0', buffer_len);

};

/*
int transporter_C::SendRecv()
{
    int s1, s2;
    int rv;
    char buf1[256], buf2[256];
    pollfd ufds[2];
    s1 = socket(PF_INET, SOCK_STREAM, 0);
    s2 = socket(PF_INET, SOCK_STREAM, 0);

    ufds[0].fd = s1;
    ufds[0].events = POLLIN | POLLPRI; // check for normal or out-of-band
    ufds[1].fd = s2;
    ufds[1].events = POLLIN; // check for just normal data

    // wait for events on the sockets, 3.5 second timeout
    rv = poll(ufds, 2, 30500);

    if (rv == -1)
    {
        perror("poll"); // error occurred in poll()

    } else if (rv == 0)
    {
        printf("Timeout occurred! No data after 3.5 seconds.\n");

    } else {
        // check for events on s1:
        if (ufds[0].revents & POLLIN)
        {
            recv(s1, buf1, sizeof buf1, 0); // receive normal data
        }
        if (ufds[0].revents & POLLPRI)
        {
            recv(s1, buf1, sizeof buf1, MSG_OOB); // out-of-band data
        }
        // check for events on s2:
        if (ufds[1].revents & POLLIN)
        {
            recv(s1, buf2, sizeof buf2, 0);
        }
    }

    return 0;
}

*/


int transporter_C::send_Data()
{
    char data[1024];
    int bytecount = 0;

    structToSend->spinOnce();

    if(structToSend->canSend())
    {
        structToSend->serialize(data);
        if(configuration[0].DEBUG)
            structToSend->printStruct(data);

        if( (bytecount=send(hsock, (const void*)data, 1024,0)) < 1)
        {
            fprintf(stderr, "send_Data Error sending data %d\n", errno);
            exitClient(hsock);
            return -1;
        }
    }
    return 0;
};

int transporter_C::recv_Data()
{
    char data[1024];
    int bytecount = 0;

    if( (bytecount=recv(hsock, (void*)data, 1024,0)) < 1)
    {
        fprintf(stderr, "recv_Data Error reciving data %d\n", errno);
        exitClient(hsock);
        return -1;
    }

    if(configuration[0].DEBUG)
            structToRecv->printStruct(data);

    structToRecv->Unserialize(data);
    return 0;
}

int transporter_C::create_Socket()
{
    hsock = socket(AF_INET, SOCK_STREAM, 0);
    if(hsock == -1)
    {
        printf("Error initializing socket %d\n",errno);
        return -1;
    }
    return hsock;
}

int transporter_C::connect_Socket()
{
    int err;
    int bytecount = 0;
    char data[1024];


    if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 )
    {
        if((err = errno) != EINPROGRESS)
        {
            fprintf(stderr, "Error connecting socket %d\n", errno);
            exitClient(hsock);
            return -1;
        }
    }
    buffer_len = 1024;
    memset(buffer, '\0', buffer_len);
    strcpy(buffer, configuration[0].name.c_str());

    SetupCode(data);

    if( (bytecount=send(hsock, (const void*)data, 1024,0)) < 1)
    {
        fprintf(stderr, "send_Data Error sending data %d\n", errno);
        exitClient(hsock);
        return -1;
    }

    memset(buffer, '\0', buffer_len);

    if((bytecount = recv(hsock, buffer, buffer_len, 0)) < 1)
    {
        fprintf(stderr, "Error receiving data %d\n", errno);
        exitClient(hsock);
    }

    unsigned char magic;
    unsigned int ps2;
    char s2[96];

    unpack((unsigned char*)buffer, "CHs",   &magic,
                                            &ps2,
                                            &s2);

    printf("recv: '%c' %hhu %s\n",  magic,
                                    ps2,
                                    s2);

    if(magic == 69)
    {
        cout << "se cerro"<< endl;
        exitClient(hsock);
        return -1;
    }
    return 0;
}



transporter_C::~transporter_C()
{

};

int transporter_C::SetupCode(char* buffer)
{
	unsigned char buf[1024];
	unsigned char magic;
	unsigned int packetsize, ps2;

    char s2[96];
    char s3[96];
    char s4[96];

	packetsize = pack(buf, "CHsss", 'S', 0, configuration->name.c_str(),
                                            configuration->Node2RTAI.c_str(),
                                            configuration->RTAI2Node.c_str());

	packi16(buf+1, packetsize);

    memcpy((unsigned char*)buffer, buf, packetsize);

	unpack((unsigned char*)buffer, "CHsss",    &magic,
                                            &ps2,
                                            &s2,
                                            &s3,
                                            &s4);

	printf("send: '%c' %hhu %s %s %s\n",   magic,
                                     ps2,
                                     s2,
                                     s3,
                                     s4);

    return NULL;
}

int exitClient(int hsock)
{
    return 0;
}


