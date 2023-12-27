#ifndef BIGINT_H
#define BIGINT_H
#include <string>
#include <vector>

// Note: it is bad practice to have `using namespaces std;` in 
//       a header file.  So where we use C++ objects (cout, cin) 
//       and C++ class types (string, vector<int>), you should
//       precede them with `std::`.  

// In the .cpp implementation file, it is fine to have a 
//  `using namespace std;` statement. So you can put that in the
//  .cpp file and then avoid having to precede everything with `std::`

class BigInt {
public:
    BigInt(std::string s, int base=10); // convert string to BigInt
    std::string to_string() const; // get string representation
    void add(BigInt b); // add another BigInt to this one
    BigInt operator+(const BigInt& rhs) const; 
    bool operator<(const BigInt& rhs) const;
    void sub(BigInt b);
    BigInt operator-(const BigInt& rhs) const;
private:
    // whatever you need
    std::vector<int> vec;
    int numBase;
     void removeLeadingZeroes();
};

#endif