#include "bshnode.h"
#include <math.h>

BSHNode::BSHNode(Mesh maillage) {
    leftE = true;
    rightE = true;
    // Initialisations
    meshDebug = maillage;
    int i, taille, directionSeparation;
    float rayon = 0;
    Vec3f centre, vecteurDistance, direction, normale, couleur;


    taille = maillage.V.size(); // Calcul de la taille du maillage courant
    for (i = 0 ; i < taille ; i++) { // Calcul du centre du maillage courant
        centre += maillage.V[i].p;
    }
    centre /= taille;

    for (i = 0 ; i < taille ; i++) { // Recherche du point le plus distant
        vecteurDistance = maillage.V[i].p - centre;
        if (vecteurDistance.length()>rayon) { // Si on trouve un point plus éloigné que le point précédent
            rayon = vecteurDistance.length(); // Alors la plus grande distance est rayon
            direction = maillage.V[i].p - centre; // Et la direction du vecteur centre-point est direction
        }
    }
    std::cout << fabs(direction[0]) << " " << fabs(direction[1]) << " " << fabs(direction[2]) << " " << std::endl;

    Mesh maillage1 = Mesh();
    Mesh maillage2 = Mesh();
    if (fabs(direction[0]) >= fabs(direction[1]) && fabs(direction[0]) >= fabs(direction[2])) {
        directionSeparation = 0; // Separation selon X
    }
    if (fabs(direction[1]) >= fabs(direction[2]) && fabs(direction[1]) >= fabs(direction[0])) {
        directionSeparation = 1; // Separation selon Y
    }
    if (fabs(direction[2]) >= fabs(direction[0]) && fabs(direction[2]) >= fabs(direction[1])) {
        directionSeparation = 2; // Separation selon Z
    }
    std::cout << directionSeparation << std::endl;
    int k1 = 0;
    int k2 = 0;
    for (i = 0 ; i < taille ; i++) { // Calcul de la taille des deux nouveaux maillages
        if (maillage.V[i].p[directionSeparation]>centre[directionSeparation]) {
            k1++;
        }
        else {
            k2++;
        }
    }
    //std::cout << taille << " " << k1 << " " << k2 << std::endl;
    maillage1.V.resize(k1); // Attribution de la taille
    maillage2.V.resize(k2);

    k1 = 0;
    k2 = 0;
    for (i = 0 ; i < taille ; i++) { // Affectation des sommets aux maillages
        if (maillage.V[i].p[directionSeparation]>centre[directionSeparation]) {
            maillage1.V[k1].p = maillage.V[i].p;
            k1++;
        }
        else {
            maillage2.V[k2].p = maillage.V[i].p;
            k2++;
        }
    }
    //std::cout << taille << " " << k1 << " " << k2 << std::endl;
    //std::cout << maillage.V.size() << " " << maillage1.V.size() << " " << maillage2.V.size() << std::endl;
    if (maillage.V.size() >50) { // Si le maillage est encore séparable, on sépare
        leftChild = new BSHNode(maillage1);
        rightChild = new BSHNode(maillage2);
    } else { // Sinon TODO
	leftE = false;
	rightE = false;
    }
    
    // Affectations au node courant
    position = centre;
    radius = rayon;
    //TODO NORMAL A CALCULER
    normale = Vec3f(1,0,0);
    couleur = Vec3f(1,0,0);

    // DEBUG
    if (maillage.V.size() >0) {
       
    }
}

BSHNode::BSHNode(Vec3f _position, Vec3f _normal, Vec3f _color, float _radius, BSHNode* _leftChild, BSHNode* _rightChild, Mesh _meshDebug) {
	position = _position;
        normal = _normal;
        color = _color;
        radius = _radius;
        leftChild = _leftChild;
        rightChild = _rightChild;
        meshDebug = _meshDebug;
}

void BSHNode::sphereEnglobante() {

}

void BSHNode::redistribution() {

}
