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
    }
}

int main(int argc, char *argv[])
{
    char buf[BUFLEN];
    int ServSock = 0;
    int err = 0;

    getParameters(argc,argv);
    err = inet_pton(AF_INET, ip_string, &ip_to_num);
    if (err < 0)
    {
        cout << "Error inet_pton" << endl;
        return 1;
    }
    

    int ClientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSock < 0)
    {
        cout << "Error initialization socket" << endl;
        return 1;
    }
    else
        cout << "Client socket initialization is OK" << endl;

    struct sockaddr_in servAddr;
    bzero((char *)&servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr = ip_to_num;

    cout << "number - " << number << endl;
    cout << "server ip - " << inet_ntoa(servAddr.sin_addr) << endl;
    cout << "server port - " << ntohs(servAddr.sin_port) << endl;

    unsigned int length = sizeof(servAddr);
    err = connect(ClientSock, (struct sockaddr *)&servAddr, length);
    if (err < 0)
    {
        cout << "Error connect" << endl;
        return 1;
    }
    else
    {
        cout << "Connect is OK" << endl;
    }

    cout<<endl;

    int t=0;
    while (1)
    {
        if(t==10){
            cout<<"End"<<endl;
            close(ClientSock);
            return 0;
        }
        t++;
        sleep(number);
        length = sizeof(servAddr);
        int n = send(ClientSock, mes, strlen(argv[1]), 0);
        if (n < 0)
            cout << "Cant send" << endl;
        n = recv(ClientSock, (char *)buf, BUFLEN, 0);
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
