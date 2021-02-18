//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}


// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here

    Intersection intersection = intersect(ray);
    
    if(!intersection.happened)
        return Vector3f(0, 0, 0); 
    if(intersection.emit.norm()>0) {
        if(depth == 0)
            return intersection.emit;
        else
            return Vector3f(0, 0, 0);
    }

    Vector3f& p = intersection.coords;
    Vector3f wo = normalize(-ray.direction);
    Vector3f normal = normalize(intersection.normal);
    Material*& material = intersection.m;

    auto format = [](Vector3f &a) {
        if(a.x < 0) a.x = 0;
        if(a.y < 0) a.y = 0;
        if(a.z < 0) a.z = 0;
    };

    // direct
    Vector3f L_direct; 
    {
        Intersection inter_dir; 
        float pdf_dir;
        sampleLight(inter_dir, pdf_dir);

        Vector3f& x = inter_dir.coords;
        Vector3f ws = normalize(x - p);
        Vector3f light_normal = normalize(inter_dir.normal);
        
        auto pws = intersect(Ray(p, ws));
        if(pws.happened && (pws.coords-x).norm() < 1e-2) {
            L_direct = inter_dir.emit * material->eval(ws, wo, normal) * dotProduct(normal, ws) 
                * dotProduct(light_normal, -ws) / (dotProduct((x-p), (x-p)) * pdf_dir);
            format(L_direct);
        }
    }
    // indirect
    Vector3f L_indirect;
    {
        float RR = this->RussianRoulette;
        if(get_random_float() < RR) {
            Vector3f wi = normalize(material->sample(wo, normal));
            L_indirect = castRay(Ray(p, wi), depth+1) 
                * material->eval(wi, wo, normal) * dotProduct(wi, normal)
                / (material->pdf(wi, wo, normal) * RR);
            format(L_indirect);
        }
    }
    return L_direct + L_indirect;
}
