#include<unistd.h>
#include<bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<pthread.h>
#include <cstdlib>
#include<fstream>
using namespace std;

#define buffersize 1024
#define BACKLOG 10
int TRACKERPORT;
string trackerinfopath;
string inputcommand;
int connfdlogin[200];

int i=0;
string userdata[100][5] ={ };


string runcommand(string inputcommand){
    string out;
   
    vector<string> tokens; 
    stringstream check1(inputcommand); 
      
    string intermediate; 
    while(getline(check1, intermediate, ' ')) 
    { 
        tokens.push_back(intermediate); 
    } 
      

if(tokens[0]=="create_user"){


}
else if(tokens[0]=="login"){
    
}

else if(tokens[0]=="upload_file"){

}
else if(tokens[0]=="download_file"){

}
else if(tokens[0]=="logout"){


}
else if(tokens[0]=="show_downloads"){


}
else if(tokens[0]=="stop_share"){
}
else if(tokens[0]=="quit"){
    out="quit";
}
else{
 out ="Wrong command!! please submit valid commmand\n";

}

return out;
}



int searchportoftno(int trackerno){
    string temp;
    ifstream read;
    int x=0;

    read.open(trackerinfopath);

    int lineno=0;
    while(lineno!=trackerno && getline(read,temp)){
    ++lineno;
    }
    
    if(lineno==trackerno){
        
      sscanf(temp.c_str(), "%d", &x);
      
    }
    else {
        cout<<"Wrong tracker number...Exiting!!!";
        exit(0);
    }
    return x;
}


//////////////////////////////////////////////////////////////////////////////////////
void *handle(void *pclient){
    int connfd = *((int*)pclient);
    free(pclient);
while(true){
    string message;
     char inputcommand[buffersize] ;
     memset(inputcommand,'\0',strlen(inputcommand));
    int valread = recv( connfd , inputcommand, 1024,0); 
    printf("%s\n",inputcommand ); 
    string out= runcommand(inputcommand);
    // string out = "hello re!!\n";
    if(out=="quit") {close(connfd); return NULL;}
    else
    send(connfd, out.c_str() , strlen(out.c_str()) , 0 ); 
}
  }
/////////////////////////////////////////////////////////////////////////////////////





int main(int argc, char* argv[]){
    int listenfd =0;
    int connfd =0;
    int readfd =0;
    char sendbuffer[buffersize]={0};
    char recvbuffer[buffersize+1]={0};
    trackerinfopath = argv[1];
    int trackerno = atoi(argv[2]);

    TRACKERPORT= searchportoftno(trackerno);

    struct sockaddr_in server_address;

    if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket not connected!!\n");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port= htons(TRACKERPORT);

    if(bind(listenfd,(struct sockaddr *)&server_address,sizeof(server_address))<0){
        perror("bind failed!");
        exit(EXIT_FAILURE);
    }

    if(listen(listenfd,BACKLOG)<0){
        perror("afsoos...LISTEN nahi ho saka!!");
        exit(EXIT_FAILURE);
    }

    for( ; ; ){
        
        struct sockaddr_in addr2;
        socklen_t a2len;
        char client_address[buffersize+1];

        printf("WAITING FOR CONNECTION OF PORT %d\n",TRACKERPORT);
        {
        if((connfd = accept(listenfd,(struct sockaddr *)&addr2,&a2len))<0){
            perror("Connection refused!!");
            return -1;
        }
        else cout<<connfd<<endl;

        //handle(connfd);
        pthread_t th;
        int *pclient = (int *)malloc(sizeof(int));
        *pclient = connfd;
        pthread_create(&th,NULL,handle,pclient);
        }

    }
    close(listenfd);
    return 0;
}
    
// Created by anurag on 05/10/19.


