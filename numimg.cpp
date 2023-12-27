#include "digitblob.h"
#include "numimg.h"
#include "bmplib.h"
#include <algorithm> // for std::sort
#include <deque>
#include <iomanip>
#include <iostream>

using namespace std;


NumImg::NumImg(const char* bmp_filename)
{
    //  Note: readGSBMP dynamically allocates a 2D array 
    //    (i.e. array of pointers (1 per row/height) where each  
    //    point to an array of unsigned char (uint8_t) pixels)

    
    
    img_ = readGSBMP(bmp_filename, h_, w_);

    // Leave this check
    if(img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }
    
    // Convert to Black and White using a fixed threshold 
    for(int i =0; i < h_; i++){
        for(int j = 0; j < w_; j++){
            if(img_[i][j] > 150){
                img_[i][j] = 255;
            }
            else {
                img_[i][j] = 0;
            }
        }
    }
    // Perform any other initialization you need
    visited = new bool*[h_];
    for(int i = 0; i < h_; i++)
    {
      visited[i] = new bool[w_];
    }

    for(int i = 0; i < h_; i++)
    {
      for(int j = 0; j < w_; j++)
      {
        visited[i][j] = false;
      }
    }
}


NumImg::~NumImg()
{
    // Add code here if necessary
    for(int i = 0; i < h_; i++)
    {
      delete[] img_[i];
      delete[] visited[i];
    }
    delete[] img_;
    delete[] visited;
}


size_t NumImg::findAndCreateDigitBlobs()
{
  for(int i = 0; i < h_; i++)
  {
    for(int j = 0; j < w_; j++)
    {
      if(img_[i][j] == 0 && visited[i][j] == false)
      {
        blobs_.push_back(createDigitBlob(visited, i, j));
      }
    }
  }
  sortDigitBlobs();
  return blobs_.size();

}


std::string NumImg::classify(bool withDebug)
{
    std::string res;
    for(size_t i = 0; i < blobs_.size(); i++){
        blobs_[i].classify();
        if(withDebug){
            blobs_[i].printClassificationResults();
        }
        char c = blobs_[i].getClassification();
        res += c;
    }
    return res;
}


void NumImg::printBoundingBoxes() const
{
    cout << setw(2) << "i" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for(size_t i = 0; i < blobs_.size(); i++){
        const DigitBlob& b = blobs_[i];
        cout << setw(2) << i << setw(6) << b.getUpperLeft().row << setw(6) << b.getUpperLeft().col 
        << setw(4) << b.getHeight() << setw(4) << b.getWidth()  << endl;
        // cout << "Blob " << i << " ul=(" << b.getUpperLeft().row << "," << b.getUpperLeft().col 
        //     << ") h=" << b.getHeight() << " w=" << b.getWidth() << endl;
    }

}


const DigitBlob& NumImg::getDigitBlob(size_t i) const
{
    if(i >= blobs_.size()){
        throw std::out_of_range("Index to getDigitBlob is out of range");
    }
    return blobs_[i];
}


size_t NumImg::numDigitBlobs() const
{
    return blobs_.size();
}


void NumImg::sortDigitBlobs()
{
    std::sort(blobs_.begin(), blobs_.end());
}


void NumImg::drawBoundingBoxesAndSave(const char* filename)
{
    for(size_t i=0; i < blobs_.size(); i++){
        Location ul = blobs_[i].getUpperLeft();
        int h = blobs_[i].getHeight();
        int w = blobs_[i].getWidth();
        for(int i = ul.row-1; i < ul.row + h + 1; i++){
            img_[i][ul.col-1] = 128;
            img_[i][ul.col+w] = 128;
        }
        for(int j = ul.col-1; j < ul.col + w + 1; j++){
            img_[ul.row-1][j] = 128;
            img_[ul.row+h][j] = 128;
        }
    }
    writeGSBMP(filename, img_, h_, w_);
}



DigitBlob NumImg::createDigitBlob(bool** explored, int pr, int pc)
{
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};

    // Add your code here
     //creating deque
  deque<Location> q;
  
  //marking the start location as explored
   explored[pr][pc] = true;

   //adding start location to q
    Location startLoc;
    startLoc.row = pr;
    startLoc.col = pc;
    q.push_back(startLoc);

    //smallest row and col - max left row and col 
    int maxLR = pr;
    int maxLC = pc;

    //largest row and col - max right row and col
    int maxRR = 0;
    int maxRC = 0;

    while(q.size() != 0)
    {
      Location check = q[0];
      q.pop_front();

      for(int i = 0; i < 8; i++)
      {
        int currR = check.row + neighbor_row[i];
        int currC = check.col + neighbor_col[i];
        if(currR >= 0 && currC >= 0 && currR <= h_ && currC <= w_ && explored[currR][currC] == false && img_[currR][currC] == 0)
        {
            explored[currR][currC] = true;
            Location add;
            add.row = currR;
            add.col = currC;
            q.push_back(add);

            //find index of smallest row and col (top left corner)
            if(currR < maxLR)
            {
              maxLR = currR;
            }
            if(currC < maxLC)
            {
              maxLC = currC;
            }
            //find index of largest row and col (bottom right corner)
            if(currR > maxRR)
            {
              maxRR = currR;
            }
            if(currC > maxRC)
            {
              maxRC = currC;
            }
        }
      }
    }
      //creating the digit blob upper left location
      Location ul;
      ul.row = maxLR;
      ul.col = maxLC;

      //creating the digit blob height and width
      int height = maxRR - maxLR;
      int width = maxRC - maxLC;
      DigitBlob create(img_, ul, height +1, width +1);
      
      return create;
  
}
