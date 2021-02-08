#ifndef OCTREE_H
#define OCTREE_H


#include <eigen3/Eigen/Geometry>
#include <vector>

class Node {
    Eigen::Vector3f position;
    float size;
    Node* childs[8];

    std::vector<int> vertices;
    Eigen::Vector3f newVertex;

public:
    Node(Eigen::Vector3f p, float s);

    bool isLeaf();

    void subdivide(int depth);
};

class Octree {
 private:

    int depth;
    float cellSize;
    Node* root;

 public:

    Octree(int depth, float size, Eigen::Vector3f position);

};

#endif // OCTREE_H
