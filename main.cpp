#include <iostream>
#include "OptionParser.h"

using namespace std;

int main()
{
    string str1 = "abc";
    string str2 = "abc";
    string str3 = "abc";
    OptionParser opr("random_arg", "Initial random arg string with ctor");
    opr.displayOptions();

    opr.add(str1, str2, str3);
    opr.add((const string)"arg1", (const string)"abc", (const string)"first arg");
//    opr.add("arg1", "abc", "first arg");
//    opr.add("arg2", 3 , "second arg");
//    opr.add("arg3", false, "third arg");

    cout << endl << "After adding args " << endl;
    opr.displayOptions();

    cout << endl << "**** END *** " << endl << endl;
    return 0;
}

