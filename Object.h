//
// Created by Arman Sadeghi on 2/16/22.
//

/**
 * An object class for a simple interpreter.
 * A quick-and-dirty Any implementation.
 *
 * (c) 2020 by Ronald Mak
 * Department of Computer Science
 * San Jose State University
 */

#ifndef PASCALPROJECT_OBJECT_H
#define PASCALPROJECT_OBJECT_H
#include <string>
#include <algorithm>
using namespace std;

class Object
{
public:
    long   L;
    double D;
    string S;
    bool   B;

    Object()             : L(0),     D(0.0),   S(""),    B(false) {}
    Object(long value)   : L(value), D(0.0),   S(""),    B(false) {}
    Object(double value) : L(0),     D(value), S(""),    B(false) {}
    Object(string value) : L(0),     D(0.0),   S(value), B(false) {}
    Object(bool value)   : L(0),     D(0.0),   S(""),    B(value) {}
};

/**
 * Return a copy of a string with all the letters shifted to upper case.
 * @param str the original string.
 * @return a copy of the string with shifted letters.
 */
inline string toUpperCase(string str)
{
    string upper_case(str);
    transform(upper_case.begin(), upper_case.end(),
              upper_case.begin(), ::toupper);
    return upper_case;
}

/**
 * Return a copy of a string with all the letters shifted to lower case.
 * @param str the original string.
 * @return a copy of the string with shifted letters.
 */
inline string toLowerCase(string str)
{
    string lower_case(str);
    transform(lower_case.begin(), lower_case.end(),
              lower_case.begin(), ::tolower);
    return lower_case;
}

#endif //PASCALPROJECT_OBJECT_H
