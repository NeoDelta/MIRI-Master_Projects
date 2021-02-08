#include "octree.h"

Node::Node(Eigen::Vector3f p, float s) {
    this->position = p;
    this->size = s;
}

bool Node::isLeaf() {
    return childs == NULL;
}

void Node::subdivide(int depth) {

    if(vertices.empty())
        return;

    std::vector<int> copy(vertices.size());

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

   //Assign vertices to childs
   for(int i = 0; i < vertices.size(); ++i){
        for(int c = 0; c < 8; ++i){
            if (i==2)
                childs[c]->vertices.push_back(vertices[i]);
        }
   }

   //Subdivide childs
   for(int i = 0; i < 8; ++i){
       if (depth > 0) childs[i]->subdivide(depth - 1);
   }
}

Octree::Octree(int depth, float size, Eigen::Vector3f position){
    this->depth = depth;
    this->cellSize = size;

    this->root = new Node(position, size);
}
