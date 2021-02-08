#include "octree.h"
#include <fstream>
#include <iostream>

Node::Node(Eigen::Vector3f p, float s) {
    this->position = p;
    this->size = s;

    for(int i = 0; i < 8; ++i) {
        this->childs[i] = nullptr;
    }

    this->newVertex = Eigen::Vector3f(0,0,0);
    this->newNormal = Eigen::Vector3f(0,0,0);
}

bool Node::isLeaf() {
    return childs[0] == nullptr;
}

void Node::subdivide(int depth, const std::vector<float> &vertsPositions, const std::vector<float> &normals) {

    if(vertices.empty())
        return;

    //std::vector<int> copy(vertices.size());

    //Create childs nodes
    for(int i = 0; i < 8; ++i){
        Eigen::Vector3f newPos = this->position;

        if ((i & 4) == 4)
            newPos[1] += size * 0.25f;
        else
            newPos[1] -= size * 0.25f;

        if ((i & 2) == 2)
            newPos[0] += size * 0.25f;
        else
            newPos[0] -= size * 0.25f;

        if ((i & 1) == 1)
            newPos[2] += size * 0.25f;
        else
            newPos[2] -= size * 0.25f;

        childs[i] = new Node(newPos, size * 0.5f);


   }

   std::vector<int> vcopy(vertices);
   //Assign vertices to childs
   for(int c = 0; c < 8; ++c){
        for(int i = 0; i < vcopy.size(); i++){
            int id = vcopy[i]*3;
            Eigen::Vector3f v(vertsPositions[id], vertsPositions[id+1], vertsPositions[id+2]);

            if (    v.x() <= childs[c]->position.x() + childs[c]->size * 0.5f && v.x() > childs[c]->position.x() - childs[c]->size * 0.5f &&
                    v.y() <= childs[c]->position.y() + childs[c]->size * 0.5f && v.y() > childs[c]->position.y() - childs[c]->size * 0.5f &&
                    v.z() <= childs[c]->position.z() + childs[c]->size * 0.5f && v.z() > childs[c]->position.z() - childs[c]->size * 0.5f ){

                childs[c]->vertices.push_back(vcopy[i]);
                childs[c]->newVertex += v;
                childs[c]->newNormal += Eigen::Vector3f(normals[id], normals[id+1], normals[id+2]);

                vcopy.erase(vcopy.begin()+i);
                i--;
            }
        }
        //std::cout << "Number of vertices of child " << c << ": " << childs[c]->vertices.size() << std::endl;
        if(!childs[c]->vertices.empty()) {
            childs[c]->newVertex /= childs[c]->vertices.size();
            childs[c]->newNormal /= childs[c]->vertices.size();
        }

        //childs[c]->newVertex = childs[c]->position;
   }
   //Subdivide childs
   for(int i = 0; i < 8; ++i){
       if (depth > 0) childs[i]->subdivide(depth - 1, vertsPositions, normals);
   }
}

void Node::draw() {

    Eigen::Vector3f top(0,this->size * 0.5f, 0);
    Eigen::Vector3f rigth(-this->size * 0.5f, 0, 0);
    Eigen::Vector3f front(0, 0, this->size * 0.5f);

    Eigen::Vector3f tfr = this->position + top + front + rigth;
    Eigen::Vector3f tfl = this->position + top + front - rigth;
    Eigen::Vector3f tbr = this->position + top - front + rigth;
    Eigen::Vector3f tbl = this->position + top - front - rigth;

    Eigen::Vector3f bfr = this->position - top + front + rigth;
    Eigen::Vector3f bfl = this->position - top + front - rigth;
    Eigen::Vector3f bbr = this->position - top - front + rigth;
    Eigen::Vector3f bbl = this->position - top - front - rigth;

    //TF
    glVertex3f(tfr.x(), tfr.y(), tfr.z());
    glVertex3f(tfl.x(), tfl.y(), tfl.z());
    //BoF
    glVertex3f(bfr.x(), bfr.y(), bfr.z());
    glVertex3f(bfl.x(), bfl.y(), bfl.z());
    //FR
    glVertex3f(tfr.x(), tfr.y(), tfr.z());
    glVertex3f(bfr.x(), bfr.y(), bfr.z());
    //FL
    glVertex3f(tfl.x(), tfl.y(), tfl.z());
    glVertex3f(bfl.x(), bfl.y(), bfl.z());

    //TB
    glVertex3f(tbr.x(), tbr.y(), tbr.z());
    glVertex3f(tbl.x(), tbl.y(), tbl.z());
    //BoB
    glVertex3f(bbr.x(), bbr.y(), bbr.z());
    glVertex3f(bbl.x(), bbl.y(), bbl.z());
    //BR
    glVertex3f(tbr.x(), tbr.y(), tbr.z());
    glVertex3f(bbr.x(), bbr.y(), bbr.z());
    //BL
    glVertex3f(tbl.x(), tbl.y(), tbl.z());
    glVertex3f(bbl.x(), bbl.y(), bbl.z());

    //TR
    glVertex3f(tfr.x(), tfr.y(), tfr.z());
    glVertex3f(tbr.x(), tbr.y(), tbr.z());
    //TL
    glVertex3f(tfl.x(), tfr.y(), tfr.z());
    glVertex3f(tbl.x(), tbl.y(), tbl.z());
    //BoR
    glVertex3f(bfr.x(), bfr.y(), bfr.z());
    glVertex3f(bbr.x(), bbr.y(), bbr.z());
    //BoL
    glVertex3f(bfl.x(), bfl.y(), bfl.z());
    glVertex3f(bbl.x(), bbl.y(), bbl.z());

   if(!this->isLeaf()) {
       for(int i = 0; i < 8; ++i){
           childs[i]->draw();
       }
   }
}

void Node::drawByLvl(int l) {
    if(l==0 && !this->vertices.empty()) {
        Eigen::Vector3f top(0,this->size * 0.5f, 0);
        Eigen::Vector3f rigth(-this->size * 0.5f, 0, 0);
        Eigen::Vector3f front(0, 0, this->size * 0.5f);

        Eigen::Vector3f tfr = this->position + top + front + rigth;
        Eigen::Vector3f tfl = this->position + top + front - rigth;
        Eigen::Vector3f tbr = this->position + top - front + rigth;
        Eigen::Vector3f tbl = this->position + top - front - rigth;

        Eigen::Vector3f bfr = this->position - top + front + rigth;
        Eigen::Vector3f bfl = this->position - top + front - rigth;
        Eigen::Vector3f bbr = this->position - top - front + rigth;
        Eigen::Vector3f bbl = this->position - top - front - rigth;

        //TF
        glVertex3f(tfr.x(), tfr.y(), tfr.z());
        glVertex3f(tfl.x(), tfl.y(), tfl.z());
        //BoF
        glVertex3f(bfr.x(), bfr.y(), bfr.z());
        glVertex3f(bfl.x(), bfl.y(), bfl.z());
        //FR
        glVertex3f(tfr.x(), tfr.y(), tfr.z());
        glVertex3f(bfr.x(), bfr.y(), bfr.z());
        //FL
        glVertex3f(tfl.x(), tfl.y(), tfl.z());
        glVertex3f(bfl.x(), bfl.y(), bfl.z());

        //TB
        glVertex3f(tbr.x(), tbr.y(), tbr.z());
        glVertex3f(tbl.x(), tbl.y(), tbl.z());
        //BoB
        glVertex3f(bbr.x(), bbr.y(), bbr.z());
        glVertex3f(bbl.x(), bbl.y(), bbl.z());
        //BR
        glVertex3f(tbr.x(), tbr.y(), tbr.z());
        glVertex3f(bbr.x(), bbr.y(), bbr.z());
        //BL
        glVertex3f(tbl.x(), tbl.y(), tbl.z());
        glVertex3f(bbl.x(), bbl.y(), bbl.z());

        //TR
        glVertex3f(tfr.x(), tfr.y(), tfr.z());
        glVertex3f(tbr.x(), tbr.y(), tbr.z());
        //TL
        glVertex3f(tfl.x(), tfr.y(), tfr.z());
        glVertex3f(tbl.x(), tbl.y(), tbl.z());
        //BoR
        glVertex3f(bfr.x(), bfr.y(), bfr.z());
        glVertex3f(bbr.x(), bbr.y(), bbr.z());
        //BoL
        glVertex3f(bfl.x(), bfl.y(), bfl.z());
        glVertex3f(bbl.x(), bbl.y(), bbl.z());
    }
    else {
        if(!this->isLeaf()) {
            for(int i = 0; i < 8; ++i){
                childs[i]->drawByLvl(l-1);
            }
        }
    }
}
void Node::getVertsRefs(int depth, std::vector<int> *vr, std::vector<float> *vp, std::vector<float> *vn) {

    if(depth == 0 && !this->vertices.empty()) {

        int idx = vp->size()/3; //Create index per cell

        vp->push_back(this->newVertex.x());
        vp->push_back(this->newVertex.y());
        vp->push_back(this->newVertex.z());

        vn->push_back(this->newNormal.x());
        vn->push_back(this->newNormal.y());
        vn->push_back(this->newNormal.z());

        for(int i = 0; i < this->vertices.size(); i++) {
            vr->at(this->vertices[i]) = idx;
        }
        return;
    }

    if(!this->isLeaf()) {
        for(int i = 0; i < 8; ++i) {
            childs[i]->getVertsRefs(depth-1, vr, vp, vn);
        }
    }

    return;

}

Octree::Octree() {

}

Octree::Octree(int depth, float size, Eigen::Vector3f position, std::vector<int> vertices){
    this->depth = depth;
    this->cellSize = size;

    this->root = new Node(position, size);

    this->root->vertices = vertices;
}

void Octree::CreateOctree(const std::vector<float> &vertices, const std::vector<float> &normals) {

    this->root->subdivide(this->depth, vertices, normals);
}

void Octree::drawOctree() {

    glBegin(GL_LINES);

    this->root->draw();
    glEnd();
}

void Octree::drawOctreeByLvl(int l) {

    glBegin(GL_LINES);

    this->root->drawByLvl(l);
    glEnd();
}

void Octree::getVertsRefByDepth(int depth, std::vector<int> *v1, std::vector<float> *v2, std::vector<float> *v3) {

    this->root->getVertsRefs(depth, v1, v2, v3);
}


