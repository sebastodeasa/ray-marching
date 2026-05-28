#include "Scene.h"

// returns minimum distance from point p to any object in the scene
float Scene::minDistanceFromPoint(const Vector3& p) const
{
    // if no objects in scene, return high value
    if (objects.empty())
        return 9999999999;

    float result = objects[0]->distanceFromPoint(p);
    for (int i = 1; i < objects.size(); i++)
    {
        float dist = objects[i]->distanceFromPoint(p);
        if (dist < result)
            result = dist;
    }
    return result;
}

// returns maximum distance from point p to any object in the scene
float Scene::maxDistanceFromPoint(const Vector3& p) const
{
    // if no objects in scene, return low value
    if (objects.empty())
        return 0;

    float result = 0;
    for (int i = 0; i < objects.size(); i++)
    {
        float dist = objects[i]->distanceFromPoint(p);
        if (dist > result)
            result = dist;
    }
    return result;
}

// returns color of object closest to point p
sf::Color Scene::closestColorToPoint(const Vector3& p) const
{
    SceneObject* closest = objects[0];
    float minDist = closest->distanceFromPoint(p);
    for (int i = 1; i < objects.size(); i++)
    {
        float dist = objects[i]->distanceFromPoint(p);
        if (dist < minDist)
        {
            minDist = dist;
            closest = objects[i];
        }
    }
    return closest->getColor();
}

// returns color of object furthest from point p
sf::Color Scene::furthestColorFromPoint(const Vector3& p) const
{
    SceneObject* furthest = objects[0];
    float maxDist = 0;
    for (int i = 0; i < objects.size(); i++)
    {
        float dist = objects[i]->distanceFromPoint(p);
        if (dist > maxDist)
        {
            maxDist = dist;
            furthest = objects[i];
        }
    }
    return furthest->getColor();
}

// deletes all objects in the scene and clears the vector (called by destructor)
void Scene::clearObjects()
{
    for (std::vector<SceneObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++)
        delete* itr;
    objects.clear();
}

Scene::~Scene()
{
    clearObjects();
}