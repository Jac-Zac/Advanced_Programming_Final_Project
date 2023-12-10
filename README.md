# Project ..

Blah blah

# IMPORTANT

##### Not vectorized instructions:

Time taken: 25684.260000 ms

#### Equations:

I might want to reuse those

q ∗ (q + (z[0] − 0.25)) < z[1]2 ∗ 0.25, where q = (z[0] − 0.25)2 + z[1]2 (4.1)
z[0]2 + (2 ∗ z[0]) + 1 + z[1]2 < 0.0625

- unlikely -> Very helpfull instead of just checking the value in the mandelbrot set

How I visualize it:

```bash
./main test.pgm 5000 1000 && icat test.pgm
```

- Create a # define COLOR

> which makes the code colored instead of back and white

- Need to add SIMD

Exploit the symmetry of the set to half computation time

It should compute pixel 0, 0 and automatically copy that to the pixcel at -1 0 or something

Perhpas use memoization -> also think about the fact that things are recomputed

precompiled header https://www.youtube.com/results?search_query=precompiled+header+

Maybe to see: https://www.youtube.com/watch?v=rVQMaiz0ydk

Maybe use a stack to keep track of the previous values computed

Also If you want your custom implementation of complex to use simd

Example code

```c
#include <stdio.h>
 #include <math.h>
 int main()
 {
          /* screen ( integer) coordinate */
        int iX,iY;
        const int iXmax = 800;
        const int iYmax = 800;
        /* world ( double) coordinate = parameter plane*/
        double Cx,Cy;
        const double CxMin=-2.5;
        const double CxMax=1.5;
        const double CyMin=-2.0;
        const double CyMax=2.0;
        /* */
        double PixelWidth=(CxMax-CxMin)/iXmax;
        double PixelHeight=(CyMax-CyMin)/iYmax;
        /* color component ( R or G or B) is coded from 0 to 255 */
        /* it is 24 bit color RGB file */
        const int MaxColorComponentValue=255;
        FILE * fp;
        char *filename="new1.ppm";
        char *comment="# ";/* comment should start with # */
        static unsigned char color[3];
        /* Z=Zx+Zy*i  ;   Z0 = 0 */
        double Zx, Zy;
        double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
        /*  */
        int Iteration;
        const int IterationMax=200;
        /* bail-out value , radius of circle ;  */
        const double EscapeRadius=2;
        double ER2=EscapeRadius*EscapeRadius;
        /*create new file,give it a name and open it in binary mode  */
        fp= fopen(filename,"wb"); /* b -  binary mode */
        /*write ASCII header to the file*/
        fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
        /* compute and write image data bytes to the file*/
        for(iY=0;iY<iYmax;iY++)
        {
             Cy=CyMin + iY*PixelHeight;
             if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
             for(iX=0;iX<iXmax;iX++)
             {
                        Cx=CxMin + iX*PixelWidth;
                        /* initial value of orbit = critical point Z= 0 */
                        Zx=0.0;
                        Zy=0.0;
                        Zx2=Zx*Zx;
                        Zy2=Zy*Zy;
                        /* */
                        for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
                        {
                            Zy=2*Zx*Zy + Cy;
                            Zx=Zx2-Zy2 +Cx;
                            Zx2=Zx*Zx;
                            Zy2=Zy*Zy;
                        };
                        /* compute  pixel color (24 bit = 3 bytes) */
                        if (Iteration==IterationMax)
                        { /*  interior of Mandelbrot set = black */
                           color[0]=0;
                           color[1]=0;
                           color[2]=0;
                        }
                     else
                        { /* exterior of Mandelbrot set = white */
                             color[0]=255; /* Red*/
                             color[1]=255;  /* Green */
                             color[2]=255;/* Blue */
                        };
                        /*write color to the file*/
                        fwrite(color,1,3,fp);
                }
        }
        fclose(fp);
        return 0;
 }
```

# simd

8723 8729 52387
40089 40093 240570
