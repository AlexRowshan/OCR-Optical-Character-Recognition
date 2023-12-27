# OCR (Optical Character Recognition)
Description:
In my OCR project, I developed an Optical Character Recognition system using C++ that processes black and white images containing bitmap representations of decimal digits (0-9). 
The project involves creating a NumImg class to encapsulate and abstract operations on digit images, implementing a connected component algorithm based on breadth-first search (BFS) 
to identify and classify digits, and utilizing a DigitBlob class to model individual digits with features such as bounding boxes and classification results. 
The classification is performed through feature analysis tests, including Euler number, aspect ratio, vertical and horizontal centroids, vertical and horizontal symmetry, 
and other user-defined measures. Additionally, the project extends to arithmetic operations on recognized digits, involving operator overloading in the BigInt class to support 
addition, subtraction, and comparison of large integers represented by the recognized digit strings. The program utilizes a BMP library for image processing and demonstrates 
proficiency in algorithmic design, data structures, and object-oriented programming.


Compilation Instructions:
g++ -g -Wall -std=c++11 bigint.cpp bmplib.cpp digitblob.cpp numimg.cpp ocr.cpp -o ocr

Single Character Test:
./ocr img-classify1.bmp img-classify1.bmp 8

Multi Character Test:
./ocr img-test1.bmp img-test1.bmp 7


