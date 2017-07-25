ascii raytracer
===============

This is a pet project to experiment with ray tracing. The goal is little more than to draw a simple scene with lighting and shadows in a terminal.

Compile with `gcc ascii_raytracer.c -o ascii_raytracer -lm` and run with `./ascii_raytracer`. Nothing else to it.

MIT licensed.

demo
====

Currently the program draws this simple orthographic projection of a sphere, with distance based shading.

                                       
                                        
                 ......                 
            ...,,,,,,,,,,...            
         ..,,,::::::::::::,,,..         
       ..,,:::;;;;oooo;;;;:::,,..       
      ..,::;;;oooxxxxxxooo;;;::,..      
     ..,::;;ooxxx%%%%%%xxxoo;;::,..     
     .,,:;;ooxx%%%####%%%xxoo;;:,,.     
    ..,::;;oxx%%##@@@@##%%xxo;;::,..    
    ..,::;;oxx%%##@@@@##%%xxo;;::,..    
     .,,:;;ooxx%%%####%%%xxoo;;:,,.     
     ..,::;;ooxxx%%%%%%xxxoo;;::,..     
      ..,::;;;oooxxxxxxooo;;;::,..      
       ..,,:::;;;;oooo;;;;:::,,..       
         ..,,,::::::::::::,,,..         
            ...,,,,,,,,,,...            
                 ......                 
                                        
                                        
                                        
                                        
