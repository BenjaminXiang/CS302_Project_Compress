#!/bin/bash

g++ -o compress fse_compress.cpp
g++ -o decompress fse_decompress.cpp

./compress $1 after_compress.txt
./decompress after_compress.txt res.txt
diff $1 res.txt
wc $1
wc after_compress.txt
rm after_compress.txt
rm res.txt
