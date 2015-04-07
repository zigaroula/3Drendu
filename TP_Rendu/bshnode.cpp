#include "bshnode.h"

BSHNode::BSHNode() {

}

BSHNode* BSHNode::build(Mesh maillage) {
    if (maillage.V.size() <4) {
        return this;
    }
    int i, taille;
    float rayon = 0;
    Vec3f centre, vecteurDistance, direction, normale_separation;

    taille = maillage.V.size();
    for (i = 0 ; i < taille ; i++) {
        centre += maillage.V[i].p;
    }
    centre /= taille;

    for (i = 0 ; i < taille ; i++) {
        vecteurDistance = maillage.V[i].p - centre;
        if (vecteurDistance.length()>rayon) {
            rayon = vecteurDistance.length();
            direction = maillage.V[i].p - centre;
        }
    }

    Mesh maillage1 = Mesh();
    Mesh maillage2 = Mesh();
    if (direction[0] >= direction[1]) {
        normale_separation = Vec3f(1.0, 0.0, 0.0);
    }
    if (direction[1] >= direction[2]) {
        normale_separation = Vec3f(0.0, 1.0, 0.0);
    }
    if (direction[2] >= direction[0]) {
        normale_separation = Vec3f(0.0, 0.0, 1.0);
    }

    int k1 = 0;
    int k2 = 0;
    for (i = 0 ; i < taille ; i++) {
        if (dot(maillage.V[i].p, normale_separation)>0) {
            k1++;
        }
        else {
            k2++;
        }
    }
    maillage1.V.resize(k1);
    maillage2.V.resize(k2);
    k1 = 0;
    k2 = 0;
    for (i = 0 ; i < taille ; i++) {
        if (dot(maillage.V[i].p, normale_separation)>0) {
            maillage1.V[k1].p = maillage.V[i].p;
            k1++;
        }
        else {
            maillage2.V[k2].p = maillage.V[i].p;
            k2++;
        }
    }

    leftChild = build(maillage1);
    rightChild = build(maillage2);
    position = centre;
    radius = rayon;
    normal = normale_separation;
    color = Vec3f(1,0,0);
    return this;

}
