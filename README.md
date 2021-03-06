Mandelbrot optimizaton with SIMD instructions
=============================================

Usage
-----------
1.  Processor
    > AMD Ryzen 5 4500U
2.  Instructions
    > AVX, AVX2
3.  Language
    > C, C++
4.  Graphic library
    > SFML
5.  Compiler
    > GCC

Controls
--------
1. Move up
    >UP
2. Move down
    >DOWN
3. Move left
    >LEFT
4. Move right
    >RIGHT
5. Zoom in
    >PGUP
6. Zoom out
    >PGDN


Compile
-------
+       g++ main.cpp -O2 -mavx -mavx2 -I"C:\Program Files\MinGW64\SFML-2.5.1\include" \
                                      -L"C:\Program Files\MinGW64\SFML-2.5.1\lib" \
                                      -lsfml-graphics -lsfml-window -lsfml-system

Results
-------

### Without SIMD instructions
![NOSIMD](/NOSSE.png)

### Using AVX & AVX2
![SIMD](/SSE.png)

CONCLUSION
----------
As seen, an increase in 
### fps is about 6 times.
The result has been gotten using
1.      __mm256 (8 32 single-precision float variables)
2.      AVX & AVX2 instructions
