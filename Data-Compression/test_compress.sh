#!/bin/bash

cd second_compress
bash test.sh $1 lz77_result
cd ..
cd fse_compress
bash test.sh /home/ypq/datacompress/data-compression/second_compress/lz77_result fse_result