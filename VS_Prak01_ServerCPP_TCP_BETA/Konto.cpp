/* 
 * File:   Konto.cpp
 * Author: debian
 * 
 * Created on 6. November 2016, 20:01
 */

#include <mutex>


#include "Konto.h"



Konto::Konto(string nr, long anfangsaldo) {
    kontonr =nr;
    saldo = anfangsaldo;
}

void Konto::SetSaldo(long saldo) {
    this->saldo = saldo;
}

long Konto::GetSaldo() const {
    return saldo;
}

void Konto::SetKontonr(string kontonr) {
    this->kontonr = kontonr;
}

string Konto::GetKontonr() const {
    return kontonr;
}

long Konto::changesaldo(long change){
    //mtx.lock();
    saldo = saldo+change;
    //mtx.unlock();
    return saldo;
}

string Konto::generatepage(){
    return("HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n"
            "<h1>Hollenbeck Desaster and Destruction Banking</h1>"
            "<h2>Welcome in your Konto</h2>"
            "<p>Kontonr: "+kontonr+"</p><br>"
            "<p>saldo: "+to_string(saldo)+"</p><br>"
            "<form action=\"User\">"
            "<input type=\"hidden\" name=\"UserBrowser\" value=\""+kontonr+"\">"
            "Betrag: <input type=\"text\" name=\"betrag\"><br>"
            "<input type=\"submit\" value=\"Submit\"><br>");
}

