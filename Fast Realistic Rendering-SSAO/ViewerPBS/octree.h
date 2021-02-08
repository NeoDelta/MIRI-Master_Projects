#ifndef OCTREE_H
#define OCTREE_H


#include <eigen3/Eigen/Geometry>
#include <vector>
#include <GL/glew.h>

class Node {
    Eigen::Vector3f position;
    float size;
    Node* childs[8];

public:
    std::vector<int> vertices;
    Eigen::Vector3f newVertex;
    Eigen::Vector3f newNormal;

    Node(Eigen::Vector3f p, float s);

    bool isLeaf();

    void subdivide(int depth, const std::vector<float> &verticesPositions, const std::vector<float> &normals);

    void draw();
    void drawByLvl(int);

    void getVertsRefs(int depth, std::vector<int> *v1, std::vector<float> *v2, std::vector<float> *v3);
};

class Octree {
 private:

    int depth;
    float cellSize;
    Node* root;

 public:
    Octree();
    Octree(int depth, float size, Eigen::Vector3f position, std::vector<int> vertices);

    void CreateOctree(const std::vector<float> &vertices, const std::vector<float> &normals);

    void drawOctree();

    void drawOctreeByLvl(int);

    void getVertsRefByDepth(int depth, std::vector<int> *v1, std::vector<float> *v2, std::vector<float> *v3);

};

#endif // OCTREE_H
