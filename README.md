ascii raytracer
===============

This is a pet project to experiment with ray tracing. The goal is little more than to draw a simple scene with lighting and shadows in a terminal.

Compile with `gcc ascii_raytracer.c -o ascii_raytracer -lm` and run with `./ascii_raytracer`.

Alternatively if run with any command line arguments, for example `./ascii_raytracer 1` the program will output a large PGM file to STDOUT, which can be piped to a file and viewed easily. 

MIT licensed.

demo
====

Currently the program renders a small sphere orbiting a larger sphere. Something like this (will change a bit as I work on things):

ASCII:
       
                    
       ...          
     .......        
     ..,,...        
    ..., ,...       
    .+;.  ...       
    .+;.  ...       
    ..., ,...       
     ..,,...        
     .......        
       ...          

PGM:

<img src="https://raw.githubusercontent.com/JayWalker512/ascii_raytracer/master/img.png" width="300" height="300">

license
=======

Copyright (c) 2017 Brandon Foltz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
                                   
                                        
                                        
