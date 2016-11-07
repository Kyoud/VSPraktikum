/* 
 * File:   Konto.h
 * Author: debian
 *
 * Created on 6. November 2016, 20:01
 */

#ifndef KONTO_H
#define	KONTO_H
#include <iostream>
#include <string>
//#include <mutex>
using namespace std;
class Konto {
    
public:
    Konto(string, long);    
    long changesaldo(long);
    void SetSaldo(long saldo);
    long GetSaldo() const;
    void SetKontonr(string kontonr);
    string GetKontonr() const;
    string generatepage();
    
private:
    string kontonr;
    long saldo;
    //std::mutex mtx;
};

#endif	/* KONTO_H */

