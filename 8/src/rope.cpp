#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

//        Comment-in this part when you implement the constructor
//        for (auto &i : pinned_nodes) {
//            masses[i]->pinned = true;
//        }

        // node_mass: weight,  k: k_spring

        for(int i = 0; i < num_nodes; i ++) {
            Vector2D now = start + (end - start) * i / (num_nodes - 1);
            masses.push_back(new Mass(now, node_mass, 0));
        }

        for (auto &i : pinned_nodes) {
            masses[i]->pinned = true;
        }

        for(int i = 0;i < num_nodes - 1;i ++) {
            springs.push_back(new Spring(masses[i], masses[i+1], k));
        }

        springs[0]->k = -1; springs[1]->k = -1;
        masses[2]->pinned = true;
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        //if(t == 1000) while(1);
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            auto ab = (s->m2->position - s->m1->position).norm();
            auto ab_dir = (s->m2->position - s->m1->position) / ab;
            
            Vector2D f_a = s->k * ab_dir * (ab - s->rest_length);
            s->m1->forces += f_a;
            s->m2->forces += -f_a;
            //a->forces += -k * ab * (now_length - length);
            //b->forces += k * ab * (now_length - length);
        }

        bool is_explicit = false;
        
        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position

                // TODO (Part 2): Add global damping
                
                m->forces += gravity * m->mass;   

                if(is_explicit) {
                    float k_d = 7; // damping
                    m->forces += -k_d * m->velocity; 
                } else {
                    float k_d = 0.01; // damping
                    m->forces += -k_d * m->velocity; 
                }

                auto a = m->forces / m->mass;
                auto v = m->velocity;
                auto v_nx = v + a * delta_t;
                
                if(is_explicit) { // explicit
                    m->velocity = m->velocity + a * delta_t;
                    m->position = m->position + v * delta_t; 
                } else { // semi-implicit
                    m->velocity = m->velocity + a * delta_t;
                    m->position = m->position + v_nx * delta_t; 
                }
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            
            if(s->k != -1) {
                auto ab = (s->m2->position - s->m1->position).norm();
                auto ab_dir = (s->m2->position - s->m1->position) / ab;
                
                Vector2D f_a = s->k * ab_dir * (ab - s->rest_length);
                s->m1->forces += f_a;
                s->m2->forces += -f_a;
            }
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                m->forces += gravity * m->mass;   
                auto a = m->forces / m->mass;
                
                Vector2D x_t0 = m->last_position;
                Vector2D x_t1 = m->position;

                float damping = 0.00005;
                Vector2D x_t2 = x_t1 + (1-damping) * (x_t1 - x_t0) + a * delta_t * delta_t;

                m->last_position = x_t1;
                m->position = x_t2;

                // TODO (Part 3.1): Set the new position of the rope mass

                // TODO (Part 4): Add global Verlet damping
            }
            m->forces = Vector2D(0, 0);
        }

        for(auto& s : springs) {
            if(s->k != -1) continue;
            auto d = s->m2->position - s->m1->position; // 1->2
            auto dir = d / d.norm();
            auto len = s->rest_length;
            auto offset1 = 0.5 * dir * (d.norm()-len);
            auto offset2 = -0.5 * dir * (d.norm()-len);
            
            if(s->m1->pinned && s->m2->pinned) continue;

            if(s->m1->pinned) offset2 *= 2, offset1 = Vector2D(0, 0);
            if(s->m2->pinned) offset1 *= 2, offset2 = Vector2D(0, 0);

            s->m1->position += offset1;
            s->m2->position += offset2;
        }
    }
}
