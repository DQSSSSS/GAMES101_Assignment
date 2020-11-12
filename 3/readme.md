# Assignment 3

## Task:

Implement: interpolation, Blinn-Phong reflectance model. 

**Key words**: Shading, Blinn-Phong Reflectance Model, texture mapping.

PPT: 7-9

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



## Code:

