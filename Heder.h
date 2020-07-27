#pragma once
//#include <GL/glut.h>

#include<iostream>
#include<math.h>
#include<ctime>
#include<vector>
#include<algorithm>
#include<windows.h>

#include"Vertex.h"
#include"Edge.h"
#include"Face.h"
#include"HalfEdge.h"
#include"BaseMesh.h"
#include"iterators.h"

//���ڿ�͸�
#define WIDTH 800
#define HEIGHT 800

//��ľ���
#define ACCURACY 10000.0

//������ɵ�ĸ���
#define VERTEXNUM 20

//voronoiMesh�ı߽�
#define BORDER 10

using namespace MeshLib;
using namespace std;

typedef CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> Mesh;
typedef VertexEdgeIterator<CVertex, CEdge, CFace, CHalfEdge> veIter;
typedef VertexFaceIterator<CVertex, CEdge, CFace, CHalfEdge> vfIter;
typedef FaceVertexIterator<CVertex, CEdge, CFace, CHalfEdge> fvIter;
typedef FaceEdgeIterator<CVertex, CEdge, CFace, CHalfEdge> feIter;
typedef MeshVertexIterator<CVertex, CEdge, CFace, CHalfEdge> mvIter;
typedef MeshEdgeIterator<CVertex, CEdge, CFace, CHalfEdge> meIter;
typedef MeshFaceIterator<CVertex, CEdge, CFace, CHalfEdge> mfIter;


class MyPoint2 {
public:
	double x;
	double y;
	MyPoint2() {}
	MyPoint2(double x, double y) {
		this->x = x;
		this->y = y;
	}
};