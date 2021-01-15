# Assignment 3

## Task:

Implement: interpolation, Blinn-Phong reflectance model. 

**Key words**: Shading, Blinn-Phong Reflectance Model, texture mapping.

PPT: 7-10

## Recall:

1. Blinn-Phong Reflectance Model   ($\vec l, \vec n, \vec v$ represent direction of light, normal, view respectively, and their length is 1.)
   1. Diffuse: $k_d (I / r^2) \max(0, \vec n \cdot \vec l )$ . $I$ is energy arrived at the shading point.
   2. Specular: $k_s(I/r^2)\max(0,\vec n \cdot \vec h)^p$.  $\vec h = \frac{\vec v + \vec l}{\parallel \vec v + \vec l\parallel}$ (half vector). Increasing $p$ make reflection lobe narrow. 
   3. Ambient: $k_aI_a$
2. Shading frequencies
   1. Flat shading. Each triangle.
   2. Gouraud shading. Each vertex, interpolate colors.
   3. Phong shading. Each pixel. Interpolate normal vectors across triangle.
3. Graphics pipeline
   1. Vertex processing -> Triangle processing -> Rasterization -> Fragment processing -> Framebuffer operations.
   2. https://www.shadertoy.com/view/ld3Gz2
4. Barycentric Coordinates(重心坐标), for interpolating.
   1. For $P(x,y)$, $(x, y) \to (\alpha, \beta, \gamma)$ . $\alpha = \frac{S_{BCP}}{S_{ABC}}, \beta = \frac{S_{ACP}}{S_{ABC}}, \gamma = 1-\alpha-\beta$.
   2. $V_P = \alpha V_A + \beta V_B + \gamma V_C$. $V$ can be any attributes.
5. Texture mapping
   1. Each triangle vertex is assigned a texture coordinate (u, v).
   2. Magnification? Sampling problem.
      1. Insufficient texture resolution. Blur.
      2. Otherwise. Sampling on large range. Moire(摩尔纹). 
         1. Data structure for range queries. Mipmap(square). Anisotropic Filtering.
   3. Applications
      1. Environment lighting. Map.
      2. Microgeometry. 
         1. Bump shading: Perturb normal by gradient decent. Fake detail geometry.
         2. Displacement shading: Actually moves the vertices.
      3. Procedural noise.
      4. Precomputed shading.
      5. 3D textures, volume rendering.

## Code:

More images in "images/“.

normal  shader: 
![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/dev/3/images/spot/normal.png?raw=true)

phong shader: 
![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/dev/3/images/spot/phong.png?raw=true)

texture shader: 
![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/dev/3/images/spot/texture.png?raw=true)

bump shader: 
![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/dev/3/images/spot/bump.png?raw=true)

displacement shader: 
![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/dev/3/images/spot/displacement.png?raw=true)


no interpolate:
![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/dev/3/images/texture-noitplt.png?raw=true)

bilinear interpolate:
![img](https://github.com/DQSSSSS/GAMES101_Assignment/blob/dev/3/images/texture-2itplt.png?raw=true)



I don't know why the texture is translation.

