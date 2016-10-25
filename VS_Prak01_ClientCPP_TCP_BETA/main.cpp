#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <ctime>
#include <time.h>
#include <algorithm>
#define PORT 12345
#define BUFSIZE 1024
using namespace std;

int main()
{
    int socket_client;
    char buffer[BUFSIZE];
    int result, i=0;
    
    
    memset(buffer, 0, BUFSIZE);

    socket_client = socket(AF_INET, SOCK_STREAM, 0);

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    if (socket_client < 0){
        cout << "...FEHLER beim Verbindungsaufbau..." << endl;
        exit(1);
    }
    cout << "...Socket Client wurde erstellt..." << endl;
    
    //Adresse des Servers
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // ggfs aendern! Port des Servers
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ggfs Aendern! IP Adresse des Servers, z.B. "localhost"
    

    /* ---------- CONNECTING THE SOCKET ---------- */
    if (connect(socket_client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "...Verbindung zur Server Portnummer: " << PORT << endl;

    cout << "...Warte auf Bestaetigung vom Server..." << endl;
    //recv(socket_client, buffer, BUFSIZE, 0);
    //cout << buffer << endl;
    cout << "...Verbindung wurde bestaetigt, Daten koennen uebertragen werden..." << endl;
    //recv(socket_client, buffer, BUFSIZE, 0);

    
    
    
    
    memset(buffer, 0, BUFSIZE);
    // Alle 5 Anfragen in einem send() schicken
    int length = 100;
    
    strncpy(buffer,"100", 3);
    cout << "sendpackage: " << buffer <<endl; 
    send(socket_client, buffer, sizeof(length), 0);
    memset(buffer, 0, BUFSIZE);
    
    string sendingbuffer = "";
    for(int i=0; i < 5; ++i){
        sendingbuffer+= "time%";
        
    } 

    
    const char *cstr = sendingbuffer.c_str();    
    strncpy(buffer, cstr, 50);
    cout << "Sende Nachricht: " << buffer << endl;
    
    string send_message;
    cout << "Nachricht jetzt senden?: ";
    //cin >> send_message;
    
    time_t now = time(NULL);  //Zeit beim Senden
    string timeStamp = ctime(&now);      
    replace(timeStamp.begin(), timeStamp.end(), '\n', ' '); 
    
    
    
    send(socket_client, buffer, BUFSIZE, 0); //Senden 5 mal time
    
    do{
        result = recv(socket_client, buffer, BUFSIZE, 0);
        if(result > 0){
            cout << "[" << timeStamp.substr(11, 8) << "] Antwort " << (++i) 
             << ": " << buffer << endl;  
        }else if(result == 0){
            cout << "---Verbindung wurde geschlossen---" << endl;
        }else{
            cout << "---recv Fehler---" << endl;
            break;
        }
    }while(result > 0);
    
    
    // 5 Anfragen einzelnt schicken
    /*
    for(int i=0; i < 5; ++i){
        strcpy(buffer, "time");
        time_t now = time(NULL);
        string timeStamp = ctime(&now);      
        replace(timeStamp.begin(), timeStamp.end(), '\n', ' '); 
        
        send(socket_client, buffer, BUFSIZE, 0);   
        recv(socket_client, buffer, BUFSIZE, 0);
   
        cout << "[" << timeStamp.substr(11, 8) << "] Antwort " << (i+1) 
             << ": " << buffer << endl; 
    }
    */
    close(socket_client);
    return 0;
}

