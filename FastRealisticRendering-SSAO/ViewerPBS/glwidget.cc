// Author: Marc Comino 2020

#include <glwidget.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "./mesh_io.h"
#include "./triangle_mesh.h"

namespace {

const double kFieldOfView = 60;
const double kZNear = 0.0001;
const double kZFar = 10;

const char kReflectionVertexShaderFile[] = "../shaders/reflection.vert";
const char kReflectionFragmentShaderFile[] = "../shaders/reflection.frag";
const char kBRDFVertexShaderFile[] = "../shaders/brdf.vert";
const char kBRDFFragmentShaderFile[] = "../shaders/brdf.frag";
const char kSkyVertexShaderFile[] = "../shaders/sky.vert";
const char kSkyFragmentShaderFile[] = "../shaders/sky.frag";
const char kGeometryPassVertexShaderFile[] = "../shaders/aogeometrypass.vert";
const char kGeometryPassFragmentShaderFile[] = "../shaders/aogeometrypass.frag";
const char kSSAOVertexShaderFile[] = "../shaders/ssao.vert";
const char kSSAOFragmentShaderFile[] = "../shaders/ssao.frag";
const char kBlurFragmentShaderFile[] = "../shaders/blur.frag";
const char k3DSSAOFragmentShaderFile[] = "../shaders/3Dssao.frag";
const char kSAOFragmentShaderFile[] = "../shaders/sao.frag";

const int kVertexAttributeIdx = 0;
const int kNormalAttributeIdx = 1;

unsigned int meshVAO, meshVBO, meshEBO, meshNormalsVBO, offsetVBO;
unsigned int skyboxVAO, skyboxVBO;

// SSAO - Buffers and textures pointers
unsigned int gFBO, gPosition, gNormal, depthRBO, ssaoFBO, ssao, blurFBO, blurSSAO, noiseTex, screenQuadVAO, screenQuadVBO;

std::vector<int> newTriangles;
float cellsize;

std::vector<Eigen::Vector3f> ssaoSamples, ssaoNoise;
float screenQuadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

float planeVertices[] = {

    -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f

    };

float skyboxVertices[] = {         
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f

    };

std::vector<float> translations;

QTime timer;
double frameCount;

bool ReadFile(const std::string filename, std::string *shader_source) {
  std::ifstream infile(filename.c_str());

  if (!infile.is_open() || !infile.good()) {
    std::cerr << "Error " + filename + " not found." << std::endl;
    return false;
  }

  std::stringstream stream;
  stream << infile.rdbuf();
  infile.close();

  *shader_source = stream.str();
  return true;
}

bool LoadImage(const std::string &path, GLuint cube_map_pos) {
  QImage image;
  bool res = image.load(path.c_str());
  if (res) {
    QImage gl_image = image.mirrored();
    glTexImage2D(cube_map_pos, 0, GL_RGBA, image.width(), image.height(), 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
  }
  return res;
}

bool LoadCubeMap(const QString &dir) {
  std::string path = dir.toUtf8().constData();
  bool res = LoadImage(path + "/right.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
  res = res && LoadImage(path + "/left.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
  res = res && LoadImage(path + "/top.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
  res = res && LoadImage(path + "/bottom.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
  res = res && LoadImage(path + "/back.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
  res = res && LoadImage(path + "/front.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

  if (res) {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  return res;
}

bool LoadProgram(const std::string &vertex, const std::string &fragment,
                 QOpenGLShaderProgram *program) {
  std::string vertex_shader, fragment_shader;
  bool res =
      ReadFile(vertex, &vertex_shader) && ReadFile(fragment, &fragment_shader);

  if (res) {
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                     vertex_shader.c_str());
    program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     fragment_shader.c_str());
    program->bindAttributeLocation("vertex", kVertexAttributeIdx);
    program->bindAttributeLocation("normal", kNormalAttributeIdx);
    program->link();
  }

  return res;
}

}  // namespace

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
      initialized_(false),
      width_(0.0),
      height_(0.0),
      shader_(0),
      fresnel_(0.2, 0.2, 0.2) {
  setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget() {
  if (initialized_) {
    glDeleteTextures(1, &specular_map_);
    glDeleteTextures(1, &diffuse_map_);
  }
}

bool GLWidget::LoadModel(const QString &filename) {
  std::string file = filename.toUtf8().constData();
  size_t pos = file.find_last_of(".");
  std::string type = file.substr(pos + 1);

  std::unique_ptr<data_representation::TriangleMesh> mesh =
      std::make_unique<data_representation::TriangleMesh>();

  bool res = false;
  if (type.compare("ply") == 0) {
    res = data_representation::ReadFromPly(file, mesh.get());
  }

  if (res) {
    mesh_.reset(mesh.release());
    camera_.UpdateModel(mesh_->min_, mesh_->max_);



    emit SetFaces(QString(std::to_string(mesh_->faces_.size() / 3).c_str()));
    emit SetVertices(
        QString(std::to_string(mesh_->vertices_.size() / 3).c_str()));

    float xs = mesh_->max_.x() - mesh_->min_.x();
    float ys = mesh_->max_.y() - mesh_->min_.y();
    float zs = mesh_->max_.z() - mesh_->min_.z();
    float size = std::max(xs, ys);
    size = std::max(size , zs);

    cellsize = size;
    Eigen::Vector3f pos(mesh_->max_.x()-xs/2,mesh_->max_.y()-ys/2,mesh_->max_.z()-zs/2);

    std::vector<int> vertsIndices;
    for(int i = 0; i < mesh_->vertices_.size() / 3; i++) {
        vertsIndices.push_back(i);
    }

    octree = Octree(5, size+size*0.1f, pos, vertsIndices);
    octree.CreateOctree(mesh_->vertices_, mesh_->normals_);

    newTriangles = mesh_->faces_;
    // TODO(students): Create / Initialize buffers.

    glGenVertexArrays(1, &meshVAO);
    glGenBuffers(1, &meshVBO);
    glGenBuffers(1, &meshEBO);
    glGenBuffers(1, &meshNormalsVBO);

    glBindVertexArray(meshVAO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
    glBufferData(GL_ARRAY_BUFFER, mesh_->vertices_.size() * sizeof(float), &mesh_->vertices_[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_->faces_.size() * sizeof(int), &mesh_->faces_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, meshNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, mesh_->normals_.size() * sizeof(float), &mesh_->normals_[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

     // END.

    return true;
  }

  return false;
}

bool GLWidget::LoadSpecularMap(const QString &dir) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, specular_map_);
  bool res = LoadCubeMap(dir);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  return res;
}

bool GLWidget::LoadDiffuseMap(const QString &dir) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, diffuse_map_);
  bool res = LoadCubeMap(dir);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  return res;
}

void GLWidget::initializeGL() {

  glewInit();

  printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

  glEnable(GL_NORMALIZE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  //resizeGL(1024,720);
  glGenTextures(1, &specular_map_);
  glGenTextures(1, &diffuse_map_);

  reflection_program_ = std::make_unique<QOpenGLShaderProgram>();
  brdf_program_ = std::make_unique<QOpenGLShaderProgram>();
  sky_program_ = std::make_unique<QOpenGLShaderProgram>();
  aogp_program_ = std::make_unique<QOpenGLShaderProgram>();
  ssao_program_ = std::make_unique<QOpenGLShaderProgram>();
  blur_program_ = std::make_unique<QOpenGLShaderProgram>();
  ssao3D_program_ = std::make_unique<QOpenGLShaderProgram>();
  sao_program_ = std::make_unique<QOpenGLShaderProgram>();

  bool res =
      LoadProgram(kReflectionVertexShaderFile, kReflectionFragmentShaderFile,
                  reflection_program_.get());
  res = res && LoadProgram(kBRDFVertexShaderFile, kBRDFFragmentShaderFile,
                           brdf_program_.get());
  res = res && LoadProgram(kSkyVertexShaderFile, kSkyFragmentShaderFile,
                           sky_program_.get());
  res = res && LoadProgram(kGeometryPassVertexShaderFile, kGeometryPassFragmentShaderFile,
                           aogp_program_.get());
  res = res && LoadProgram(kSSAOVertexShaderFile, kSSAOFragmentShaderFile,
                           ssao_program_.get());
  res = res && LoadProgram(kSSAOVertexShaderFile, kBlurFragmentShaderFile,
                           blur_program_.get());
  res = res && LoadProgram(kSSAOVertexShaderFile, k3DSSAOFragmentShaderFile,
                           ssao3D_program_.get());
  res = res && LoadProgram(kSSAOVertexShaderFile, kSAOFragmentShaderFile,
                           sao_program_.get());
  if (!res) exit(0);

  initialized_ = true;


  // Temporal - Skybox buffer initialization
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);

  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  // Screen plane VAO
  glGenVertexArrays(1, &screenQuadVAO);
  glGenBuffers(1, &screenQuadVBO);

  glBindVertexArray(screenQuadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  // Initialize buffers for AO
  glGenFramebuffers(1, &gFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, gFBO);

  // Positions
  glGenTextures(1, &gPosition);
  glBindTexture(GL_TEXTURE_2D, gPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

  // Normals
  glGenTextures(1, &gNormal);
  glBindTexture(GL_TEXTURE_2D, gNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

  //Attach buffers
  unsigned int attach[2] ={GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2, attach);

  glGenRenderbuffers(1, &depthRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, int(this->width_), int(this->height_));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

  // check if framebuffer is complete (for some reason always fails in init, needs to be repeat it at runing)
  int result;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);

  std::cout << result << std::endl;
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
         std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // SSAO Buffer and texture
  glGenFramebuffers( 1, &ssaoFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

  glGenTextures(1, &ssao);
  glBindTexture(GL_TEXTURE_2D, ssao);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao, 0);

  glGenFramebuffers( 1, &ssaoFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

  //Blur buufer and texture
  glGenFramebuffers( 1, &blurFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);

  glGenTextures(1, &blurSSAO);
  glBindTexture(GL_TEXTURE_2D, blurSSAO);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurSSAO, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
         std::cout << "SSAO Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Create Kernel/Samples and noise vectors for SSAO
  CreateSSAOSamples();



  numElements_ = 1;

  translations.resize(static_cast<size_t>(1) * 3);
  translations[0] = 0;
  translations[1] = 0;
  translations[2] = 0;

  timer.start();
  frameCount = 0;

  drawOctree = false;
  drawByLvl = false;

  useMean = true;
  useQuadrics = false;

  kernelSize_ = 64;
  intensity_ = 2;
  sampleRadius_ = 0.2;
  sampleRange_ = 0.2;
}

void GLWidget::resizeGL(int w, int h) {
  if (h == 0) h = 1;
  width_ = w;
  height_ = h;

  camera_.SetViewport(0, 0, w, h);
  camera_.SetProjection(kFieldOfView, kZNear, kZFar);

  // Initialize buffers for AO
  glGenFramebuffers(1, &gFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, gFBO);

  // Positions
  glGenTextures(1, &gPosition);
  glBindTexture(GL_TEXTURE_2D, gPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

  // Normals
  glGenTextures(1, &gNormal);
  glBindTexture(GL_TEXTURE_2D, gNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

  //Attach buffers
  unsigned int attach[2] ={GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2, attach);

  glGenRenderbuffers(1, &depthRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, int(this->width_), int(this->height_));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

  // check if framebuffer is complete
  int result;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);

  std::cout << result << std::endl;
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
         std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // SSAO and blur fbos
  glGenFramebuffers( 1, &ssaoFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

  glGenTextures(1, &ssao);
  glBindTexture(GL_TEXTURE_2D, ssao);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao, 0);

  glGenFramebuffers( 1, &blurFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);

  glGenTextures(1, &blurSSAO);
  glBindTexture(GL_TEXTURE_2D, blurSSAO);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width_, this->height_, 0, GL_RGB, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurSSAO, 0);

  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);

  std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
         std::cout << "SSAO Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StartRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StartZooming(event->x(), event->y());
  }
  updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  camera_.SetRotationX(event->y());
  camera_.SetRotationY(event->x());
  camera_.SafeZoom(event->y());
  updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StopRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StopZooming(event->x(), event->y());
  }
  updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Up) camera_.Zoom(-1);
  if (event->key() == Qt::Key_Down) camera_.Zoom(1);

  if (event->key() == Qt::Key_Left) camera_.Rotate(-1);
  if (event->key() == Qt::Key_Right) camera_.Rotate(1);

  if (event->key() == Qt::Key_W) camera_.Zoom(-1);
  if (event->key() == Qt::Key_S) camera_.Zoom(1);

  if (event->key() == Qt::Key_A) camera_.Rotate(-1);
  if (event->key() == Qt::Key_D) camera_.Rotate(1);

  if (event->key() == Qt::Key_R) {
    reflection_program_.reset();
    reflection_program_ = std::make_unique<QOpenGLShaderProgram>();
    LoadProgram(kReflectionVertexShaderFile, kReflectionFragmentShaderFile,
                reflection_program_.get());

    brdf_program_.reset();
    brdf_program_ = std::make_unique<QOpenGLShaderProgram>();
    LoadProgram(kBRDFVertexShaderFile, kBRDFFragmentShaderFile,
                brdf_program_.get());

    sky_program_.reset();
    sky_program_ = std::make_unique<QOpenGLShaderProgram>();
    LoadProgram(kSkyVertexShaderFile, kSkyFragmentShaderFile,
                sky_program_.get());
  }

  updateGL();
}

void GLWidget::paintGL() {
  glClearColor(0.4f, 0.8f, 0.5f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  if (initialized_) {
    camera_.SetViewport();

    Eigen::Matrix4f projection = camera_.SetProjection();
    Eigen::Matrix4f view = camera_.SetView();
    Eigen::Matrix4f model = camera_.SetModel();

    Eigen::Matrix4f t = view * model;
    Eigen::Matrix3f normal;
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) normal(i, j) = t(i, j);

    normal = normal.inverse().transpose();

    if (mesh_ != nullptr) {
      GLint projection_location, view_location, model_location,
          normal_matrix_location, specular_map_location, diffuse_map_location,
          fresnel_location, numInstances, roughness, metalness, kernelSize_location,
          intensity_location, sampleRadius_location, sampleRange_location;

      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      //SSAO - Passes
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      //For some unknown reason these needs to be done every frame
      //----------------------------------------------------------
      reflection_program_->bind();
      GLint p = reflection_program_->uniformLocation("gPosition");
      glUniform1i(p, 0);
      GLint n = reflection_program_->uniformLocation("gNormal");
      glUniform1i(n, 1);
      GLint ss = reflection_program_->uniformLocation("ssao");
      glUniform1i(ss, 3);

      ssao_program_->bind();
      p = ssao_program_->uniformLocation("gPosition");
      glUniform1i(p, 0);
      n = ssao_program_->uniformLocation("gNormal");
      glUniform1i(n, 1);
      ss = ssao_program_->uniformLocation("texNoise");

      ssao3D_program_->bind();
      p = ssao3D_program_->uniformLocation("gPosition");
      glUniform1i(p, 0);
      n = ssao3D_program_->uniformLocation("gNormal");
      glUniform1i(n, 1);
      ss = ssao3D_program_->uniformLocation("texNoise");
      glUniform1i(ss, 2);

      sao_program_->bind();
      p = sao_program_->uniformLocation("gPosition");
      glUniform1i(p, 0);
      n = sao_program_->uniformLocation("gNormal");
      glUniform1i(n, 1);
      ss = sao_program_->uniformLocation("texNoise");
      glUniform1i(ss, 2);

      blur_program_->bind();
      ss = blur_program_->uniformLocation("ao");
      glUniform1i(ss, 0);
      //------------------------------------------------------------

      //SSAO - Geometry pass ----------------------------------------------
      glBindFramebuffer(GL_FRAMEBUFFER, gFBO);

          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          aogp_program_->bind();
          projection_location = aogp_program_->uniformLocation("projection");
          view_location = aogp_program_->uniformLocation("view");
          model_location = aogp_program_->uniformLocation("model");
          normal_matrix_location = aogp_program_->uniformLocation("normal_matrix");
          numInstances = aogp_program_->uniformLocation("numInstances");
         // GLint invN =  aogp_program_->uniformLocation("inversen");

          model = camera_.SetIdentity();
          //model = model*Eigen::Scaling(10.0f);
          model.col(3).head<3>() << 0, mesh_->max_.y() - (mesh_->max_.y() - mesh_->min_.y())/2, 0;
          glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
          glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
          glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
          glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());
          glUniform1f(numInstances, float(1));
          //glUniform1i(invN, 0);

          glBindVertexArray(skyboxVAO);
          glDrawArrays(GL_TRIANGLES, 0, 36);

          model = camera_.SetModel();

          glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
          glUniform1f(numInstances, float(numElements_));
          //glUniform1i(invN, 0);

          glBindVertexArray(meshVAO);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
          glDrawElementsInstanced(GL_TRIANGLES, newTriangles.size(), GL_UNSIGNED_INT, (void*)0, numElements_*numElements_);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      // -----------------------------------------------------------
      // SSAO pass -------------------------------------------------
      glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
          glClear(GL_COLOR_BUFFER_BIT);
          GLint screenSize_location;
          if(shader_== 0) {
              ssao_program_->bind();

              for (unsigned int i = 0; i < 64; ++i) {
                  GLint sample_;
                  QString route = "samples[";
                  route.append(QChar(i));
                  route.append("]");
                  sample_ = ssao_program_->uniformLocation(route);
                  glUniform3f(sample_, ssaoSamples[i].x(), ssaoSamples[i].y(), ssaoSamples[i].z());
              }
              projection_location = ssao_program_->uniformLocation("projection");
              screenSize_location =  ssao_program_->uniformLocation("screenSize");
              kernelSize_location = ssao_program_->uniformLocation("kernelSize");
              intensity_location = ssao_program_->uniformLocation("intensity");
              sampleRadius_location = ssao_program_->uniformLocation("radius");
              sampleRange_location = ssao_program_->uniformLocation("range");

          }
          if(shader_== 2) {
              ssao3D_program_->bind();

              for (unsigned int i = 0; i < 64; ++i) {
                  GLint sample_;
                  QString route = "samples[";
                  route.append(QChar(i));
                  route.append("]");
                  sample_ = ssao3D_program_->uniformLocation(route);
                  glUniform3f(sample_, ssaoSamples[i].x(), ssaoSamples[i].y(), ssaoSamples[i].z());
              }
              projection_location = ssao3D_program_->uniformLocation("projection");
              screenSize_location =  ssao3D_program_->uniformLocation("screenSize");
              kernelSize_location = ssao3D_program_->uniformLocation("kernelSize");
              intensity_location = ssao3D_program_->uniformLocation("intensity");
              sampleRadius_location = ssao3D_program_->uniformLocation("radius");
              sampleRange_location = ssao3D_program_->uniformLocation("range");


          }
          if(shader_== 1 or shader_ == 3){
              sao_program_->bind();

              for (unsigned int i = 0; i < 64; ++i) {
                  GLint sample_;
                  QString route = "samples[";
                  route.append(QChar(i));
                  route.append("]");
                  sample_ = sao_program_->uniformLocation(route);
                  glUniform3f(sample_, ssaoSamples[i].x(), ssaoSamples[i].y(), ssaoSamples[i].z());
              }
              projection_location = sao_program_->uniformLocation("projection");
              screenSize_location =  sao_program_->uniformLocation("screenSize");
              kernelSize_location = sao_program_->uniformLocation("kernelSize");
              intensity_location = sao_program_->uniformLocation("intensity");
              sampleRadius_location = sao_program_->uniformLocation("radius");
              sampleRange_location = sao_program_->uniformLocation("range");

          }

          glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
          glUniform2f(screenSize_location, width_, height_);
          glUniform1i(kernelSize_location, kernelSize_);
          glUniform1i(intensity_location, intensity_);
          glUniform1f(sampleRadius_location, float(sampleRadius_));
          glUniform1f(sampleRange_location, float(sampleRange_));

          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, gPosition);
          glActiveTexture(GL_TEXTURE1);
          glBindTexture(GL_TEXTURE_2D, gNormal);
          glActiveTexture(GL_TEXTURE2);
          glBindTexture(GL_TEXTURE_2D, noiseTex);

          glBindVertexArray(screenQuadVAO);
          glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
          glBindVertexArray(0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      // -----------------------------------------------------------
      // SSAO - Blur pass -------------------------------------------------
      glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
          glClear(GL_COLOR_BUFFER_BIT);
          blur_program_->bind();

          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, ssao);

          glBindVertexArray(screenQuadVAO);
          glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
          glBindVertexArray(0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      // -----------------------------------------------------------
      // SSAO - For lights and testing -----------------------------
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (shader_!=1) {
        reflection_program_->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, blurSSAO);

        glBindVertexArray(screenQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
      // ------------------------------------------------------------
      } else {
        brdf_program_->bind();
        projection_location = brdf_program_->uniformLocation("projection");
        view_location = brdf_program_->uniformLocation("view");
        model_location = brdf_program_->uniformLocation("model");
        normal_matrix_location =
            brdf_program_->uniformLocation("normal_matrix");
        specular_map_location = brdf_program_->uniformLocation("specular_map");
        diffuse_map_location = brdf_program_->uniformLocation("diffuse_map");
        fresnel_location = brdf_program_->uniformLocation("fresnel");
        roughness = brdf_program_->uniformLocation("roughness");
        metalness = brdf_program_->uniformLocation("metalness");
        numInstances = brdf_program_->uniformLocation("numInstances");

        glUniform1f(roughness, float(roughness_));
        glUniform1f(metalness, float(metalness_));

        glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
        glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());
        glUniform1f(numInstances, float(numElements_));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, specular_map_);
        glUniform1i(specular_map_location, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, diffuse_map_);
        glUniform1i(diffuse_map_location, 1);

        glUniform3f(fresnel_location, fresnel_[0], fresnel_[1], fresnel_[2]);

        // TODO(students): Implement model rendering.
        //std::cout << "Implement model rendering."<< std::endl;

        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
        //glDrawElements(GL_TRIANGLES, mesh_->faces_.size(), GL_UNSIGNED_INT, (void*)0);
        //glDrawElementsInstanced(GL_TRIANGLES, mesh_->faces_.size(), GL_UNSIGNED_INT, (void*)0, numElements_*numElements_);
        glDrawElementsInstanced(GL_TRIANGLES, newTriangles.size(), GL_UNSIGNED_INT, (void*)0, numElements_*numElements_);
        // END.
      }

      if(drawOctree) octree.drawOctree();
      if(drawByLvl) octree.drawOctreeByLvl(LODlvl);
    }

    /*model = camera_.SetIdentity();

    sky_program_->bind();
    GLint projection_location = sky_program_->uniformLocation("projection");
    GLint view_location = sky_program_->uniformLocation("view");
    GLint model_location = sky_program_->uniformLocation("model");
    GLint normal_matrix_location =
        sky_program_->uniformLocation("normal_matrix");
    GLint specular_map_location = sky_program_->uniformLocation("specular_map");

    glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
    glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());



    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, specular_map_);
    glUniform1i(specular_map_location, 0);

    // TODO(students): implement the rendering of a bounding cube displaying the
    // environment map.
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);*/
    // END.


    ++frameCount;

    if (timer.elapsed() >= 1000)
    {
         double fps = frameCount / ((double)timer.restart()/1000.0);
         emit SetFramerate(QString(std::to_string(fps).c_str()));
         frameCount = 0;
    }
  }

}

void GLWidget::SetReflection(bool set) {
  if (set) shader_ = 0;
  updateGL();
}

void GLWidget::SetBRDF(bool set) {
  if (set) shader_ = 1;
  updateGL();
}

void GLWidget::Set3DSSAO(bool set) {
  if (set) shader_ = 2;
  updateGL();
}

void GLWidget::SetSAO(bool set) {
  if (set) shader_ = 3;
  updateGL();
}

void GLWidget::SetFresnelR(double r) {
  fresnel_[0] = r;
  updateGL();
}

void GLWidget::SetFresnelG(double g) {
  fresnel_[1] = g;
  updateGL();
}

void GLWidget::SetFresnelB(double b) {
  fresnel_[2] = b;
  updateGL();
}

void GLWidget::SetNumElements(int n) {
   numElements_ = n;

   translations.resize(static_cast<size_t>(n*n) * 3);

   float xs = mesh_->max_.x() - mesh_->min_.x();
   float ys = mesh_->max_.y() - mesh_->min_.y();
   unsigned int index = 0;
   float offsetx = 0;
   float offsety = 0;
   if(n % 2 == 0){
       offsetx = xs/n;
       offsety = ys/n;
   }

   for(int x = 0; x < n; x = x+1) {
       for(int y = 0; y < n; y = y+1) {

           float ox = (x - n/2)*(xs/n)+offsetx/2;
           float oy = (y - n/2)*(ys/n)+offsety/2;

           translations[index++] = ox;
           translations[index++] = oy;
           translations[index++] = 0;
       }
   }

   glGenBuffers(1, &offsetVBO);
   glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * translations.size(), &translations[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindVertexArray(meshVAO);
   glEnableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, offsetVBO); // this attribute comes from a different vertex buffer
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

   updateGL();
}

void GLWidget::SetRoughness(double r) {
  roughness_ = r;
  updateGL();
}

void GLWidget::SetMetalness(double m) {
  metalness_ = m;
  updateGL();
}

void GLWidget::CreateSSAOSamples() {

  // generate random floats between 0.0 and 1.0
  std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
  std::default_random_engine generator;

  int numSamples = 64;
  int numNoiseSamples = 16; //4x4

  for(unsigned int i = 0; i < numSamples; i++) {

    Eigen::Vector3f sample(
                randomFloats(generator) * 2.0f - 1.0f,      //Range -1 to 1
                randomFloats(generator) * 2.0f - 1.0f,
                randomFloats(generator)                     //Upper hemisphere, better for 3DSSAO
    );

    //sample = sample.normalized();
    //sample *= randomFloats(generator);

    ssaoSamples.push_back(sample);
  }

  for(unsigned int i = 0; i < numNoiseSamples; i++) {

    Eigen::Vector3f sample(
                randomFloats(generator) * 2.0f - 1.0f,      //Range -1 to 1
                randomFloats(generator) * 2.0f - 1.0f,
                0.0f
    );

    ssaoNoise.push_back(sample);
  }

  glGenTextures(1, &noiseTex);
  glBindTexture(GL_TEXTURE_2D, noiseTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void GLWidget::SetLODLevel(int l) {

    LODlvl = l+2;

    newTriangles.clear();
    meanVertices.clear();
    meanNormals.clear();
    quadricVertices.clear();
    quadricNormals.clear();

    std::vector<int> vr(mesh_->vertices_.size()/3);

    octree.getVertsRefByDepth(LODlvl, &vr, &meanVertices, &meanNormals);

    std::vector<Eigen::MatrixXd> quadrics(meanVertices.size()/3, Eigen::Matrix4d::Zero());
    quadricVertices.resize(meanVertices.size());

    std::cout << meanVertices.size()/3 << std::endl;
    for(int j = 0; j < mesh_->faces_.size(); j=j+3) {
        int ta = vr[mesh_->faces_[j]];
        int tb = vr[mesh_->faces_[j+1]];
        int tc = vr[mesh_->faces_[j+2]];

        if(ta!=tb && tb!=tc && ta!=tc) {
            //push new triangle abc
            newTriangles.push_back(ta);
            newTriangles.push_back(tb);
            newTriangles.push_back(tc);
        }

        //if(ta==tb && tb==tc && ta==tc) {
            Eigen::Vector3d v1(mesh_->vertices_[mesh_->faces_[j] * 3],
                               mesh_->vertices_[mesh_->faces_[j] * 3 + 1],
                               mesh_->vertices_[mesh_->faces_[j] * 3 + 2]);
            Eigen::Vector3d v2(mesh_->vertices_[mesh_->faces_[j + 1] * 3],
                               mesh_->vertices_[mesh_->faces_[j + 1] * 3 + 1],
                               mesh_->vertices_[mesh_->faces_[j + 1] * 3 + 2]);
            Eigen::Vector3d v3(mesh_->vertices_[mesh_->faces_[j + 2] * 3],
                               mesh_->vertices_[mesh_->faces_[j + 2] * 3 + 1],
                               mesh_->vertices_[mesh_->faces_[j + 2] * 3 + 2]);

            Eigen::Vector3d v1v2 = v2 - v1;
            Eigen::Vector3d v1v3 = v3 - v1;
            Eigen::Vector3d normal = v1v2.cross(v1v3);

            if (normal.norm() < 0.00001) {
              normal = Eigen::Vector3d(0.0, 0.0, 0.0);
            } else {
              normal.normalize();
            }

            double cx = (v1.x() + v2.x() + v3.x())/3.0f;
            double cy = (v1.y() + v2.y() + v3.y())/3.0f;
            double cz = (v1.z() + v2.z() + v3.z())/3.0f;

            double a = normal.x();
            double b = normal.y();
            double c = normal.z();
            double d = -a*cx -b*cy - c*cz;

            Eigen::Matrix4d q;
            q << a*a, a*b, a*c, a*d,
                 a*b, b*b, b*c, b*d,
                 a*c, c*b, c*c, c*d,
                 a*d, d*b, d*c, d*d;

        if(ta==tc && ta!=tb) {
            quadrics[ta] += q;
            quadrics[tb] += q;
        }
        if(ta==tb && ta!=tc){
            quadrics[ta] += q;
            quadrics[tc] += q;
        }
        if(tb==tc && tb!=ta) {
            quadrics[tb] += q;
            quadrics[ta] += q;
        }
        if(ta==tb && tb==tc && ta==tc) quadrics[ta] += q;
        if(ta!=tb && tb!=tc && ta!=tc){
            quadrics[ta] += q;
            quadrics[tb] += q;
            quadrics[tc] += q;
        }



        //}

    }

    //Solve linear squares system
    for(int i = 0; i < quadrics.size(); i++) {
        quadrics[i](3,0) = 0.0f;
        quadrics[i](3,1) = 0.0f;
        quadrics[i](3,2) = 0.0f;
        quadrics[i](3,3) = 1.0f;

        Eigen::Vector4d rs(0.0f,0.0f,0.0f,1.0f);
        //Eigen::VectorXd sol = (quadrics[i].transpose() * quadrics[i]).ldlt().solve(quadrics[i].transpose() * rs);
        //Eigen::VectorXd sol = quadrics[i].colPivHouseholderQr().solve(rs);
        //Eigen::VectorXd sol = quadrics[i].fullPivHouseholderQr().solve(rs);
        Eigen::VectorXd sol = quadrics[i].bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(rs);

        quadricVertices[i*3] = sol.x();
        quadricVertices[i*3+1] = sol.y();
        quadricVertices[i*3+2] = sol.z();
    }

    this->quadricNormals.resize(this->meanNormals.size());
    data_representation::ComputeVertexNormals(this->quadricVertices, newTriangles, &this->quadricNormals);

    // TODO(students): Create / Initialize buffers.

    if(useMean) UseMean(useMean);
    if(useQuadrics) UseQuadrics(useQuadrics);


    emit SetFaces(QString(std::to_string(newTriangles.size()/3).c_str()));
    emit SetVertices(
        QString(std::to_string(meanVertices.size()/3).c_str()));

    updateGL();
}

void GLWidget::DrawOctree(bool d) {
    drawOctree = d;

    if(d) {
        drawByLvl = !d;
        emit SetDrawOctreeByLvl(false);
    }

    updateGL();
}

void GLWidget::DrawOctreeByLvl(bool d) {
    drawByLvl = d;

    if(d) {
        drawOctree = !d;
        emit SetDrawOctree(false);
    }

    updateGL();
}

void GLWidget::UseMean(bool b) {
    useMean = b;

    if(b) {
        useQuadrics !=b;
        emit SetQuadrics(false);

        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, meanVertices.size() * sizeof(float), &meanVertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, newTriangles.size() * sizeof(int), &newTriangles[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, meshNormalsVBO);
        glBufferData(GL_ARRAY_BUFFER, meanNormals.size() * sizeof(float), &meanNormals[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    }

    if(!b) UseQuadrics(true);

    updateGL();
}

void GLWidget::UseQuadrics(bool b) {
    useQuadrics = b;

    if(b) {
        useMean !=b;
        emit SetMean(false);

        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, quadricVertices.size() * sizeof(float), &quadricVertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, newTriangles.size() * sizeof(int), &newTriangles[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, meshNormalsVBO);
        glBufferData(GL_ARRAY_BUFFER, quadricNormals.size() * sizeof(float), &quadricNormals[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    if(!b) UseMean(true);

    updateGL();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//SSAO - Parameters
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void GLWidget::SetKernelSize(double k) {
    kernelSize_ = int(k);

    updateGL();
}

void GLWidget::SetAOIntensity(double i) {
    intensity_ = int(i);

    updateGL();
}

void GLWidget::SetSampleRadius(double r) {
    sampleRadius_ = r;

    updateGL();
}

void GLWidget::SetRange(double r) {
    sampleRange_ = r;

    updateGL();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
