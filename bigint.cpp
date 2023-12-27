#include <iostream>
#include <math.h> 
#include <string>
#include <vector>
#include "bigint.h"

using namespace std;

// Write your implementation below
BigInt::BigInt(string s, int base)// convert string to BigInt
{
    numBase = base;
    for(int i =  s.size() - 1; i >= 0; i--)
    {
      char c = s[i];

      int x = static_cast<int>(c) - '0';
      if(x > 9)
      {
        x = 10 + static_cast<int>(c) - 'A';
      }
      vec.push_back(x);
    }
    removeLeadingZeroes();
}


std::string BigInt::to_string() const
{
  string s = "";
  for(int i = vec.size() - 1; i >= 0; i--)
  {
    if(vec[i] <= 9)
    {
      char c = (char)(vec[i]+'0');
      s += c;
    }
  }
  return s;
}

void BigInt::add(BigInt b)
{

  int change = vec.size();
  int noCh = b.vec.size();

  if(change > noCh)
  {
    int diff = change - noCh;
    for(int i = 0; i < diff; i++)
    {
      b.vec.push_back(0);
    }
  }
  if(change < noCh)
  {
    int diff = noCh - change;
    for(int i = 0; i < diff; i++)
    {
      vec.push_back(0);
      change++;
    }
  }

  int carry = 0;
  int sum;
  for(int i = 0; i < change; i++)
  {
      sum = vec[i] + b.vec[i] + carry;
      if(sum >= numBase)
      {
          int add = sum % numBase;
          vec[i] = add;
          carry = 1;
      }
      else
      {
        vec[i] = sum;
        carry = 0;
      }
  }
  if(carry == 1)
  {
    vec.push_back(1);
  }
}

void BigInt::removeLeadingZeroes()
{
  int countPop = 0;
  if(vec.size() > 1)
  {
    for(int i = vec.size() - 1; vec[i] == 0 && i > 0; i--)
    {
      countPop++;
    }
    for(int i = 0; i < countPop; i++)
    {
    vec.pop_back();
    }
  }
}

BigInt BigInt::operator+(const BigInt& rhs) const
{
  BigInt temp = *this;
  temp.add(rhs);
  //removeLeadingZeroes();
  return temp;
}

bool BigInt::operator<(const BigInt& rhs) const
{
  //BigInt tempM = *this;
  //BigInt tempR = *rhs;

  if(vec.size() < rhs.vec.size())
  {
    return true;
  }
  else if(vec.size() > rhs.vec.size())
  {
    return false;
  }
  else
  {
    for(int i = vec.size() - 1; i >= 0 ; i--)
    {
      if(vec[i] > rhs.vec[i])
      {
        return false;
      }
      else
      {
        return true;
      }
    }
  }
  return true;
}

void BigInt::sub(BigInt b)
{
  int change = vec.size();
  int noCh = b.vec.size();

  if(change > noCh)
  {
    int diff = change - noCh;
    for(int i = 0; i < diff; i++)
    {
      b.vec.push_back(0);
    }
  }

  if(change == 1)
  {
    int diff = vec[0] - b.vec[0];
    vec[0] = diff;
  }
  else
  {
    for(int i = 0; i < vec.size(); i++)
    {
      if(vec[i] < b.vec[i])
      {
        vec[i + 1] -= 1;
        vec[i] += 10;
        vec[i] = vec[i] - b.vec[i];
      //cout << "vec[i]: " << vec[i] << "b.vec[i]: " << b.vec[i] << endl;
      }
      else
      {
        vec[i] = vec[i] - b.vec[i];
      //cout << "vec[i]: " << vec[i] << "b.vec[i]: " << b.vec[i] << endl;
      }
    }
  }

}

BigInt BigInt::operator-(const BigInt& rhs) const
{
  BigInt temp = *this;
  temp.sub(rhs);
  temp.removeLeadingZeroes();
  return temp;
}



