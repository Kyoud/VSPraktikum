#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <ctime>
#include <algorithm>
#define PORT 12345
#define BUFSIZE 1024
#define MAX_SOCKETS 20
#define MAX 10

using namespace std;

int main()
{
    int socket_client, socket_server, result;
    char buffer[BUFSIZE];
    char sendbuffer[BUFSIZE];
    socklen_t size;
    
    
    memset(buffer, 0, BUFSIZE);

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    socket_client = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_client < 0){
        cout << "---FEHLER bei TCP Verbindungsaufbau---" << endl;
        return -1;
    }
    cout << "---Socked Server wurde erstellt---" << endl;

    //Adresse des Servers
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(PORT); // ggfs aendern! Port des Servers

    /* ---------- BINDING THE SOCKET ---------- */
    //Bindet Socket an eine Adresse IP und Port
    if ((bind(socket_client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0){
        cout << "\n---FEHLER binding connection, Socket wurde schon gebunden---" << endl;
        return -1;
    }

    size = sizeof(server_addr);
    cout << "\n---Suche nach Client---" << endl;

    /* ------------- LISTENING CALL ------------- */
    listen(socket_client, 1);
    /* ------------- ACCEPT CALL -----------------*/
    socket_server = accept(socket_client,(struct sockaddr *)&server_addr,&size);
    if (socket_server < 0) 
        cout << "\n---FEHLER bei accept---" << endl;

    //strcpy(buffer, "\n...Server verbunden...\n");
    //send(socket_server, buffer, BUFSIZE, 0);
    cout << "\n---Verbindung mit Client Bestaetigt, Daten koennen uebertragen werden---" << endl;
    
    
    
    // mehrere Befehle aus einem Packet lesen und einzelnt beantworten
    
    //char sendbuffer[BUFSIZE];
    //while(1){
        time_t now = time (NULL);   
        string timeStamp = ctime(&now);
        replace(timeStamp.begin(), timeStamp.end(), '\n', ' ');
        
        memset(buffer, 0, BUFSIZE);
        result = recv(socket_server, buffer, BUFSIZE, 0);
        if(result == 0){
            cout << "---Verbindung wurde geschlossen---" << endl;
        }else if(result < 0){
            cout << "---recv Fehler---" << endl;
            //break;
        }
        cout << "[" << timeStamp.substr(11, 8) << "] Anfrage von " << inet_ntoa(server_addr.sin_addr) 
             << ", Port" << PORT << ". Empfangene Nachricht: " << buffer << endl;
        
        char *p = strtok(buffer, "%");
        while(p != NULL){
            if(memcmp(p, "time", sizeof(p))){
                string antwort = "tood und verderben";
                memset(sendbuffer, 0, BUFSIZE);
                
                time_t now_send = time (NULL); 
                timeStamp = ctime(&now_send);
                replace(timeStamp.begin(), timeStamp.end(), '\n', ' ');
            
                antwort = timeStamp.substr(11, 8);
                const char *cstr = antwort.c_str();    
                strncpy(sendbuffer,cstr,8);
                
                //cout << "sende uhrzeit..." << endl;
                send(socket_server, sendbuffer, BUFSIZE, 0);
            } 
            p = strtok(NULL, "%");
            
            string nextPackage;
            cout << "send next package: ";
            cin >> nextPackage;
            if(nextPackage == "e")
                break;
        }
    //}
     
    
    //Packete einzelnt bekommen
    /*
    while (1){
        time_t now = time (NULL);   
        string timeStamp = ctime(&now);
        replace(timeStamp.begin(), timeStamp.end(), '\n', ' ');

        
        recv(socket_server, buffer, BUFSIZE, 0);
        
        cout << "[" << timeStamp.substr(11, 8) << "] Anfrage von " << inet_ntoa(server_addr.sin_addr) 
             << ", Port" << PORT << " Nachricht: " << buffer << endl;

        string antwort = "tod und verderben";
        if(buffer[0] == 't'){
            memset(buffer, 0, BUFSIZE);
            
            time_t now_send = time (NULL); 
            timeStamp = ctime(&now_send);
            replace(timeStamp.begin(), timeStamp.end(), '\n', ' ');
            
            antwort = timeStamp.substr(11, 8);
            const char *cstr = antwort.c_str();    
            strncpy(buffer,cstr,8);
        }else{
            break;
        }
        //cout << "Server schickt buffer: " << buffer << endl;
        send(socket_server, buffer, BUFSIZE, 0);
        
        
        string nextPackage;
        cout << "next package: ";
        cin >> nextPackage;
        if(nextPackage == "e")
            break;
    }  
    */
        
    
    
    close(socket_server);
    close(socket_client);
    return 0;
}
