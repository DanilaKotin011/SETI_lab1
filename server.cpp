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

int main()
{
   struct sockaddr_in servAddr, clientAddr;
   char buf[BUFLEN];
   in_addr ip_to_num;
   int err = 0;
   int ServSock;
   unsigned int length;

   err = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
   if (err < 0)
   {
      cout << "Error inet_pton" << endl;
      return 1;
   }

   ServSock = socket(AF_INET, SOCK_DGRAM, 0);
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

   length = sizeof(servAddr);
   getsockname(ServSock, (sockaddr *)&servAddr, &length);

   cout << "Server:" << endl;
   cout << "ip - " << inet_ntoa(servAddr.sin_addr) << endl;
   cout << "port - " << ntohs(servAddr.sin_port) << endl;

   while (1)
   {
      length = sizeof(clientAddr);
      bzero(buf, sizeof(BUFLEN));

      int res = recvfrom(ServSock, buf, BUFLEN, 0, (struct sockaddr *)&clientAddr, &length);
      if (res < 0)
      {
         cout << "Cant recv" << buf << endl;
      }
      else
      {
         cout << "Client: " << buf << endl;
         sendto(ServSock, buf, strlen(buf), 0, (const struct sockaddr *)&clientAddr, sizeof(clientAddr));
         cout << "ip - " << inet_ntoa(clientAddr.sin_addr) << endl;
         cout << "port - " << ntohs(clientAddr.sin_port) << endl;
         cout << endl;
      }
   }
}
