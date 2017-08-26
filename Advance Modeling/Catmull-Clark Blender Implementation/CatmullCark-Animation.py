import bpy 
import numpy
import math
import mathutils
from mathutils import Vector
from time import time

#Constants------------------------
 #Retrieve the active object (the last one selected)
ob = bpy.data.scenes['Scene'].objects.active

# Check that it is indeed a mesh
if not ob or ob.type != 'MESH': 
    BPyMessages.Error_NoMeshActive() 
           
# If we are in edit mode, return to object mode
bpy.ops.object.mode_set(mode='OBJECT')

# Retrieve mesh data
cube = ob.data

# Number of catmullclark iterations
N_ITERATIONS = 3;
#---------------------------------

def r(a):
    return int(a*1000+0.5)/1000.0
  
def r6(a):
    return int(a*1000000+0.5)/1000000.0
  
def compEdge(f1, f2):
  
  compV = 0
  
  for v in f1.vertices:
    if v in f2.vertices:
      compV += 1
      
  if compV>=2:
    return True
  
  return False
    
def recShell(me, face, visit):
  
  visit.append(face)
  neighP = []
  
  for p in me.polygons:
    if (p not in neighP) and compEdge(face, p) and (p not in visit):
      neighP.append(p)
      visit = recShell(me, p, visit)
      
  return visit
        
  
def shells(me):
  
  vistP = []
  shell = []
  
  for p in me.polygons:
    if p not in vistP:
      aux = recShell(me,p,vistP)
      vistP = list(set(vistP+aux))
      shell.append(aux)
      
  return len(shell)
      

def convexTest(f1,f2,edge,verts):
  
  t1 = Vector((0.0,0.0,0.0))
  t2 = Vector((0.0,0.0,0.0))
  
  for v in f1.vertices:
    if v not in edge.vertices:
        t1 += verts[v].co
  
  for v in f2.vertices:
    if v not in edge.vertices:
        t2 += verts[v].co
        
  t1 = t1/(len(f1.vertices)-2)
  t2 = t2/(len(f2.vertices)-2)
  
  return r6((t2-t1)*f1.normal)
   
def angleVectors(n1, n2):
  
  norm1 = math.sqrt(numpy.dot(n1, n1))
  norm2 = math.sqrt(numpy.dot(n2, n2))
    
  return math.acos(numpy.dot(n1, n2) / (norm1*norm2))

def indexOf(element, array):
   
    for i, j in enumerate(array):
        if j == element:
            return i
        
    return False
  
def edgeCount(me):
  bound = 0
  twoM = 0
  more = 0
  convex = 0
  concave = 0
  planar = 0
  
  for e in me.edges:
    nFaces = []
    for p in me.polygons:
      if e.vertices[0] in p.vertices and e.vertices[1] in p.vertices:
          iV1 = indexOf(e.vertices[0], p.vertices)
          iV2 = indexOf(e.vertices[1], p.vertices)
          if iV1==len(p.vertices)-1 and iV2==0:
              nFaces.append(p)
          elif iV2==len(p.vertices)-1 and iV1==0:
              nFaces.append(p)
          elif iV1+1==iV2 or iV1-1==iV2:
              nFaces.append(p)
              
    if len(nFaces)==1:
      bound += 1
    elif len(nFaces)==2:
      twoM += 1
      cTest = convexTest(nFaces[0],nFaces[1], e, me.vertices)
      if cTest > 0:
        concave +=1
      if cTest < 0:
        convex +=1
      if cTest == 0:
        planar += 1
    elif len(nFaces)>2:
      more += 1
     
  return bound,twoM,more,convex,concave,planar

def vertexDegree(me):
  max = 0
  min = 10000
  total = 0
  for v in range(len(me.vertices)):
    degree = 0
    for e in me.edges:
      if v in e.vertices:
        degree += 1
    total += degree
    
    if max<degree:
      max = degree
    if min>degree:
      min = degree
  
  return max, min, total/len(me.vertices)

def meshvolume(me):
   volume = 0.0
   cm = Vector((0.0,0.0,0.0))
   for f in me.polygons:
      verts = []
      faceVerts = []
      for i in range(len(f.vertices)):
          verts.append((me.vertices[f.vertices[i]].co[0], me.vertices[f.vertices[i]].co[1], 0.0))
          faceVerts.append(i)
      face = [tuple(faceVerts)]
      
      auxMesh = bpy.data.meshes.new("xy_Poly")
      auxMesh.from_pydata(verts, [], face)
      
      xy_area = auxMesh.polygons[0].area
      Nz = f.normal[2]
      
      avg_x = sum([me.vertices[f.vertices[i]].co[0] for i in range(len(f.vertices))])/len(f.vertices)
      avg_y = sum([me.vertices[f.vertices[i]].co[1] for i in range(len(f.vertices))])/len(f.vertices)
      avg_z = sum([me.vertices[f.vertices[i]].co[2] for i in range(len(f.vertices))])/len(f.vertices)
      
      aux_z = 0
      
      for i in range(len(f.vertices)):
        for j in range(len(f.vertices)):
            aux_z += me.vertices[f.vertices[i]].co[2]*me.vertices[f.vertices[j]].co[2]
      
      centroid = Vector((avg_x,avg_y,avg_z/2))     
      partial_volume = avg_z * xy_area 
      
      if Nz < 0: 
          volume -= partial_volume
          cm -= partial_volume * centroid
      if Nz > 0: 
          volume += partial_volume
          cm += partial_volume* centroid
          
   return volume,cm/volume

def processa_malla(me):
    print('Name of the mesh: %s' % (me.name))
    
    areaMe = 0
    center = Vector((0,0,0))

    for v in me.vertices:
      center += v.co
      
    for i in range(len(me.polygons)):
        areaMe = areaMe + me.polygons[i].area
        
    print("Surface area: ", r(areaMe))
    center = center/len(me.vertices)
    print("Centroid of vertices: (%f,%f,%f)" % (r(center[0]), r(center[1]), r(center[2])))
    
    maxD, minD, avgD = vertexDegree(me)
    
    print("Maximum vertex valence: %d Minimum vertex valence: %d Average valence: %f" % (maxD, minD, r(avgD)))
    
    bound,twoM,more,convex,concave,planar = edgeCount(me)
    
    print("Edges: %d Boundary: %d 2-manifold: %d More-than-two: %d" % (len(me.edges),bound, twoM, more))
    print("Convex edges: %d Concave: %d Planar: %d Other: %d" % (convex, concave, planar, len(me.edges)-(convex+concave+planar)))
    
    f = len(me.polygons)
    v = len(me.vertices)
    e = len(me.edges)
    s = shells(me)    
    h = s - (f+v-e)/2
    
    print("Euler: F=%d, V=%d, E=%d, R=0, S=%d, H=%d" % (f, v, e, s,h))
    
    volume,centerM = meshvolume(me)
    
    print("Centre of mass: (%f,%f,%f)" % (r(centerM[0]), r(centerM[1]), r(centerM[2])))  
    print("Volume: ", r(volume))
    
    print("\nEnd of Data for mesh "+me.name+"\n\n")
    
def compFace(e1, e2, me):
    
    for p in me.polygons:
        if e1.vertices[0] in p.vertices and e1.vertices[1] in p.vertices:
             if e2.vertices[0] in p.vertices and e2.vertices[1] in p.vertices:
                return True, p
    return False, p

def edgeIndex(v1, v2, me):
    
    for e in me.edges:
        if e.vertices[0] == v1 and e.vertices[1] == v2:
            return indexOf(e, me.edges)
        if e.vertices[0] == v2 and e.vertices[1] == v1:
            return indexOf(e, me.edges)

def catmullClarkOneStep(me, t):
    
    faceVerts = []
    edgeVerts = []
    edgeVerts2 = []
    vertVerts = []
    vertVerts2 = []
    
    for p in me.polygons:
        faceVerts.append(p.center)
    
    for e in me.edges:
        nFaces = []
        for p in me.polygons:
            if e.vertices[0] in p.vertices and e.vertices[1] in p.vertices:
              iV1 = indexOf(e.vertices[0], p.vertices)
              iV2 = indexOf(e.vertices[1], p.vertices)
              if iV1==len(p.vertices)-1 and iV2==0:
                  nFaces.append(indexOf(p, me.polygons))
              elif iV2==len(p.vertices)-1 and iV1==0:
                  nFaces.append(indexOf(p, me.polygons))
              elif iV1+1==iV2 or iV1-1==iV2:
                  nFaces.append(indexOf(p, me.polygons))
                      
        vertE = (me.vertices[e.vertices[0]].co + me.vertices[e.vertices[1]].co + faceVerts[nFaces[0]] + faceVerts[nFaces[1]])/4
        
        midE = (me.vertices[e.vertices[0]].co+me.vertices[e.vertices[1]].co)/2
        
        edgeVerts2.append((1-t)*midE+t*vertE)
        
        edgeVerts.append(vertE)
        
    for v in me.vertices:
        nFaces = []
        for p in me.polygons:
            if indexOf(v, me.vertices) in p.vertices:
                nFaces.append(indexOf(p, me.polygons))
                
        centerF = Vector((0,0,0))
        for n in nFaces:
            centerF += faceVerts[n]
        centerF = centerF/len(nFaces)
        
        degree = 0
        edgesMP = Vector((0,0,0))
        for e in me.edges:
          if indexOf(v, me.vertices) in e.vertices:
            degree += 1
            edgesMP += (me.vertices[e.vertices[0]].co+me.vertices[e.vertices[1]].co)/2
            
        edgesMP = edgesMP/degree
        
        newV = (centerF+2*edgesMP+(degree-3)*v.co)/degree
        
        vertVerts.append(newV)
        
        vertVerts2.append((1-t)*v.co+t*newV)
        
    newVerts = vertVerts2+edgeVerts2+faceVerts
    newFaces = []                                                      
    for p in me.polygons:
        for v in range(len(p.vertices)):
            if v == 0:
                ie1 = edgeIndex(p.vertices[v],p.vertices[v+1],me)
                ie2 = edgeIndex(p.vertices[v],p.vertices[len(p.vertices)-1],me)
                iFace = indexOf(p, me.polygons)
                newFace = (p.vertices[v], indexOf(edgeVerts2[ie1], newVerts), indexOf(faceVerts[iFace], newVerts), indexOf(edgeVerts2[ie2], newVerts))
            elif v == len(p.vertices)-1:
                ie1 = edgeIndex(p.vertices[v],p.vertices[0],me)
                ie2 = edgeIndex(p.vertices[v],p.vertices[v-1],me)
                iFace = indexOf(p, me.polygons)
                newFace = (p.vertices[v], indexOf(edgeVerts2[ie1], newVerts), indexOf(faceVerts[iFace], newVerts), indexOf(edgeVerts2[ie2], newVerts))
            else:
                ie1 = edgeIndex(p.vertices[v],p.vertices[v+1],me)
                ie2 = edgeIndex(p.vertices[v],p.vertices[v-1],me)
                iFace = indexOf(p, me.polygons)
                newFace = (p.vertices[v], indexOf(edgeVerts2[ie1], newVerts), indexOf(faceVerts[iFace], newVerts), indexOf(edgeVerts2[ie2], newVerts))
            newFaces.append(newFace)
            
    for i in range(len(newVerts)):
        newVerts[i] = (newVerts[i][0], newVerts[i][1], newVerts[i][2])
    
    auxMesh = bpy.data.meshes.new("subdiv")
    auxMesh.from_pydata(newVerts, [], newFaces)  
    auxMesh.update()
    
    return auxMesh
        
        
def catmullClark(me, t, n):
    aux = me
    for i in range(n):
        aux=catmullClarkOneStep(aux,t) 
    mesh=aux      
    return mesh   
                 
def frameHandler(scene):
    start = scene.frame_start
    end = scene.frame_end
    num = end-start
    curr = scene.frame_current
    time = float(curr-start)/num #time in [0,1]
    
    ob.data = catmullClark(cube, time, N_ITERATIONS)


"""# Retrieve the active object (the last one selected)
ob = bpy.data.scenes['Scene'].objects.active

# Check that it is indeed a mesh
if not ob or ob.type != 'MESH': 
    BPyMessages.Error_NoMeshActive() 
           
# If we are in edit mode, return to object mode
bpy.ops.object.mode_set(mode='OBJECT')

# Retrieve the mesh data
mesh = ob.data 
    
# Get current time
t = time()

# Function that does all the work
#processa_malla(mesh)

ob.data = catmullClark(mesh, 1.0, 3) """

# get the current scene
scn = bpy.context.scene 

# assign new duration value
scn.frame_end = 240

bpy.app.handlers.frame_change_pre.append(frameHandler)

# Report performance...
#print("Script took %6.2f secs.\n\n"%(time()-t))
