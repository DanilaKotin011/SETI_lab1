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

pthread_mutex_t st_mutex;
ofstream file("Out.txt");

struct hand_atr{
   int ServSock;
   int ClientSock;
};

void *handler(void *atrr){
   hand_atr atr;
   atr = *((hand_atr*) atrr);

   char buf[BUFLEN];
   unsigned int length;

   while (1)
   {
      bzero(buf, sizeof(BUFLEN));

      int res = recv(atr.ClientSock, buf, BUFLEN, 0);
      if (res < 0)
      {
         cout << "Cant recv" << buf << endl;
      }
      else if (res > 0)
      {
         cout << "Client: " << buf << endl;
         pthread_mutex_lock(&st_mutex);
         file << buf << endl;
         pthread_mutex_unlock(&st_mutex);
         send(atr.ClientSock, buf, strlen(buf), 0);
         cout << endl;
      }
      else if (res == 0)
      {
         cout << "End client" << endl << endl;
         close(atr.ClientSock);
         exit(0);
      }
   }
}

int main()
{
   struct sockaddr_in servAddr, clientAddr;
   
   int pid = 0;
   int ClientSock = 0;
   in_addr ip_to_num;
   int err = 0;

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

   pthread_t th;
   pthread_attr_t ta;
   pthread_attr_init(&ta);
   pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
   pthread_mutex_init(&st_mutex,0);
   
  

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
         hand_atr atr;
         atr.ServSock=ServSock;
         atr.ClientSock=ClientSock;
     
         if (pthread_create( &th, &ta, handler, (void *) &atr) < 0 )
         { cout << endl << "Error pthread_create" << endl;}

      }
   }

   file.close();
   cout<<endl;
   
}
