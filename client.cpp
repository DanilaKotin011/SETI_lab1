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


int main(int argc, char* argv[]){
    char buf[BUFLEN];
    int number;
    int port;
    int ServSock=0;
    in_addr ip_to_num;
    int err=0;
 
    if(argc==1) number = 10;
    else if(argc>2){
        number = stoi(argv[1]);
        port=stoi(argv[2]);
    } 
    if(argc>3){
        err=inet_pton(AF_INET, argv[3], &ip_to_num);
        if(err<0){
            cout<<"Error inet_pton"<<endl;
            return 1;
        }
    }
    
    int ClientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSock < 0) {
		cout << "Error initialization socket"<< endl; 
		return 1;
    }
    else cout << "Client socket initialization is OK" << endl;

    struct sockaddr_in     servAddr;
    bzero( (char *) &servAddr, sizeof( servAddr ) );

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr= ip_to_num;

    cout<<"ip - " << inet_ntoa(servAddr.sin_addr) << endl;
    cout<<"port - " << ntohs(servAddr.sin_port ) << endl;

    unsigned int length = sizeof( servAddr ) ;
    err=connect(ClientSock,(struct sockaddr *)&servAddr, length);
    if(err<0){
        cout<<"Error connect"<<endl;
        return 1;
    }
    else{
        cout<<"Connect is OK"<<endl;
    }

    while(1){
        sleep(number);
        length = sizeof( servAddr ) ;
        int n=send(ClientSock, (const char *)argv[1], strlen(argv[1]),0);
        if(n<0) cout<<"Cant send"<<endl;
        n = recv(ClientSock, (char *)buf, BUFLEN, 0);
        if(n<0) cout<<"Cant recv"<<endl;
        else{
            buf[n] = '\0';
            std::cout<<"Server :"<<buf<<std::endl;
            cout<<"ip - " << inet_ntoa(servAddr.sin_addr) << endl;
            cout<<"port - " << ntohs(servAddr.sin_port ) << endl;
            cout<<endl;
        }
        
    }
    }
   