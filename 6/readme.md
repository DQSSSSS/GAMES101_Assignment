# Assignment 6

## Task:

Implement BVH.

**Key words**: BVH 

PPT: 14

## Recall:

1. Spatial Partitions. Oct tree, KD tree, BSP tree
2. Object Partitions & BVH(Bounding Volume Hierarchy)
   1.  Recursively spilt set of objects in two subsets.
3. SAH(Surface Area H)

## Code:

spilt method:

naive: 

```c++
Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersection

    std::array<int, 3> dirIsNeg = {ray.direction.x>0, ray.direction.y>0, ray.direction.z>0};
    if(!node->bounds.IntersectP(ray, ray.direction_inv, dirIsNeg)) {
        return Intersection();
    } else {
        if(node->left && node->right) {
            auto L = getIntersection(node->left, ray);
            auto R = getIntersection(node->right, ray);
            if(L.distance > R.distance) return R;
            return L;
        } else {
            return node->object->getIntersection(ray);
        }   
    }
}
```

generation: 0s

render: 7s

a simple optimization. Function `IntersectPGetT` return the `<isIntersect,tenter>`, `tenter` is the t when ray entering this bbox.

```c++
Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersection

    std::array<int, 3> dirIsNeg = {ray.direction.x>0, ray.direction.y>0, ray.direction.z>0};
  /*  if(!node->bounds.IntersectP(ray, ray.direction_inv, dirIsNeg)) {
        return Intersection();
    } 
    else */
    {
        if(node->left && node->right) {
            auto p1 = node->left->bounds.IntersectPGetT(ray, ray.direction_inv, dirIsNeg);
            auto p2 = node->right->bounds.IntersectPGetT(ray, ray.direction_inv, dirIsNeg);

            if(!p1.first && !p2.first) return Intersection();
            if(!p1.first) return getIntersection(node->right, ray);
            if(!p2.first) return getIntersection(node->left, ray);

            float t1 = p2.second, t2 = p2.second;

            BVHBuildNode* first = node->left;
            BVHBuildNode* second = node->right;
            
            if(t1 <= t2) {
                std::swap(t1, t2); std::swap(first, second);
            }

            auto firstIns = getIntersection(first, ray);
            if(firstIns.distance > t2) {
                auto secondIns = getIntersection(second, ray);
                if(firstIns.distance > secondIns.distance)
                    return secondIns;
            }

            return firstIns;
        } else {
            return node->object->getIntersection(ray);
        }   
    }
}
```

generation: 0s

render: 7s



SAH:

For each axis, make B buckets on value range, place each triangle in the corresponding bucket according to its centroid's coordinates. Then for B-1 plane, choose the best one with the smallest C(cost). 
$$
C=C_{trav}+|A|S_A/S_NC_{isect}+|B|S_B/S_NC_{isect}
$$

http://15462.courses.cs.cmu.edu/fall2015/lecture/acceleration/slide_024


```c++
		if(splitMethod == SplitMethod::SAH) {
          
            struct Data {
                std::vector<Object*> lobjs, robjs;
                double C;
            };

            auto deal = [](std::vector<Object*> objects, int axis_id) {
                
                auto getValue = [](Object* obj, int id) {
                    if(id == 0) return obj->getBounds().Centroid().x;
                    if(id == 1) return obj->getBounds().Centroid().y;
                    if(id == 2) return obj->getBounds().Centroid().z;
                    assert(0);
                };

                int B = 50;
                double maxV = -std::numeric_limits<double>::infinity();
                double minV = std::numeric_limits<double>::infinity();
                for(auto obj : objects) {
                    maxV = std::max(maxV, (double)getValue(obj, axis_id));
                    minV = std::min(minV, (double)getValue(obj, axis_id));
                }

                std::vector<std::vector<Object*>> buckets(B);
                std::vector<Bounds3> lbd(B), rbd(B);
                for(auto obj : objects) {
                    auto val = getValue(obj, axis_id);
                    int i = std::min((int)((val-minV)/(maxV-minV) * B), B-1);
                    buckets[i].push_back(obj);
                    lbd[i] = Union(lbd[i], obj->getBounds());
                }
                for(int i = 0;i < B;i ++) rbd[i] = lbd[i];
                for(int i = 1;i < B;i ++) lbd[i] = Union(lbd[i], lbd[i-1]);
                for(int i = B-2;i >= 0;i --) rbd[i] = Union(rbd[i], rbd[i+1]);
                Bounds3 bd = lbd[B-1];

                int lsz = 0, rsz = objects.size(), sz = objects.size();
                double C = std::numeric_limits<double>::infinity();
                int partition = -1;
                for(int i = 0;i < B - 1;i ++) {
                    lsz += buckets[i].size();
                    rsz -= buckets[i].size();
                    if(lsz && rsz) {
                        double nowC = 1.0*lbd[i].area()/bd.area()*lsz
                                        + 1.0*rbd[i+1].area()/bd.area()*rsz;
                        
                        if(nowC < C) {
                            C = nowC;
                            partition = i;
                        }
                    }
                }
                assert(partition != -1);

                Data ret;
                ret.C = C;
                for(auto obj : objects) {
                    auto val = getValue(obj, axis_id);
                    int i = std::min((int)((val-minV)/(maxV-minV) * B), B-1);
                    if(i <= partition) ret.lobjs.push_back(obj);
                    else ret.robjs.push_back(obj);
                }

                return ret;
            };

            Data x_result = deal(objects, 0);
            Data y_result = deal(objects, 1);
            Data z_result = deal(objects, 2);

            if(x_result.C > y_result.C) std::swap(x_result, y_result);
            if(x_result.C > z_result.C) std::swap(x_result, z_result);

            //std::cout << x_result.C << std::endl;


            node->left = recursiveBuild(x_result.lobjs);
            node->right = recursiveBuild(x_result.robjs);

            node->bounds = Union(node->left->bounds, node->right->bounds);
        }

```

Need: `area()` in `Bound3`.

generation:0s

render: 6s