#include <qpid/types/Variant.h>
#include <iostream>

using namespace std;
int main(int argc, char** argv) {

  cout << endl;
//  cout << "VAR_VOID = " << qpid::types::getTypeName(qpid::types::VAR_VOID) << endl;;

  qpid::types::Variant var1(true);
  cout << "var1 type = " << qpid::types::getTypeName(var1.getType()) << endl;

  qpid::types::Variant var2(1L);
  cout << "var2 type = " << var2.getType() << endl;

  qpid::types::Variant var3(2.5);
  cout << "var3 type = " << var3.getType() << endl;

  qpid::types::Variant var4("3.5d");
  cout << "var4 type = " << var4.getType() << endl;

  qpid::types::Variant var5("aString");
  cout << "var5 type = " << var5.getType() << endl;
  cout << "var 5 = " << var5 << endl;

  qpid::types::Variant::Map aMap;
  aMap.insert(pair<string, int>("one", 1));
  aMap.insert(pair<string, int>("two", 2));
  qpid::types::Variant var6(aMap);
  cout << "var6 type = " << var6.getType() << endl;
  cout << "var6 = " << var6 << endl;

  qpid::types::Variant::List aList;
  aList.push_back('a');
  aList.push_back('b');
  qpid::types::Variant var7(aList);
  cout << "var7 type = " << var7.getType() << endl;
  cout << "var7 = " << var7 << endl;

  qpid::types::Variant var8(var7);
  cout << "var8 type = " << var8.getType() << endl;


  var6.reset();
  cout << "var6 type = " << var6.getType() << endl;
  var8.reset();
  cout << "var8 type = " << var8.getType() << endl;

  cout << endl << endl;
  return 0;
}


