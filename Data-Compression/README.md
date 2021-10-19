# Data-Compression
Waveform data compression in FPGA

## first_compress
压缩引擎一，利用波形采样间的上下文信息，使用**行程编码(run-length encoding, RLE)**和增量编码**（delta coding）**

## second_compress
压缩引擎二，实现了LZ77算法

## fse_compress
**有限状态熵编码（Finite State Entropy，FSE）**，有限状态熵编码时近年来熵编码领域的一个重大成果，它有着算术编码的压缩率和哈夫曼编码的速度，
被Facebook用在了目前在大数据系统中广泛使用的Zstd压缩软件库中
