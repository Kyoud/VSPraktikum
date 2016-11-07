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
#include <thread>
#include <map>
#include <mutex>

#include "Konto.h"

#define PORT 12345
#define BUFSIZE 1024
#define MAX_SOCKETS 20
#define MAX 10

using namespace std;

std::mutex mtx;

void talk(int socket_server, int socket_client, map<string, Konto> konten) {
    char buffer[BUFSIZE];
    char sendbuffer[BUFSIZE];
    int result, sendbytes;
    bool zahlung=false;
    string betrag="";
    string page = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n"
            " <h1>Hollenbeck Desaster and Destruction Banking</h1><p align=\"left\"><font color=\"#000000\">Bitte geben sie ihre Kontonr ein"
            "</font></p>"
            "<p> <form action=\"User\">"
            "kontonr: <input type=\"text\" name=\"kontonr\"><br>"
            "<input type=\"submit\" value=\"Submit\">"
            "</form> </p>";

    while (1) {


        //strcpy(buffer, "\n...Server verbunden...\n");
        //send(socket_server, buffer, BUFSIZE, 0);
        memset(buffer, 0, BUFSIZE);
        cout << "\n---Verbindung mit Client Bestaetigt, Daten koennen uebertragen werden---" << endl;



        // mehrere Befehle aus einem Packet lesen und einzelnt beantworten

      
        result = recv(socket_server, buffer, BUFSIZE, 0);
        if (result == 0) {
            cout << "---Verbindung wurde geschlossen---" << endl;
            //cout << "warte auf neue verbindung" <<endl;
            //socket_server = accept(socket_client,(struct sockaddr *)&server_addr,&size);
            //if (socket_server < 0) 
            //cout << "\n---FEHLER bei accept---" << endl;
            //close(socket_server);
        } else if (result < 0) {
            cout << "---recv Fehler---" << endl;
            return;
            //break;
        }

        cout << "Anfrage von "
                << ", Port" << PORT << ". Empfangene Nachricht: " << buffer << endl; /*<< inet_ntoa(server_addr.sin_addr)*/
        //char *p = strtok(buffer, "%");
        string http = buffer;
        int tmp1 = http.find("/");
        int tmp2 = http.find(" ", http.find("/"))-(tmp1);
        string get = http.substr(tmp1, tmp2);
        cout << "Get message: " << get << " testnr: " << endl;
        if (tmp2 > 1) {
            int gettmp1= get.find("=");
            int gettmp2;
            if(get.find("&")==-1)
                gettmp2 = get.length()-(gettmp1+1);
            else{
                gettmp2 =get.find("&")-(get.find("=")+1);
                betrag = get.substr(get.find("=",get.find("=")+1)+1,get.length()-get.find("=",get.find("=")+1));
                cout << "betrag: " << betrag << endl;
                zahlung=true;
                
            }
            string kontonummer = get.substr(gettmp1+1,gettmp2);
            cout << "Get message: " << kontonummer << " testnr: " << endl;
            if (konten.find(kontonummer) == konten.end()) {
                
                page = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n"
                        " <h1>Hollenbeck Desaster and Destruction Banking</h1><p align=\"left\"><font color=\"#000000\">Bitte geben sie ihre Kontonr ein"
                        "</font></p>"
                        "<p> <form action=\"User\">"
                        "kontonr: <input type=\"text\" name=\"kontonr\"><br>"
                        "<input type=\"submit\" value=\"Submit\"><br>"
                        "<p align=\"left\"><font color=\"#FF0000\">Kontonrexistiertnicht</font></p>"
                        "</form> </p>";
            } else {
                if (zahlung)
                {
                    mtx.lock();
                    konten.at(kontonummer).changesaldo(stol(betrag));
                    mtx.unlock();
                }
                page = konten.at(kontonummer).generatepage();
            }
        }

        const char *cstr = page.c_str();
        strncpy(sendbuffer, cstr, BUFSIZE);
        sendbytes = send(socket_server, sendbuffer, sizeof (sendbuffer), 0);
        if (sendbytes == -1)
            cout << "send failed";
        //close(socket_client);



        //return 0;
    }

}

int main() {
    int socket_client, socket_server, length, recived;


    socklen_t size;
    map<string, Konto> konten;
    Konto k1 = Konto("123", 100);
    Konto k2 = Konto("124", 100);
    Konto k3 = Konto("125", 100);
    konten.insert(make_pair("123", k1));
    konten.insert(make_pair("124", k2));
    konten.insert(make_pair("125", k3));

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    socket_client = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_client < 0) {
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
    if ((bind(socket_client, (struct sockaddr*) &server_addr, sizeof (server_addr))) < 0) {
        cout << "\n---FEHLER binding connection, Socket wurde schon gebunden---" << endl;
        return -1;
    }
    while (1) {
        size = sizeof (server_addr);
        cout << "\n---Suche nach Client---" << endl;

        /* ------------- LISTENING CALL ------------- */
        listen(socket_client, 1);
        /* ------------- ACCEPT CALL -----------------*/
        socket_server = accept(socket_client, (struct sockaddr *) &server_addr, &size);
        if (socket_server < 0)
            cout << "\n---FEHLER bei accept---" << endl;

        std::thread first(talk, socket_server, socket_client, konten);
        first.detach();
    }
}
