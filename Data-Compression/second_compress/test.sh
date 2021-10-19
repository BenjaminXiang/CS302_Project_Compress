#!/bin/bash

cmake .
make

./compress_engine_2 $1 $2
./decompress_engine_2 $2 out.bin
diff $1 out.bin
wc $1
wc out.bin
wc $2
rm out.bin
# rm lz77_result
