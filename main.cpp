#include"Heder.h"
#include "GL.H"
#include "glut.h"
#include "GLAUX.H"


bool key1 = true;
bool key2 = false;
bool key3 = true;
bool key4 = false;

double width = WIDTH;
double height = HEIGHT;

Mesh* pDelaunayMesh;
Mesh* pVoronoiMesh;

int delaunayVertexId = 0;
int delaunayFaceId = 0;
int voronoiVertexId = 0;
int voronoiFaceId = 0;

//�����洢DelaunayMesh�����е�
vector<CVertex*> allVertex;

//�����洢ConvexHull�㷨�����İ�ΧȦ�ĵ�
vector<CVertex*> vecConvexHull;

//�����洢͹���ڲ��ĵ�
vector<CVertex*> toBeAdd;

//�����洢face��Ӧ�����ԲԲ��
map<CFace*, MyPoint2> mpFaceCenter;



//�������ʼ��Vertex
void initVertex();

//�������򣬴�С������x�����С������x������ͬ����y�����С����
void insertSort();

//������������0��ʾ��oa��obΪ��ʱ����ת
GLfloat cross(CVertex* o, CVertex* a, CVertex* b);

//Andrew's Monotone Chain
void ConvexHull();

//����Բ�ģ�����Ϊ������
MyPoint2 getCenter(vector<CVertex*> vec);

//����Բ�ģ�����Ϊһ����
MyPoint2 getTriCenter(CFace* pFace);

//��Face����Ӧ��Բ��Vertex����mpFaceCenter��
void addMpFaceCenter(CFace* pFace);

//��Face����Ӧ��Բ��Vertex��mpFaceCenter��ɾ��
void delMpFaceCenter(CFace* pFace);

//Euclidean Distance��ƽ��
double eDistance(double x1, double y1, double x2, double y2);

//�ж�һ�����Ƿ���һ������������Բ��
bool isCircleContain(CFace* pFace, CVertex* pVertex);

//����ConvexHull���ɵĵ㼯��ʼ��Delaunayͼ
void initDelaunayMesh();

//�ж��Ƿ�ΪDelaunayTriangle
bool isDelaunayTriangle();

//Delaunay Triangulation
void DelaunayTriangulation();

//VoronoiDiagram�㷨
void VoronoiDiagram();

//����
void help();

//���Ƶ�
void drawVertex(Mesh* pMesh);

//���Ʊ�
void drawEdge(Mesh* pMesh);

//������
void drawFace(Mesh* pMesh);

//OpenGLͼ�����
void renderScene();

//���ڸı�
void changeSize(int w, int h);

//��Ӧ����¼�
void mouseClick(int button, int state, int x, int y);

//��Ӧ�����¼�
void keyBoardFunc(unsigned char key, int x, int y);

//��ʼ��OpenGL
void init_openGL(int argc, char *argv[]);



//�������ʼ��Vertex
void initVertex() {
	CVertex* tmp = NULL;
	while (delaunayVertexId < VERTEXNUM) {
		tmp = (*pDelaunayMesh).createVertex(delaunayVertexId++);

		tmp->point().coord()[0] = (rand() % (int)(2 * ACCURACY) - ACCURACY) / ACCURACY;
		tmp->point().coord()[1] = (rand() % (int)(2 * ACCURACY) - ACCURACY) / ACCURACY;
		allVertex.push_back(tmp);
	}
}

//�������򣬴�С������x�����С������x������ͬ����y�����С����
void insertSort() {
	int i, j;
	CVertex* tmp = NULL;
	for (i = 0; i < allVertex.size(); i++) {
		tmp = allVertex[i];
		j = i - 1;
		while (j >= 0 && allVertex[j]->point().coord()[0] >= tmp->point().coord()[0]) {
			if (allVertex[j]->point().coord()[0] == tmp->point().coord()[0]
				&& allVertex[j]->point().coord()[1] <= tmp->point().coord()[1]) {
				break;
			}
			allVertex[j + 1] = allVertex[j];
			j--;
		}
		allVertex[j + 1] = tmp;
	}
}

//������������0��ʾ��oa��obΪ��ʱ����ת
GLfloat cross(CVertex* o, CVertex* a, CVertex* b) {
	return (a->point().coord()[0] - o->point().coord()[0])
		*(b->point().coord()[1] - o->point().coord()[1])
		- (a->point().coord()[1] - o->point().coord()[1])
		*(b->point().coord()[0] - o->point().coord()[0]);
}

//Andrew's Monotone Chain
void ConvexHull() {
	//��Vertex�Ķ���id����
	insertSort();

	//��¼��Χ��ĸ���
	int m = 0;
	int size = allVertex.size();

	//�����ң��ó��°벿�ְ�Χ��
	for (int i = 0; i < size; i++) {
		while (m >= 2 && cross(vecConvexHull[m - 2], vecConvexHull[m - 1], allVertex[i]) < 0) {
			vecConvexHull.pop_back();
			m--;
		}
		vecConvexHull.push_back(allVertex[i]);
		m++;
	}

	//���ҵ��󣬵ó��ϰ벿�ְ�Χ�㣬��洢��һ�����id
	for (int i = size - 2; i >= 0; i--) {
		while (cross(vecConvexHull[m - 2], vecConvexHull[m - 1], allVertex[i]) < 0) {
			vecConvexHull.pop_back();
			m--;
		}
		vecConvexHull.push_back(allVertex[i]);
		m++;
	}

	vecConvexHull.pop_back();
}

//����Բ�ģ�����Ϊ������
MyPoint2 getCenter(vector<CVertex*> vec) {
	//������Բ��Բ��
	double x0 = vec[0]->point().coord()[0];
	double y0 = vec[0]->point().coord()[1];
	double x1 = vec[1]->point().coord()[0];
	double y1 = vec[1]->point().coord()[1];
	double x2 = vec[2]->point().coord()[0];
	double y2 = vec[2]->point().coord()[1];

	double a0 = (x0 + x1) / 2;
	double b0 = (y0 + y1) / 2;
	double a1 = (x2 + x1) / 2;
	double b1 = (y2 + y1) / 2;

	//��������
	double x;
	double y;


	if (y0 != y1 && y1 != y2) {
		double k1 = -(x0 - x1) / (y0 - y1);
		double c1 = b0 - k1 * a0;
		double k2 = -(x1 - x2) / (y1 - y2);
		double c2 = b1 - k2 * a1;

		x = (c2 - c1) / (k1 - k2);
		y = k1 * x + c1;
	}
	else if (y0 == y1) {
		x = a0;
		double k2 = -(x1 - x2) / (y1 - y2);
		double c2 = b1 - k2 * a1;
		y = k2 * x + c2;
	}
	else {
		x = a1;
		double k1 = -(x0 - x1) / (y0 - y1);
		double c1 = b0 - k1 * a0;
		y = k1 * x + c1;
	}

	MyPoint2 point2(x, y);

	return point2;
}

//����Բ�ģ�����Ϊһ����
MyPoint2 getTriCenter(CFace* pFace) {
	vector<CVertex*> vec;
	for (fvIter iter(pFace); !iter.end(); iter++) {
		vec.push_back(*iter);
	}
	return getCenter(vec);
}

//��Face����Ӧ��Բ��Vertex����mpFaceCenter��
void addMpFaceCenter(CFace* pFace) {
	MyPoint2 point2 = getTriCenter(pFace);
	mpFaceCenter.insert(pair<CFace*, MyPoint2>(pFace, point2));
}

//��Face����Ӧ��Բ��Vertex��mpFaceCenter��ɾ��
void delMpFaceCenter(CFace* pFace) {
	mpFaceCenter.erase(pFace);
}

//Euclidean Distance��ƽ��
double eDistance(double x1, double y1, double x2, double y2) {
	return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

//�ж�һ�����Ƿ���һ������������Բ��
bool isCircleContain(CFace* pFace, CVertex* pVertex) {
	MyPoint2 point2 = mpFaceCenter.at(pFace);
	fvIter iter(pFace);

	double r = eDistance(point2.x, point2.y, (*iter)->point().coord()[0], (*iter)->point().coord()[1]);
	double distance = eDistance(point2.x, point2.y, pVertex->point().coord()[0], pVertex->point().coord()[1]);

	return r > distance;
}

//����ConvexHull���ɵĵ㼯��ʼ��Delaunayͼ
void initDelaunayMesh() {
	vector<CVertex*> vecTmp = vecConvexHull;
	bool isContainOther = false;
	while (vecTmp.size() > 3) {
		int size = vecTmp.size();
		for (int i = 0; i < size; i++) {
			isContainOther = false;
			vector<CVertex*> fv;
			fv.push_back(vecTmp[i]);
			fv.push_back(vecTmp[(i + 1) % size]);
			fv.push_back(vecTmp[(i + 2) % size]);

			MyPoint2 center = getCenter(fv);
			double r = eDistance(center.x, center.y, fv[0]->point().coord()[0], fv[0]->point().coord()[1]);

			for (int j = 0; j < vecConvexHull.size(); j++) {
				CVertex* pVertexTmp = vecConvexHull[j];
				if (pVertexTmp != fv[0] && pVertexTmp != fv[1] && pVertexTmp != fv[2]
					&& r > eDistance(center.x, center.y, pVertexTmp->point().coord()[0], pVertexTmp->point().coord()[1])) {
					isContainOther = true;
					break;
				}
			}
			if (!isContainOther) {
				mpFaceCenter.insert(pair<CFace*, MyPoint2>((*pDelaunayMesh).createFace(fv, delaunayFaceId++), center));
				vecTmp.erase(vecTmp.begin() + (i + 1) % size);
				break;
			}
		}
	}

	addMpFaceCenter((*pDelaunayMesh).createFace(vecTmp, delaunayFaceId++));
}

//�ж��Ƿ�ΪDelaunayTriangle
bool isDelaunayTriangle() {
	for (mfIter iter(pDelaunayMesh); !iter.end(); iter++) {
		fvIter iter2(*iter);
		CVertex* v1 = *iter2;
		iter2++;
		CVertex* v2 = *iter2;
		iter2++;
		CVertex* v3 = *iter2;
		for (int i = 0; i < allVertex.size(); i++) {
			if (v1 != allVertex[i] && v2 != allVertex[i] && v3 != allVertex[i] && isCircleContain(*iter, allVertex[i])) {
				return false;
			}
		}
	}
	return true;
}

//Delaunay Triangulation
void DelaunayTriangulation() {
	//��ʼ��toBeAdd
	for (int i = 0; i < allVertex.size(); i++) {
		int num = count(vecConvexHull.begin(), vecConvexHull.end(), allVertex[i]);
		if (num == 0) {
			toBeAdd.push_back(allVertex[i]);
		}
	}

	//�ð�Χ��������һ�㴴��������
	initDelaunayMesh();

	//�����DelaunayMesh����ӵ�
	for (int n = 0; n < toBeAdd.size(); n++) {

		//ȡ���������Բ������ǰ���Face
		vector<CFace*> vecFace;
		for (mfIter iter(pDelaunayMesh); !iter.end(); iter++) {
			if (isCircleContain(*iter, toBeAdd[n])) {
				vecFace.push_back(*iter);
			}
		}

		//ȡ����һ���õ�������Face�໥֮�䲻����ıߵĶ˵�,������ʱ��˳�����vecVerTmp��
		vector<CVertex*> vecVerTmp;
		for (int i = 0; i < vecFace.size(); i++) {
			CFace* f0 = vecFace[i];
			for (feIter iter(f0); !iter.end(); iter++) {
				CEdge* tmpEdge = *iter;
				CHalfEdge* h0 = tmpEdge->halfedge(0);
				CHalfEdge* h1 = tmpEdge->halfedge(1);
				assert(h0 != NULL);
				if (h1 == NULL) {
					vecVerTmp.push_back(h0->source());
					vecVerTmp.push_back(h0->target());
				}
				else {
					CFace* f1 = (f0 == (*pDelaunayMesh).edgeFace1(tmpEdge))
						? (*pDelaunayMesh).edgeFace2(tmpEdge)
						: (*pDelaunayMesh).edgeFace1(tmpEdge);
					vector<CFace*>::iterator tmpIter = find(vecFace.begin(), vecFace.end(), f1);
					if (tmpIter == vecFace.end()) {

						CHalfEdge* tmpHe = f0->halfedge();
						while (tmpHe != h0 && tmpHe != h1) {
							tmpHe = tmpHe->he_next();
						}
						vecVerTmp.push_back(tmpHe->source());
						vecVerTmp.push_back(tmpHe->target());
					}
				}
			}
		}

		//ɾ�����е�һ���õ���Face
		for (int i = 0; i < vecFace.size(); i++) {
			delMpFaceCenter(vecFace[i]);
			(*pDelaunayMesh).deleteFace(vecFace[i]);
		}

		//ÿ��������ߵ������˵��뵱ǰ���γ��µ�Face
		for (int i = 0; i < vecVerTmp.size(); i += 2) {
			vector<CVertex*> tmp;
			tmp.push_back(toBeAdd[n]);
			tmp.push_back(vecVerTmp[i]);
			tmp.push_back(vecVerTmp[i + 1]);
			addMpFaceCenter((*pDelaunayMesh).createFace(tmp, delaunayFaceId++));
		}
	}
}

//VoronoiDiagram�㷨
void VoronoiDiagram() {
	ConvexHull();
	DelaunayTriangulation();

	(*pDelaunayMesh).labelBoundary();

	//��DelaunayMesh�е�Face��VoronoiMesh�еĵ��Ӧ����
	map<CFace*, CVertex*> mpFaceVertex;
	for (mfIter iter(pDelaunayMesh); !iter.end(); iter++) {
		CVertex* tmp = (*pVoronoiMesh).createVertex(voronoiVertexId++);
		tmp->point().coord()[0] = mpFaceCenter.at(*iter).x;
		tmp->point().coord()[1] = mpFaceCenter.at(*iter).y;

		mpFaceVertex.insert(pair<CFace*, CVertex*>(*iter, tmp));
	}

	//����DelaunayMesh�а���toBeAdd��һ�������Face���õ�VoronoiMesh�еĵ��Ӧ�Ĳ�����Face
	for (int i = 0; i < toBeAdd.size(); i++) {
		vector<CVertex*> vecTmp;

		for (vfIter iter(toBeAdd[i]); !iter.end(); iter++) {
			vecTmp.push_back(mpFaceVertex.at(*iter));
		}

		(*pVoronoiMesh).createFace(vecTmp, voronoiFaceId++);
	}

	//�õ�λ��Mesh��Ե��Edge�����ɴ�ֱƽ�����ϵ���һ��
	map<CEdge*, CVertex*> mpEdgeVertex;
	for (int i = 0; i < vecConvexHull.size(); i++) {
		for (vfIter iter(vecConvexHull[i]); !iter.end(); iter++) {
			CFace* faceTmp = *iter;

			for (feIter iter(faceTmp); !iter.end(); iter++) {
				CEdge* edgeTmp = *iter;
				if (edgeTmp->boundary()) {

					CVertex* center = mpFaceVertex.at(faceTmp);

					CVertex* v0 = (*pDelaunayMesh).edgeVertex1(edgeTmp);
					CVertex* v1 = (*pDelaunayMesh).edgeVertex2(edgeTmp);

					CVertex* newVert = (*pVoronoiMesh).createVertex(voronoiVertexId++);
					mpEdgeVertex.insert(pair<CEdge*, CVertex*>(edgeTmp, newVert));

					double x = center->point().coord()[0];
					double y = center->point().coord()[1];

					double x0 = v0->point().coord()[0];
					double y0 = v0->point().coord()[1];
					double x1 = v1->point().coord()[0];
					double y1 = v1->point().coord()[1];

					if (y0 == y1) {
						newVert->point().coord()[0] = x;
						if (y > 0) {
							newVert->point().coord()[1] = BORDER;
						}
						else {
							newVert->point().coord()[1] = -BORDER;
						}
					}
					else if (x0 == x1) {
						newVert->point().coord()[1] = y;
						if (x > 0) {
							newVert->point().coord()[0] = BORDER;
						}
						else {
							newVert->point().coord()[0] = -BORDER;
						}
					}
					else {
						double k = -(x0 - x1) / (y0 - y1);
						double b = y - k * x;

						double xm = (x0 + x1) / 2.0;
						double ym = (y0 + y1) / 2.0;

						if (xm > 0 && ym>0) {
							if (k<1 && k>-1) {
								newVert->point().coord()[0] = BORDER;
								newVert->point().coord()[1] = BORDER * k + b;
							}
							else {
								newVert->point().coord()[1] = BORDER;
								newVert->point().coord()[0] = (BORDER - b) / k;
							}
						}
						else if (xm > 0 && ym < 0) {
							if (k<1 && k>-1) {
								newVert->point().coord()[0] = BORDER;
								newVert->point().coord()[1] = BORDER * k + b;
							}
							else {
								newVert->point().coord()[1] = -BORDER;
								newVert->point().coord()[0] = (-BORDER - b) / k;
							}
						}
						else if (xm < 0 && ym>0) {
							if (k<1 && k>-1) {
								newVert->point().coord()[0] = -BORDER;
								newVert->point().coord()[1] = -BORDER * k + b;
							}
							else {
								newVert->point().coord()[1] = BORDER;
								newVert->point().coord()[0] = (BORDER - b) / k;
							}
						}
						else {
							if (k<1 && k>-1) {
								newVert->point().coord()[0] = -BORDER;
								newVert->point().coord()[1] = -BORDER * k + b;
							}
							else {
								newVert->point().coord()[1] = -BORDER;
								newVert->point().coord()[0] = (-BORDER - b) / k;
							}
						}
					}
				}
			}
		}
	}

	//����DelaunayMesh�а���vecConvexHull��һ�������Face
	for (int i = 0; i < vecConvexHull.size(); i++) {
		vector<CVertex*> vecTmp;

		vfIter iter(vecConvexHull[i]);
		CFace* startFace = *iter;
		CVertex* startVertex = mpFaceVertex.at(startFace);
		for (feIter startIter(startFace); !startIter.end(); startIter++) {
			CEdge* edgeTmp = *startIter;
			if (edgeTmp->boundary()) {
				vecTmp.push_back(mpEdgeVertex.at(edgeTmp));
			}
		}
		vecTmp.push_back(startVertex);
		iter++;

		for (; !iter.end(); iter++) {
			CFace* faceTmp = *iter;
			CVertex* center = mpFaceVertex.at(faceTmp);
			assert(center != NULL);
			vecTmp.push_back(center);
			for (feIter iter2(faceTmp); !iter2.end(); iter2++) {
				CEdge* edgeTmp = *iter2;
				if (edgeTmp->boundary()) {
					vecTmp.push_back(mpEdgeVertex.at(edgeTmp));
				}
			}
		}

		(*pVoronoiMesh).createFace(vecTmp, voronoiFaceId++);
	}

	//(*pVoronoiMesh).labelBoundary();
}

//����
void help() {
	cout << "1   -  Show Delaunay Mesh" << endl;
	cout << "2   -  Show Delaunay Face" << endl;
	cout << "3   -  Show Voronoi Mesh" << endl;
	cout << "4   -  Show Voronoi Face" << endl;
	cout << "?   -  Help Info" << endl;
	cout << "Esc -  Quit" << endl;
	cout << "tips:  click screen to add vertex and redraw the diagram" << endl;
}

//���Ƶ�
void drawVertex(Mesh* pMesh) {
	glPointSize(4.0f);
	glBegin(GL_POINTS);
	for (mvIter iter(pMesh); !iter.end(); iter++) {
		glVertex2f((*iter)->point().coord()[0], (*iter)->point().coord()[1]);
	}
	glEnd();
}

//���Ʊ�
void drawEdge(Mesh* pMesh) {
	glBegin(GL_LINES);
	for (meIter iter(pMesh); !iter.end(); iter++) {
		glVertex2f((*pDelaunayMesh).edgeVertex1(*iter)->point().coord()[0], (*pDelaunayMesh).edgeVertex1(*iter)->point().coord()[1]);
		glVertex2f((*pDelaunayMesh).edgeVertex2(*iter)->point().coord()[0], (*pDelaunayMesh).edgeVertex2(*iter)->point().coord()[1]);
	}
	glEnd();
}

//������
void drawFace(Mesh* pMesh) {
	for (mfIter iter(pMesh); !iter.end(); iter++) {
		double r = rand() % 256;
		double g = rand() % 256;
		double b = rand() % 256;
		glColor3ub(r, g, b);
		glBegin(GL_POLYGON);
		for (fvIter iter2(*iter); !iter2.end(); iter2++) {
			CVertex* v = *iter2;
			glVertex2f(v->point().coord()[0], v->point().coord()[1]);
		}
		glEnd();
	}
}

//OpenGLͼ�����
void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2.0f);
	/*glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);*/
	//����DelaunayMesh�ĵ�ͱ�
	if (key1) {
		glColor3f(0.0f, 0.0f, 0.0f);

		drawVertex(pDelaunayMesh);

		glLineStipple(2, 0x0f0f);
		glEnable(GL_LINE_STIPPLE);
		drawEdge(pDelaunayMesh);
		glDisable(GL_LINE_STIPPLE);
	}

	//����DelaunayMesh��
	if (key2) {
		drawFace(pDelaunayMesh);
	}

	//����VoronoiMesh�ĵ�ͱ�
	if (key3) {
		glColor3f(1.0f, 0.0f, 0.0f);

		drawVertex(pVoronoiMesh);
		drawEdge(pVoronoiMesh);

		glColor3f(0.0f, 0.0f, 0.0f);
		drawVertex(pDelaunayMesh);
	}

	//����VoronoiMesh����
	if (key4) {
		drawFace(pVoronoiMesh);

		glColor3f(0.0f, 0.0f, 0.0f);
		drawVertex(pDelaunayMesh);
	}

	glFlush();
}

//���ڸı�
void changeSize(int w, int h) {
	width = w;
	height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//��Ӧ����¼�
void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

		//ɾ��DelaunayMesh�е����бߣ�ɾ��VoronoiMesh
		vector<CFace*> delaunayFace;
		for (mfIter iter(pDelaunayMesh); !iter.end(); iter++) {
			delaunayFace.push_back(*iter);
		}
		for (int i = 0; i < delaunayFace.size(); i++) {
			(*pDelaunayMesh).deleteFace(delaunayFace[i]);
		}

		delete pVoronoiMesh;
		pVoronoiMesh = new Mesh;

		//���ø��ֲ���
		delaunayFaceId = 0;
		voronoiFaceId = 0;
		vecConvexHull.clear();
		toBeAdd.clear();
		mpFaceCenter.clear();


		double transX = (x - width / 2.0) / (width / 2.0);
		double transY = (height / 2.0 - y) / (height / 2.0);

		//mesh�¼���һ��Vertex
		CVertex* newVert = (*pDelaunayMesh).createVertex(delaunayVertexId++);
		newVert->point().coord()[0] = transX;
		newVert->point().coord()[1] = transY;
		allVertex.push_back(newVert);

		VoronoiDiagram();

		glutPostRedisplay();
	}
}

//��Ӧ�����¼�
void keyBoardFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1':
		key1 = !key1;
		break;
	case '2':
		key2 = !key2;
		break;
	case '3':
		key3 = !key3;
		break;
	case '4':
		key4 = !key4;
		break;
	case '?':
		help();
		break;
		/*case 27:
		exit(0);
		break;*/
	default:
		break;
	}
	glutPostRedisplay();
}

//��ʼ��OpenGL
void init_openGL(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Voronoi Diagram2D");

	glutDisplayFunc(&renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyBoardFunc);

	glClearColor(1.0, 1.0, 1.0, 0.0);

	glutMainLoop();
}

int main(int argc, char*argv[])
{
	srand((unsigned)time(NULL));
	pDelaunayMesh = new Mesh;
	pVoronoiMesh = new Mesh;
	initVertex();
	VoronoiDiagram();
	init_openGL(argc, argv);
	return 0;
}