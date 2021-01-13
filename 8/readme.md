# Assignment 8

## Task:

Mass-Spring System, a Rope Simulator.

PPT: 21-22

## Recall:

1. Animation
   1. Particle System
   2. Forward Kinematics
   3. Inverse Kinematics
   4. Rigging
   5. Motion Capture
2. Physical Simulation
   1. F=ma
3. Mass-Spring System
   1. Ideal, zero length. $f_{a->b} = k_s(b-a)$ . 
   2. Non-zero. $f_{a \to b}=k_s \frac {b-a}{||b-a||}(||b-a||-l)$
   3. $f_b=-k_d\frac {b-a}{||b-a||}(b'-a') * \frac {b-a}{||b-a||}$
4. Single Particle Simulation
   1. Explicit Euler Method
      1. x1 = x0 + delta_t * v, unstable
   2. Some Methods to Combat Instability
5. Rigid Body Simulation
   1. Similar to simulation a particle
6. Fluid Simulation
   1. Position-Based Method: Gradient Descent
   2. Eulerian vs. Lagrangian
   3. Material Point Method (MPM)



## Code:

