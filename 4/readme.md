# Assignment 4

## Task:

De Casteljau Algorithm.

**Key words**: Geometry 

PPT: 10-12

## Recall:

1. Represent.
   1. Implicit. For example: f(x,y,z)=...
      1. Constructive: Boolean operations.
      2. Signed distance functions. Gradually blend surface together.  [Wiki](https://en.wikipedia.org/wiki/Signed_distance_function).
   2. Explicit. Point cloud, polygon mesh ... Curve and surface.
2. Curves. Bezier Curves. De Casteljau Algorithm.
   1. Quadratic: Let's $t \in [0, 1]$ , choose point S on AB that |SA|=t|AB|, choose point T on BC that |TB|=t|BC|, then choose point P on ST that |PS|=t|ST|.  
   2. Cubic: Four input points.
   3. Let's $b_i^t$ is i-th point in t-th layer(both t, i's index from 0). $b_0^n$ is component by $b_i^0$ with Bernstein polynomials with unknown number t.
   4.  Properties:
      1. Interpolates endpoints. $b(0)=b_0^0, b(1) = b_n^0$ .
      2. Tangent tp end segments.
      3. Affine transformation.
      4. Curve is within convex hull of control points.
   5. Piecewise Bezier Curves. Continuity. 
3. Surfaces.
   1. Bezier surfaces. 1-D's extension.
   2. Mesh operations.
      1. Mesh subdivision. Create then tune.
         1. Catmull-Clark Subdivision.
      2. Mesh simplification. Edge collapsing. New vertex should minimize its sum of L2 distance.
      3. Mesh regularization.
4. Shadow mapping.
   1. Key idea: the points not in shadow mush be seen with both by the light and by the camera.
   2. Get depth image from light, then render from eye.
   3. Problems
      1. Hard shadows(point lights only)
      2. Quality depends on shadow map resolution.
      3. Equality comparison between float number.

## Code:

Antialiasing, two methods:

1. $\sum_i 255 \times \max(2-\exp(dist_i), 0)$
2. For 2*2 grids, $f_{i,j} = 255\times(\lfloor x+1-i\rfloor-x)(\lfloor y+1-j\rfloor-y)$ for each $i = 0,1,j = 0,1$ .

