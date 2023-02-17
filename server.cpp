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
#define BUFLEN 81

using namespace std;

int main(){
   int ServSock = socket(AF_INET, SOCK_STREAM, 0);
   if (ServSock < 0) {
		cout << "Error initialization socket"<< endl; 
		return 1;
   }
   else cout << "Server socket initialization is OK" << endl;
   
   struct sockaddr_in servAddr, clientAddr ;
   char buf[BUFLEN] ;
   bzero( (char *) &servAddr, sizeof( servAddr ) );
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr.s_addr = htonl( INADDR_ANY );
   servAddr.sin_port = 0;
   
   if (bind( ServSock, (sockaddr*)&servAddr, sizeof(servAddr)))
   {
	cout<< "Error bind socket"<< endl;
	return 1;
   }
   else cout<< "Bind os OK"<< endl;
   int length = sizeof(servAddr);
   
   getsockname( ServSock, (sockaddr*)&servAddr, sizeof(servAddr));
   
   cout<<"Server:"<<endl;
   cout<<"port - " << ntohs(servAddr.sin_port ) << endl;
   cout<<"addres - " << servAddr.sin_addr.s_addr << endl;
   
   while(1){
   	length = sizeof( clientAddr ) ;
	bzero( buf, sizeof( BUFLEN) );
	
   	if(recv(ServSock,buf,BUFLEN,0)>=0){
   		cout<<"Mes - "<<buf<<endl;
   	}
   	
   
   }
   
   


}
