//============================================================================
// Name        : Client.cpp
// Author      : Viktor Cerny
// Version     :
// Copyright   : Your copyright notice
// Description : TCP client - demo
//============================================================================

#include <iostream>
using namespace std;

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <netdb.h>
#include <string.h>


int main(int argc, char **argv) {
    if(argc < 3){
        cerr << "Usage: client address port" << endl;
        return -1;
    }

    int port = atoi(argv[2]);
    if(port == 0){
        cerr << "Usage: client address port2" << endl;
        return -1;
    }
    // Vytvoreni koncoveho bodu spojeni
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        perror("Nemohu vytvorit socket: ");
        return -1;
    }

    socklen_t sockAddrSize;
    sockAddrSize = sizeof(struct sockaddr_in);

    struct sockaddr_in serverAddr;
    bzero((char *) &serverAddr, sockAddrSize);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    struct hostent *host;
    host = gethostbyname(argv[1]); // <netdb.h>
    memcpy(&serverAddr.sin_addr, host->h_addr,
           host->h_length); // <string.h>
    std :: cout << "Host : " << host->h_addr_list[0] << "\n";
    // Pripojeni ke vzdalenemu serveru
    if(connect(s, (struct sockaddr *) &serverAddr, sockAddrSize) < 0){
        perror("Nemohu spojit!");
        close(s);
        return -1;
    }

#define BUFFER_SIZE 10240
    char buffer[BUFFER_SIZE];
    char bufferRead[BUFFER_SIZE];
    int pid = fork();
    while(true)
    {
       
       if(pid == 0)
            cout << "> ";
            cin.getline (buffer, BUFFER_SIZE - 1);
            if(send(s, buffer, strlen(buffer), MSG_NOSIGNAL) < 0){
                perror("Nemohu odeslat data:");
                close(s);
                return -3;
        }
        else
        {

          int r = recv(s, bufferRead, BUFFER_SIZE - 1, 0);
          bufferRead[r] = '\0';
          std :: cout <<  ">" << bufferRead << "<";
          if(r <= 0)
          {
              close(s);
              return -2;
          }
          std :: cout << "\n";
        }
//        return 0;
        // Kdyz poslu "konec", ukoncim spojim se serverem
        if(string("konec") == buffer){
            break;
        }
    }
    int status = 0;
    waitpid(0, &status, 0 );

    close(s);
    return 0;
}
