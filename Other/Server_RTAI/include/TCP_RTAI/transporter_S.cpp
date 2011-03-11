#include "transporter_S.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include "pack2.hpp"
#include <signal.h>
#include <pthread.h>

// time to sleep threads and principal proces
int usleepServer = 100000;
int usleepThread = 100000;

int exitClient(int hsock);


void sig_pipe(int signum)
{
    signal(SIGPIPE, sig_pipe);
}

transporter_S::transporter_S(char * aux)
{

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    DataLayout auxFile[5];
    parseFile parser(aux);
    parser.Parse(auxFile);
    numProcess = parser.getNumProcess();

    pthread_mutexattr_t mta;
    pthread_mutexattr_init(&mta);



    for(int i=0;i<numProcess;i++)
    {
        configuration[i].active = auxFile[i].active;
        configuration[i].csock = auxFile[i].csock;
        configuration[i].IP_RTAI = auxFile[i].IP_RTAI.data();
        configuration[i].name = auxFile[i].name.data();
        configuration[i].Node2RTAI = auxFile[i].Node2RTAI.data();
        configuration[i].PORT_RTAI = auxFile[i].PORT_RTAI.data();
        configuration[i].Publisher = auxFile[i].Publisher.data();
        configuration[i].RTAI2Node = auxFile[i].RTAI2Node.data();
        configuration[i].SHM_IN = auxFile[i].SHM_IN.data();
        configuration[i].SHM_OUT = auxFile[i].SHM_OUT.data();
        configuration[i].Subscriber = auxFile[i].Subscriber.data();

        //configuration[i].mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&configuration[i].mutex, &mta);
        configuration[i].canRecv = false;
        configuration[i].canSend = false;
        configuration[i].DEBUG = false;
        configuration[i].DEBUG = auxFile[i].DEBUG;
    }

    numProcess = parser.getNumProcess();
    if(numProcess == 0)
    {
        cout << "No hay ningun proceso configurado" << endl;
    }else{
        cout << "Procesos configurados = " << parser.getNumProcess() <<endl;
    }

	int host_port= 1101;

	int * p_int ;
//	int err;

	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n", errno);
		goto FINISH;
	}

	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;

	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", errno);
		free(p_int);
		goto FINISH;
	}
	free(p_int);

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);

	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;

	if( bind( hsock, (sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		fprintf(stderr,"Error binding to socket, make sure nothing else is listening on this port %d\n",errno);
		goto FINISH;
	}
	if(listen( hsock, 10) == -1 ){
		fprintf(stderr, "Error listening %d\n",errno);
		goto FINISH;
	}

	FD_SET(hsock, &master);

	// keep track of the biggest file descriptor
    fdmax = hsock; // so far, it's this one

	//Now lets do the server stuff

	addr_size = sizeof(sockaddr_in);


FINISH:
;
    return;
};


int transporter_S::create_Socket()
{
    hsock = socket(AF_INET, SOCK_STREAM, 0);
    if(hsock == -1)
    {
        printf("Error initializing socket %d\n",errno);
        return -1;
    }
    return hsock;
}


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int transporter_S::waitEvent()
{
    listener = hsock;
    for(;;) {

        usleep(usleepServer);
        read_fds = master;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        int i=0;
        for(i = 0; i <= fdmax; i++) {

            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(hsock,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);

                        startThread(newfd);
                    }
                } else {
                    int numberThread = -1;
                    for(int w = 0; w < 5 ; w++)
                    {

                        if(configuration[w].csock == i)
                        {
                            numberThread = w;
                        }
                    }

                    if(numberThread != -1)
                    {
                        pthread_mutex_lock(&configuration[numberThread].mutex);
                        configuration[numberThread].canRecv = true; //rand() % 2 + -1;
                        pthread_mutex_unlock(&configuration[numberThread].mutex);

                    }

                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    //########################################################################
    return 0;
}

//############################################################################################

int transporter_S::startThread(int csock)
{
    printf("---------------------\nReceived connection from %s\n",inet_ntoa(sadr.sin_addr));
    cout << "---------------------------" << endl;

    char buffer[1024];
    int buffer_len = 1024;
    int bytecount;
    memset(buffer, 0, buffer_len);
    DataLayout processThread;

    unsigned char magic;
    unsigned int ps2;
    char s2[96];
    char s3[96];
    char s4[96];
    unsigned char buf[1024];

    char data[1024];

    if( (bytecount=recv(csock, (void*)data, 1024,0)) < 1)
    {
        fprintf(stderr, "recv_Data Error reciving data %d\n", errno);
        exitClient(hsock);
        return -1;
    }


    memcpy((unsigned char*)buf, data, 1024);

    unpack((unsigned char*)buf, "CHsss",    &magic,
                                            &ps2,
                                            &s2,
                                            &s3,
                                            &s4);

    printf("recv: '%c' %hhu %s %s %s\n",    magic,
                                            ps2,
                                            s2,
                                            s3,
                                            s4);

    int existProcess = 0;
    int activeProcess = 0;
    int process2Active = -1;
    for( int i = 0; i < numProcess ; i++)
    {
        if(configuration[i].name.compare(s2) == 0)
        {
            cout << "encontre el proceso a encender = " << s2 << endl;
            process2Active = i;
        }
    }

    if(process2Active > -1)
    {
        cout << "active =" << configuration[process2Active].active << endl;
        if(configuration[process2Active].active == 0)
        {
            configuration[process2Active].active = 1;
            existProcess = 1;
            activeProcess = 1;

            processThread.name = configuration[process2Active].name;
            processThread.IP_RTAI = configuration[process2Active].IP_RTAI;
            processThread.Node2RTAI = configuration[process2Active].Node2RTAI;
            processThread.PORT_RTAI = configuration[process2Active].PORT_RTAI;
            processThread.RTAI2Node = configuration[process2Active].RTAI2Node;
            processThread.SHM_IN = configuration[process2Active].SHM_IN;
            processThread.SHM_OUT = configuration[process2Active].SHM_OUT;

            processThread.csock = configuration[process2Active].csock = csock;

            unsigned int packetsize, ps2;

            char s2[96];
            char buffer[1024];
            packetsize = pack(buf, "CHs", 'A', 0, "Existe el proceso");

            packi16(buf+1, packetsize);


            memcpy((unsigned char*)buffer, buf, packetsize);


            if( (bytecount=send(csock, (void*)buffer, 1024,0)) < 1)
            {
                fprintf(stderr, "recv_Data Error reciving data %d\n", errno);
                exitClient(hsock);
                return -1;
            }

            unpack((unsigned char*)buffer, "CHs",   &magic,
                                                    &ps2,
                                                    &s2);

            printf("send: '%c' %hhu %s\n",  magic,
                                            ps2,
                                            s2);

            pthread_create( &thread_id, 0, &transporter_S::SocketHandler, (void*)&configuration[process2Active]);
            pthread_detach(thread_id);

        }else{

            unsigned int packetsize, ps2;
            char s2[96];
            char buffer[1024];
            packetsize = pack(buf, "CHs", 'E', 0, "Proceso Activo");

            packi16(buf+1, packetsize);

            memcpy((unsigned char*)buffer, buf, packetsize);

            if( (bytecount=send(csock, (void*)buffer, 1024,0)) < 1)
            {
                fprintf(stderr, "recv_Data Error reciving data %d\n", errno);
                exitClient(hsock);
                return -1;
            }

            unpack((unsigned char*)buffer, "CHs",   &magic,
                                                    &ps2,
                                                    &s2);

            printf("send: '%c' %hhu %s\n",  magic,
                                            ps2,
                                            s2);
        }

    }

    if(existProcess == 0)
    {
        cout << "no existe el proceso con nombre " << s2 << endl;
        unsigned int packetsize, ps2;
        char s2[96];
        char buffer[1024];
        packetsize = pack(buf, "CHs", 'E', 0, "no existe");

        packi16(buf+1, packetsize);

        memcpy((unsigned char*)buffer, buf, packetsize);

        if( (bytecount=send(csock, (void*)buffer, 1024,0)) < 1)
        {
            fprintf(stderr, "recv_Data Error reciving data %d\n", errno);
            return -1;
        }

        unpack((unsigned char*)buffer, "CHs",   &magic,
                                                &ps2,
                                                &s2);

        printf("send: '%c' %hhu %s\n",  magic,
                                        ps2,
                                        s2);
    }

    if(activeProcess == 0)
    {
        cout << "el proceso ya esta activo " << s2 << endl;
        unsigned int packetsize, ps2;
        char s2[96];
        char buffer[1024];
        packetsize = pack(buf, "CHs", 'E', 0, "proceso activo");

        packi16(buf+1, packetsize);

        memcpy((unsigned char*)buffer, buf, packetsize);


        if( (bytecount=send(csock, (void*)buffer, 1024,0)) < 1)
        {
            fprintf(stderr, "recv_Data Error reciving data %d\n", errno);
            return -1;
        }

        unpack((unsigned char*)buffer, "CHs",   &magic,
                                                &ps2,
                                                &s2);

        printf("send: '%c' %hhu %s\n",  magic,
                                        ps2,
                                        s2);
    }
    return 0;
}

int transporter_S::connect_Socket()
{
    int err;
    int bytecount = 0;
    if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 )
    {
        if((err = errno) != EINPROGRESS)
        {
            fprintf(stderr, "Error connecting socket %d\n", errno);
            exitClient(hsock);
        }
    }

    buffer_len = 1024;
    memset(buffer, '\0', buffer_len);
    memcpy(buffer, configuration[0].name.data(), configuration[0].name.length() );
    cout << "The name of the service is = " << buffer << endl;

    if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1)
    {
        fprintf(stderr, "Error sending data %d\n", errno);

        exitClient(hsock);
    }

    memset(buffer, '\0', buffer_len);

    if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1)
    {
        fprintf(stderr, "Error receiving data %d\n", errno);
        exitClient(hsock);
    }
    printf("Recieved bytes %d\nReceived string \"%s\"\n", bytecount, buffer);

    if(strcmp(buffer, "close") == 0)
    {
        cout << "se cerro"<< endl;
        exitClient(hsock);
    }
    return 0;
}

transporter_S::~transporter_S()
{

};

void* transporter_S::SocketHandler(void* lp){

    signal(SIGPIPE, sig_pipe);

    struct DataLayout *temp = (struct DataLayout *)lp;
    struct DataLayout processThread_2;
    processThread_2.active = temp->active;

    processThread_2.name = temp->name;
    processThread_2.IP_RTAI = temp->IP_RTAI;
    processThread_2.Node2RTAI = temp->Node2RTAI;
    processThread_2.PORT_RTAI = temp->PORT_RTAI;
    processThread_2.RTAI2Node = temp->RTAI2Node;
    processThread_2.SHM_IN = temp->SHM_IN;
    processThread_2.SHM_OUT = temp->SHM_OUT;
    processThread_2.active = temp->active;
    processThread_2.csock = temp->csock;
    processThread_2.DEBUG = temp->DEBUG;

    temp->canRecv = true;
    temp->canSend = true;

    int csock = processThread_2.csock;
    int counter = 0;

    structType_S* structToSend;
    structType_S* structToRecv;


    /** configuration send structure **/
    if(processThread_2.Node2RTAI.compare("Twist") == 0)
    {
        structToRecv = new struct_Twist;
        structToRecv->iniSHM(1, 0, (char*)processThread_2.SHM_IN.data());

    }else if(processThread_2.Node2RTAI.compare("odom") == 0)
    {
        structToRecv = new struct_Odometry;
        structToRecv->iniSHM(1, 0, (char*)processThread_2.SHM_IN.data());

    }else if(processThread_2.Node2RTAI.compare("Joy") == 0)
    {
        structToRecv = new struct_Joy;
        structToRecv->iniSHM(1, 0, (char*)processThread_2.SHM_IN.data());

    }else if(processThread_2.Node2RTAI.compare("Pose") == 0)
    {
        structToRecv = new struct_Pose;
        structToRecv->iniSHM(1, 0, (char*)processThread_2.SHM_IN.data());

    }else if(processThread_2.Node2RTAI.compare("posWheels") == 0)
    {
        structToRecv = new struct_posWheels;
        structToRecv->iniSHM(1, 0, (char*)processThread_2.SHM_IN.data());

    }else if(processThread_2.Node2RTAI.compare("comStruc") == 0)
    {
        structToRecv = new struct_comStruc;
        structToRecv->iniSHM(1, 0, (char*)processThread_2.SHM_IN.data());

    }


    /** configuration recv structure **/
    if(processThread_2.RTAI2Node.compare("Twist") == 0)
    {
        structToSend = new struct_Twist;
        structToSend->iniSHM(0,1, (char*)processThread_2.SHM_OUT.data());

    }else if(processThread_2.RTAI2Node.compare("odom") == 0)
    {
        structToSend = new struct_Odometry;
        structToSend->iniSHM(0,1, (char*)processThread_2.SHM_OUT.data());

    }else if(processThread_2.RTAI2Node.compare("Joy") == 0)
    {
        structToSend = new struct_Joy;
        structToSend->iniSHM(0,1, (char*)processThread_2.SHM_OUT.data());

    }else if(processThread_2.RTAI2Node.compare("Pose") == 0)
    {
        structToSend = new struct_Pose;
        structToSend->iniSHM(0,1, (char*)processThread_2.SHM_OUT.data());
    }else if(processThread_2.RTAI2Node.compare("posWheels") == 0)
    {
        structToSend = new struct_posWheels;
        structToSend->iniSHM(0,1, (char*)processThread_2.SHM_OUT.data());
    }else if(processThread_2.RTAI2Node.compare("comStruc") == 0)
    {
        structToSend = new struct_comStruc;
        structToSend->iniSHM(0,1, (char*)processThread_2.SHM_OUT.data());
    }

    counter = counter + 1;

    cout << "name = " << processThread_2.name << endl
        << "IP_RTAI = " << processThread_2.IP_RTAI << endl
        << "Node2RTAI = " << processThread_2.Node2RTAI << endl
        << "PORT_RTAI = " << processThread_2.PORT_RTAI << endl
        << "RTAI2Node = " << processThread_2.RTAI2Node << endl
        << "SHM_IN = " << processThread_2.SHM_IN << endl
        << "SHM_OUT = " << processThread_2.SHM_OUT << endl
        << "DEBUG = " << processThread_2.DEBUG << endl
        << " = " << processThread_2.active << endl
        << " = " << processThread_2.csock << endl ;

	char buffer[1024];
	int buffer_len = 1024;
	int bytecount=0;
    memset(buffer, 0, buffer_len);

    cout << "esperando identificacion del proceso" << endl;

    printf("Received bytes %d\nReceived string \"%s\"\n", bytecount, buffer);

    while(1)
    {
        int   rc;
        char data[1024];

        int bytecount = 0;

        rc = pthread_mutex_lock(&temp->mutex);

        if(temp->canRecv)
        {

            if( (bytecount=recv(csock, (void*)data, 1024,0)) < 1)
            {
                fprintf(stderr, "Error sending data %d\n", errno);
                goto FINISH;

            }

            if(processThread_2.DEBUG)
                structToRecv->printStruct(data);

            structToRecv->Unserialize(data);
        }

        if(temp->canSend)
        {
            structToSend->serialize(data);

            if(processThread_2.DEBUG)
                structToSend->printStruct(data);

            if( (bytecount=send(csock, (void*)data, 1024,0)) < 1)
            {
                fprintf(stderr, "Error sending data %d\n", errno);
                goto FINISH;
            }

        }


        temp->canRecv = false;
        temp->canSend = true;
        rc = pthread_mutex_unlock(&temp->mutex);


        usleep(usleepThread);

    }


FINISH:

	cout << "Close thread and connecion tcp" << endl;
	temp->active = 0;
	pthread_mutex_unlock(&temp->mutex);
    return 0;
}


int exitClient(int hsock)
{
    close(hsock);
    return 0;
}
