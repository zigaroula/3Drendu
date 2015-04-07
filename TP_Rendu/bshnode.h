#ifndef BSHNODE_H
#define BSHNODE_H

#include "Vec3.h"
#include "Mesh.h"

class BSHNode {
    public:
        BSHNode();
        BSHNode * build(Mesh);
        void sphereEnglobante();
        void redistribution();
        Vec3f getPosition() { return position; }
        float getRadius() { return radius; }
    private:
        Vec3f position;
        Vec3f normal;
        Vec3f color;
        float radius;
        BSHNode * leftChild;
        BSHNode * rightChild;
};

#endif // BSHNODE_H
