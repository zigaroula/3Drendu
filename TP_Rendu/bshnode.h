#ifndef BSHNODE_H
#define BSHNODE_H

#include "Vec3.h"
#include "Mesh.h"

class BSHNode {
    public:
        BSHNode(Mesh);
        BSHNode(Vec3f, Vec3f, Vec3f, float, BSHNode*, BSHNode*, Mesh);
        void sphereEnglobante();
        void redistribution();
        Vec3f getPosition() { return position; }
        float getRadius() { return radius; }
	BSHNode * getLeftChild() { return leftChild; }
	BSHNode * getRightChild() { return rightChild; }
        Mesh getMeshDebug() { return meshDebug; }
	bool left() { return leftE; }
	bool right() { return rightE; }
    private:
        Vec3f position;
        Vec3f normal;
        Vec3f color;
        float radius;
        BSHNode * leftChild;
        BSHNode * rightChild;
        Mesh meshDebug;
	bool leftE;
	bool rightE;
};

#endif // BSHNODE_H
