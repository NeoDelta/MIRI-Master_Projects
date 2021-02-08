
#include <octree.h>

Node::Node(Eigen::Vector3f p, float s) {
    this->position = p;
    this->size = s;
}

bool Node::isLeaf() {
    return childs == NULL;
}

Octree::Octree(int depth, float size, Eigen::Vector3f position){
    this->depth = depth;
    this->cellSize = size;

    this->root = new Node(position, size);
}
