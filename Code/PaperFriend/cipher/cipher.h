#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <iostream>
#include <QCryptographicHash>
using namespace std;

QString str2qstr(const string& str);
string qstr2str(const QString& qstr);
string encode_string(string input, string key);
string decode_string(string input, string key);


#endif // CIPHER_H
