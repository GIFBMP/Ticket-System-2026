#pragma once
#ifndef LEXER_H
#define LEXER_H
#include <string>
#include "vector.h"
namespace sjtu {
    using std::string;
    long long stringtoint(const string &);
    vector<string> decompose(const string &); //command->pieces
    vector<string> split(const string &); // a|b|c -> (a, b, c)
}
#endif