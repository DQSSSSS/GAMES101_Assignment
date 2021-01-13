# Assignment 2

## Task:

Implement Z-buffer and super-sampling anti-aliasing(SSAA).

**Key words**: Sampling.

PPT: 5-7

## Recall:

1. Antialiasing. Blurring(pre-filtering) before sampling.
   1. Filtering(=Averaging): Getting rid of certain frequency contents. 
   2. Sampling: Repeating frequency contents.
   3. Antialiasing = Limiting then repeating.
2. Antialiasing by super-sampling.
   1. MSAA(Multi-Sampling Anti-Aliasing): Sampling multiple locations pre pixel, then averaging.
   2. Today: FXAA, TAA, DLSS.
3. Z-buffer. Saving depth.

## Code:

Adaptive any N, change it in **rasterizer.hpp** line 102.

Z-buffer:

![image](https://github.com/DQSSSSS/GAMES101_Assignment/blob/master/2/md_image/naive.png?raw=true)

SSAA:

N=2

![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/master/2/md_image/SSAA2.png?raw=true)

N=3

![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/master/2/md_image/SSAA3.png?raw=true)
