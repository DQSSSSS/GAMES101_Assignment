# Assignment 7

## Task:

Implement path tracing.

**Key words**: Path tracing 

PPT: 14-17

## Recall:

1. Radiometry
   1. Definition
      1. Radiant: the energy of electromagnetic radiation. Q.
      2. Radiant flux (power): Radiant per unit time. Φ = dQ/dt.
      3. Radiant intensity: radiant flux per unit solid angle.  dΦ/dw = d^2Q/dtdw.
      4. Solid angle: ratio of subtended area on sphere to radius squared. A/r^2.
      5. Irradiance: the power per unit area incident on a surface point. dΦ/dA = d^2Q/dtdA.
      6. Radiance: the power per unit solid angle, per projected unit area. d^2Φ/dwdAcosθ.
         1. The fundamental. Rendering is all about computing radiance.
   2. Irradiance fall off.
2. Bidirectional Reflectance Distribution Function (BRDF)
   1. How much light is reflected into each outgoing direction w_r from each incoming direction.
3. Reflection Equation
4. Rendering Equation (add emission term)
5. Monte Carlo Path Tracing
   1. Monte Carlo Integration. https://zhuanlan.zhihu.com/p/146144853.
   2. Solve Integration.![shade-func](https://github.com/DQSSSSS/GAMES101_Assignment/blob/master/7/images/shade-func.png?raw=true)
6. BRDF (Material)
   1. Diffuse, Glossy, Ideal reflective/refractive
   2. Fresnel Reflection/Term (菲涅尔项). Different grazing angle, different reflectance. 
      1. Dielectric (绝缘体), Conductor (导体). Rs, Rp, R=(Rs+Rp)/2. Schlick's approximation.
   3. Microfacet Theory
      1. Fundamentals: Individual elements of surface act like mirrors.
      2. **Fresnel Term**: rate of total energy.  **Shadowing-Masking (Geometry) Term**: microfacets' occlusion. **Distribution of normal**: the distribution of microfacets' normal vectors.
      3.  [Cook-Torrance model](https://zhuanlan.zhihu.com/p/25421091)
      4. Calculation: https://zhuanlan.zhihu.com/p/25539396
   4. Isotropic / Anisotropic Material
   5. Properties of BRDFs
      1. Non-negativity: f(wi->wo) >= 0
      2. Linearity: f = f1 + f2, render(BRDF=f) = render(BRDF=f1) + render(BRDF=f2)
      3. f(wi->wo) = f(wo->wi)
      4. $\int f(w_i \to w_o)\cos\theta_i dw_i \le 1$
      5. Isotropic: $f(\theta_i, \phi_i, \theta_r, \phi_r) = f(\theta_i,\theta_r,|\phi_i-\phi_r|)$



## Code:

**Problem of Framework**: Random generator should be static, Sphere's getIntersection should consider the precision.

![shade-exp](https://github.com/DQSSSSS/GAMES101_Assignment/blob/master/7/images/shade-exp.png?raw=true)



![256-spp](https://github.com/DQSSSSS/GAMES101_Assignment/blob/master/7/images/256-spp.png?raw=true)

Multi-threading: 256spp, 120s.

Microfacet:

![256-spp-microfacet](https://github.com/DQSSSSS/GAMES101_Assignment/blob/master/7/images/256-spp-microfacet.png?raw=true)