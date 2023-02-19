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
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#define BUFLEN 1024

using namespace std;

void reaper(int sig){
   pid_t pid;
   int status;
   while((pid = wait3(&status, WNOHANG, NULL)) > 0);
}

int main()
{
   struct sockaddr_in servAddr, clientAddr;
   char buf[BUFLEN];
   int pid = 0;
   int ClientSock = 0;
   in_addr ip_to_num;
   int err = 0;

   signal(SIGCHLD, reaper);

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

   while (1)
   {
      length = sizeof(clientAddr);
      ClientSock = accept(ServSock, (sockaddr *)&clientAddr, &length);
      if (ClientSock < 0)
      {
         cout << endl << "Error accept" << endl;
      }
      else
      {
         cout << endl << "New client" << endl;
         pid = fork();
         if (pid == 0)
            break;
      }
   }

   cout<<endl;
   close(ServSock);

   while (1)
   {
      length = sizeof(clientAddr);
      bzero(buf, sizeof(BUFLEN));

      int res = recv(ClientSock, buf, BUFLEN, 0);
      if (res < 0)
      {
         cout << "Cant recv" << buf << endl;
      }
      else if (res > 0)
      {
         cout << "Client: " << buf << endl;
         send(ClientSock, buf, strlen(buf), 0);
         cout << "ip - " << inet_ntoa(clientAddr.sin_addr) << endl;
         cout << "port - " << ntohs(clientAddr.sin_port) << endl;
         cout << endl;
      }
      else if (res == 0)
      {
         cout << "End client" << endl;
         close(ClientSock);
         exit(0);
      }
   }
}
