# Simple Parser with variable support


## Project Info
- Cmake Ver: 3.28.3
- C Version: c23

# To run

### Generate build files with cmake in the project root directory
```cmake -GNinja -DCMAKE_BUILD_TYPE=Release -B build -S .```

### Build with ninja
```ninja -C build```

### run (from root)
```./build/bin/warmup2```

## Examples
Queries must begin with the keyword "computation" and end with a ".", you can have multiple expressions separated by a semicolon ";"

```
computation 1 + 1.                                                                      // output: 2
computation 1 + 1 - 1 - 1.                                                              // output: 0
computation (3+11)*3-3/3.                                                               // output: 41
computation 3+11*3-3/3.                                                                 // output: 35
computation (3+11-(3+5))*3-3/3.                                                         // output: 17
computation var hi <- 10; 1 + 1.                                                        // output: 2
computation var x <- 10 * 2; var y <- (x + 5) / 3; var z <- (y - 1) * 2; z + x - 5.     // output: 29
computation var i <- 2 * 3; var abracadabra <- 7; (((abracadabra * i))); i - 5 - 1 .    // output: 42 0
```
