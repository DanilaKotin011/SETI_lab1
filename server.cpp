#include <iostream>
#include <fstream>
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
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#define BUFLEN 1024

using namespace std;

const int MAX_CLIENTS = 2;

int main()
{
   struct sockaddr_in servAddr, clientAddr;
   
   int pid = 0;
   int client_socket[MAX_CLIENTS];
   in_addr ip_to_num;
   int err = 0;
   char buf[BUFLEN];

   for(int i=0;i<MAX_CLIENTS;i++){
    client_socket[i]=0;
   }

   err = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
   if (err < 0)
   {
      cout << "Error inet_pton" << endl;
      return 1;
   }

   int ServSock = socket(AF_INET, SOCK_STREAM, 0);
   if (ServSock < 0)
   {
      cout << "Error initialization socket" << endl;
      return 1;
   }
   else
      cout << "Server socket initialization is OK" << endl;

   bzero((char *)&servAddr, sizeof(servAddr));
   bzero((char *)&clientAddr, sizeof(clientAddr));
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr = ip_to_num;
   servAddr.sin_port = 0;

   if (bind(ServSock, (sockaddr *)&servAddr, sizeof(servAddr)))
   {
      cout << "Error bind socket" << endl;
      return 1;
   }
   else
      cout << "Bind os OK" << endl;

   unsigned int length = sizeof(servAddr);
   getsockname(ServSock, (sockaddr *)&servAddr, &length);

   cout << "Server:" << endl;
   cout << "ip - " << inet_ntoa(servAddr.sin_addr) << endl;
   cout << "port - " << ntohs(servAddr.sin_port) << endl;

   err = listen(ServSock, 5);
   if (err < 0)
   {
      cout << "Error listen" << endl;
      return 1;
   }
   else
   {
      cout << "Listen is OK" << endl;
   }
   
  fd_set readfds;
  int max_sd, sd, new_client, readsize;
  length = sizeof(clientAddr);
   while (1)
   {
        FD_ZERO(&readfds);
        FD_SET(ServSock, &readfds);
        max_sd = ServSock;
        
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socket[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        if(select(max_sd + 1, &readfds, NULL, NULL, NULL)<0){
            cout << endl << "Error select" << endl;
        }

        if (FD_ISSET(ServSock, &readfds)) {
            if ((new_client= accept(ServSock, (struct sockaddr *)&clientAddr, (socklen_t*)&length )) < 0) {
                cout << endl << "Error accept" << endl;
            }
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_client , inet_ntoa(clientAddr.sin_addr) , ntohs(clientAddr.sin_port));
         
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_client;
                    break;
                }
            }
        }
      for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                if ((readsize = read(sd, buf, 1024)) == 0) {
                    cout << endl << "Disconnect" << endl;
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    buf[readsize] = '\0';
                    cout << endl << "Client: " << buf << endl;
                    send(sd, buf, strlen(buf), 0);
                }
            }
        }
   }
   cout<<endl;
   
}
