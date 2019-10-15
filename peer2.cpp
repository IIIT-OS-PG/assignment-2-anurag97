#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<iostream>
using namespace std;
#define buffersize 10240
int threadcount[100] ;
int threadno;
int SERVERPORT ;
int CPORT;
char buffer[buffersize];
string inputcommand;

string fname;

void* serverfunc(void* servptr){
    int connfd  = *((int*)servptr);
    string ch = "_";
    string txt =".txt";
    fname +=ch+txt;
    FILE *fileptr = fopen(fname.c_str(),"w");
    int n=0;
    int filesize=0;
    char buffer[buffersize];
    
    //recv(connfd , &filesize, sizeof(filesize),0);

    while((n = recv(connfd , buffer,buffersize,0))>0){
        fwrite(buffer, sizeof(char),n,fileptr);
        memset(buffer,'\0',buffersize);
        filesize = filesize-n;
    }
    close(connfd);
    fclose(fileptr);
}

void* clientfunc(void* pclient){
    
    int connfd = *((int*)pclient);
    string s;
    cout<<"ENter filename:\n";
    cin>>s;
    FILE *fileptr = fopen(s.c_str(), "rb");
   // fseek(fileptr,0 ,SEEK_END);
    //int currpos = ftell(fileptr);
    
    //send(connfd , &currpos, sizeof(currpos),0);
     int bytes=0;

    char buffer[buffersize];
    while((bytes = fread(buffer, sizeof(char), buffersize, fileptr))>0){
        send(connfd, buffer,bytes,0);
        memset(buffer,'\0',buffersize);
        //currpos = currpos -bytes;

    }
    cout<<"File sent complete\n";
    fclose(fileptr);

}

void* clienthandle( void *pclient){
    sleep(10);
/*    int connfd=0;
    struct sockaddr_in server_address;

    if((connfd= socket(AF_INET,SOCK_STREAM,0))<0){
      perror("Error connecting tracker!!\n");
      exit(EXIT_FAILURE);
    }

    memset(&server_address,'0',sizeof(server_address));
    server_address.sin_family = AF_INET;
    cout<<"Please enter the Server port to which you wnt to connect your client:\n";
    int portno =0;
    cin>>portno;
    server_address.sin_port =htons(portno);
  
    
 if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<0)
    {
        printf("\n inet_pton error occured\n");
        exit(EXIT_FAILURE);
    }   
  

    if(connect(connfd,(struct sockaddr*)&server_address,sizeof(server_address))<0){
    
    cout<<"Connection failed!!"<<endl;
    exit(EXIT_FAILURE);
}*/

while(true)
{   int connfd=0;
    struct sockaddr_in server_address;

    if((connfd= socket(AF_INET,SOCK_STREAM,0))<0){
      perror("Error connecting tracker!!\n");
      exit(EXIT_FAILURE);
    }

    memset(&server_address,'0',sizeof(server_address));
 if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<0)
    {
        printf("\n inet_pton error occured\n");
        exit(EXIT_FAILURE);
    }   
  
    server_address.sin_family = AF_INET;
    cout<<"Please enter the Server port to which you wnt to connect your client:\n";
    int portno =0;
    cin>>portno;
    server_address.sin_port =htons(portno);
  
    
 if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<0)
    {
        printf("\n inet_pton error occured\n");
        exit(EXIT_FAILURE);
    }   
  

    if(connect(connfd,(struct sockaddr*)&server_address,sizeof(server_address))<0){
    
    cout<<"Connection failed!!"<<endl;
    exit(EXIT_FAILURE);
}


    pthread_t t_id;
    pthread_create(&t_id,NULL, &clientfunc, &connfd);

    pthread_join(t_id,NULL);
    cout<<"Closed the client thread!"<<endl;
    close(connfd);

   
} 

//close(connfd);
return NULL;
}


void* serverhandle( void *pserver){
    int listenfd =0;
    int connfd =0;
    struct sockaddr_in server_address;
    if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("server thread socket not connected!!\n");
        return NULL;
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port= htons(SERVERPORT);
    int addrlen = sizeof(sockaddr);

    if(bind(listenfd,(struct sockaddr *)&server_address,sizeof(server_address))<0){
        perror("SERVER THREAD bind failed!");
        exit(EXIT_FAILURE);
    }
    if(listen(listenfd,10)<0){
        perror("afsoos...LISTEN nahi ho saka!!");
        exit(EXIT_FAILURE);
    }
   else 
   cout<<"Server thread running....."<<endl;

   while(1)
	{

	connfd = accept ( listenfd, (struct sockaddr *)&server_address , (socklen_t*)&addrlen);

	cout<<"\nsocket port no - "<<server_address.sin_port;

	cout<<"\nConnection Established";

	pthread_t thread_id;
	pthread_create(&thread_id,NULL,&serverfunc,&connfd);

	pthread_join(thread_id,NULL);

	cout<<"\nServer thread ending!!!";

	}
   
   close(listenfd);
   return NULL;
   }




int main(int argc, char *argv[]){
pthread_t client,server;
    SERVERPORT = atoi(argv[1]);
    CPORT = atoi(argv[2]);
    
	 pthread_create(&server,NULL,&serverhandle,NULL);
     
     pthread_create(&client,NULL,&clienthandle,NULL);
     


     pthread_join(server,NULL);
     pthread_join(client,NULL);
  
return 0;
}


    
