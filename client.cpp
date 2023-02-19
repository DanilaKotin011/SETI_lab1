#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <strings.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFLEN 1024

using namespace std;


int number=3;
char *mes="3";
int port = 0;
char *ip_string="127.0.0.1";
in_addr ip_to_num;
bool with_end=false;

void getParameters(int argc, char* argv[]){
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-p")==0 || strcmp(argv[i],"--port")==0){
            i++;
            port=stoi(argv[i]);
            continue;
        }
        else if(strcmp(argv[i],"-n")==0 || strcmp(argv[i],"--number")==0){
            i++;
            number=stoi(argv[i]);
            mes=argv[i];
            continue;
        }
        else if(strcmp(argv[i],"-i")==0 || strcmp(argv[i],"--ip")==0){
            i++;
            ip_string=argv[i];
            continue;
        }
        else if(strcmp(argv[i],"-e")==0 || strcmp(argv[i],"--end")==0){
            with_end=true;
            continue;
        }
    }
}

int main(int argc, char *argv[])
{
    char buf[BUFLEN];
    int ClientSock;
    unsigned int length;
    struct sockaddr_in servAddr;
    int err=0;

    getParameters(argc,argv);
    err = inet_pton(AF_INET, ip_string, &ip_to_num);
    if (err < 0)
    {
        cout << "Error inet_pton" << endl;
        return 1;
    }

    ClientSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (ClientSock < 0)
    {
        cout << "Error initialization socket" << endl;
        return 1;
    }
    else
        cout << "Client socket initialization is OK" << endl;

    bzero((char *)&servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    while (1)
    {
        sleep(number);
        length = sizeof(servAddr);
        sendto(ClientSock, (const char *)argv[1], strlen(argv[1]), 0, (const struct sockaddr *)&servAddr, sizeof(servAddr));
        int n = recvfrom(ClientSock, (char *)buf, BUFLEN, 0, (struct sockaddr *)&servAddr, &length);
        if (n < 0)
            cout << "Cant recv" << endl;
        else
        {
            buf[n] = '\0';
            std::cout << "Server :" << buf << std::endl;
            cout << "ip - " << inet_ntoa(servAddr.sin_addr) << endl;
            cout << "port - " << ntohs(servAddr.sin_port) << endl;
            cout << endl;
        }
    }
}
