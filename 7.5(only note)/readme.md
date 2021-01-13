# Assignment 7.5

## Task:

Advanced Topics in Rendering

Cameras, Lenses and Light Fields

Color and Perception

PPT: 18-20

## Recall:

1. Advanced Topics in Rendering
   1. Light Transport
      1. Unbiased: 
         1. Bidirectional Path Tracing (BDPT) : two directions.
         2. Metropolis Light Transport (MLT) : Markov Chain Monte Carlo, locally.
      2. Biased: 
         1. Photon Mapping (光子映射) : Specular-Diffuse-Specular (SDS). Firstly emit photons from the light source, stop on diffuse surfaces, then shoot sub-paths from the camera, hit on diffuse surfaces.
         2. Vertex Connection and Merging (VCM) : BDPT + Photon Mapping.
      3. Instant Radiosity (IR)
   2. Appearance Modeling
      1. Non-surface Models
         1. Participating Media (参与介质)
         2. Hair / fur (毛皮) / fiber (BCSDF)
         3. Granular (颗粒状) Material
      2. Surface Models
         1. Translucent (半透明) Material (BSSRDF)
         2. Cloth
         3. Detailed Material (Non-statistical BRDF)
      3. Procedural Appearance
2. Cameras, Lenses and Light Fields
   1. Pinhole Image Formation
   2. Field of View (FOV)
   3. Exposure (曝光)
3. Color and Perception
   1. The Plenoptic Function (全光函数). P(\theta, \phi, \lambda, t, x, y, z)
   2. Lumigraph
   3. Light Field Camera
   4. Color

