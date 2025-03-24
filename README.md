
# HuffmanCoder

A C++ program that compresses and decompresses text files using **Huffman coding**.

---

## Usage

### Compress a file:
```bash
./zap zap input.txt compressed.zap
```

### Decompress a file:
```bash
./zap unzap compressed.zap output.txt
```

---

## Compile

```bash
g++ -o zap main.cpp HuffmanCoder.cpp BinaryIO.cpp
```

---

## Features

- Compress text files (Huffman encoding)
- Decompress encoded files
- Saves space with efficient binary encoding

---

Let me know if you want help with a `.gitignore` or adding file examples!
