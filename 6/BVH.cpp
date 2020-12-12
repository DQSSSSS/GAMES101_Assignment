#include <algorithm>
#include <cassert>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else {
        if(splitMethod == SplitMethod::NAIVE) {
            Bounds3 centroidBounds;
            for (int i = 0; i < objects.size(); ++i)
                centroidBounds =
                    Union(centroidBounds, objects[i]->getBounds().Centroid());
            int dim = centroidBounds.maxExtent();
            switch (dim) {
            case 0:
                std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                    return f1->getBounds().Centroid().x <
                        f2->getBounds().Centroid().x;
                });
                break;
            case 1:
                std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                    return f1->getBounds().Centroid().y <
                        f2->getBounds().Centroid().y;
                });
                break;
            case 2:
                std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                    return f1->getBounds().Centroid().z <
                        f2->getBounds().Centroid().z;
                });
                break;
            }

            auto beginning = objects.begin();
            auto middling = objects.begin() + (objects.size() / 2);
            auto ending = objects.end();

            auto leftshapes = std::vector<Object*>(beginning, middling);
            auto rightshapes = std::vector<Object*>(middling, ending);

            assert(objects.size() == (leftshapes.size() + rightshapes.size()));

            node->left = recursiveBuild(leftshapes);
            node->right = recursiveBuild(rightshapes);

            node->bounds = Union(node->left->bounds, node->right->bounds);
        } else if(splitMethod == SplitMethod::SAH) {
          
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
        } else {
            std::cout << "error split method" << std::endl;
            assert(0);
        }
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray& ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

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
/*
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
}*/