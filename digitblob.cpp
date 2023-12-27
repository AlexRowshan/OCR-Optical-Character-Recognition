#include "digitblob.h"
#include <iostream>
// Add more includes if necessary
#include <vector>

using namespace std;


bool Location::operator<(const Location& other) const
{
    /*Implement the operator of the Location struct to return true if the left-hand side 
    Location object is further left (column is smaller) 
    than the right-hand side Location object. 
    If the two have the same column, then use the row to break ties, 
    returning true if the left-hand side Location's row is smaller than the right-hand sides.*/
    if(this->col ==  other.col)
    {
        return (this->row < other.row);
    }
    return (this->col < other.col);
}


DigitBlob::DigitBlob()
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // ul_'s Location default constructor already initializes it to -1,-1

    // Initilaize h_ and w_ and any other data members
    h_ = 0;
    w_ = 0;

}


DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // Initilaize ul_, h_ and w_ and any other data members
    ul_ = upperleft;
    h_ = height;
    w_ = width;

}


DigitBlob::~DigitBlob()
{
 

}


void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features
    //cout << "Running tests:" <<endl;
    calc_centers_of_mass();
    calc_symmetry();

    
    if(euler_ == - 1)
    {
      digit_ = '8';
    }
    else if(euler_ == 0)
    {
      if(hs >= 90 || vs >= 90)
      {
        digit_ = '0';
      }
      else if(hs >= 62 && hc < 0.52)
      {
        digit_ = '6';
      }
      else if(vs <= 69 && hs >= 60 && hs <= 76)
      {
        digit_ = '9';
      }
      else
      {
        digit_ = '4';
      }
    }
    else if(euler_ == 1)
    {
      if((hs >= 63.5 && vs >= 72) || (hc >= 0.63 && hc <= 0.64))
      {
        digit_ = '1';
      }
      else if(vs >= 77 || (hc >= 0.60 && hc <= 0.61))
      {
        digit_ = '3';
      }
      else if(vs < 49 || (hs >= 65 && hs <= 67) || (vc > 0.4 && vc < 0.5))
      {
        digit_ = '5';
      }
      else if(vc <= 0.4)
      {
        digit_ = '7';
      }
      else
      {
        digit_ = '2';
      }
    }
    else
    {
      digit_ = 'L';
    }


}


char DigitBlob::getClassification() const
{
    return digit_;
}


void DigitBlob::printClassificationResults() const
{
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << "Euler number is " << euler_ << endl;
    





    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}


const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}


int DigitBlob::getHeight() const
{
    return h_;
}


int DigitBlob::getWidth() const
{
    return w_;
}


bool DigitBlob::operator<(const DigitBlob& other) const
{
    return ul_ < other.ul_;
}


void DigitBlob::calc_euler_number()
{
    euler_ = (bq1_ - bq3_ - 2*bqd_) / 4;
}


void DigitBlob::calc_bit_quads()
{
  for(int i=ul_.row-1; i < ul_.row + h_ ; i++)
  {
    for(int j=ul_.col-1; j < ul_.col + w_ ; j++)
    {
      int tl = img_[i][j];
      int tr = img_[i][j + 1];
      int bl = img_[i + 1][j];
      int br = img_[i + 1][j + 1];

      //counting blacks
      int bcnt = 0;
      if(tl == 0)
      {
        bcnt++;
      }
      if(tr == 0)
      {
        bcnt++;
      }
      if(bl == 0)
      {
        bcnt++;
      }
      if(br == 0)
      {
        bcnt++;
      }
      
      if(bcnt == 0)//bq0
      {
        bq0_++;
      }
      if(bcnt == 1)//bq1_
      {
        bq1_++;
      }
      if(bcnt == 2)//bq2_ and bqd_
      {
        if(tl == br)
        {
          bqd_++;
        }
        else
        {
          bq2_++;
        }
      }
      if(bcnt == 3)
      {
        bq3_++;
      }
      if(bcnt == 4)//bq4
      {
        bq4_++;
      }
    }
  }
}
// Add more private helper function implementations below

void DigitBlob::calc_centers_of_mass()
{
  double vertSum = 0;
  double horiSum = 0;
  double blks = 0;
  //center of mass 
  for(int i=0; i <h_ ; i++)
  {
    for(int j=0; j < w_ ; j++)
    {
      if(img_[i + ul_.row][j + ul_.col] == 0)
      {
        blks++;
        vertSum += i;
        horiSum += j;
      }
    }
  }
  vc = (vertSum / blks) / ( h_ - 1);
  hc = (horiSum / blks) / ( w_ - 1);

}

void DigitBlob::calc_symmetry()
{
  //horizontal symm
  double horiS = 0;
  double pixelsH = 0;
  for(int i=0; i < h_ ; i++)
  {
    for(int j=0; j < (w_ / 2) ; j++)
    {
      if(img_[i + ul_.row][j + ul_.col] == img_[i + ul_.row][ul_.col + w_ - j - 1])
      {
        horiS++;
      }
      pixelsH++;
    }
  }

  //vertical symm
  double vertS = 0;
  double pixelsV = 0;
  for(int i= 0; i < (h_ / 2); i++)
  {
    for(int j= 0; j < w_ ; j++)
    {
      if(img_[i + ul_.row][j + ul_.col] == img_[ul_.row + h_ - i - 1][j + ul_.col])
      {
        vertS++;
      }
      pixelsV++;
    }
  }
  
  vs = ((vertS / pixelsV) * 100);
  hs = ((horiS / pixelsH) * 100);
}

