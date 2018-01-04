#include "Mesh.h"

#define NEXT_INDICE do{i++;}while((buf[i]<'0')&&(buf[i]>'9'));

bool Mesh::s_bComputeNormals = false;

Mesh::Mesh()
{
	m_pVBO = NULL;
}

bool Mesh::Load(const string& name)
{
	cerr << "Loading mesh " << name << endl;
	m_pVBO = new VertexBufferObject();

	bool ret = false;
	if (name.find(".obj") != string::npos) {
		ret = LoadOBJ("./resources/meshs/" + name);
	}
	else {
		cerr << "Le mesh " << name << " n'est pas dans un format valide" << endl;
		return false;
	}

	if (!ret) {
		cerr << "[Error] Impossible de charger le mesh " << name << endl;
		return false;
	}

	if (s_bComputeNormals)
		ComputeNormals();
	ComputeTangents();
	//ComputeBoundingBox();

	return m_pVBO->Create(GL_STATIC_DRAW);
}

void Mesh::ComputeBoundingBox()
{
	//m_BBox.min = vec3(100000.0f, 100000.0f, 100000.0f);
	//m_BBox.max = vec3(-100000.0f, -100000.0f, -100000.0f);

	//vector<vec3>&	tPosition = m_pVBO->getPosition();

	//for (int i = 0; i < (int)tPosition.size(); i++)
	//{
	//	m_BBox.Add(tPosition[i]);
	//}
}

void Mesh::Destroy()
{
	m_pVBO->Destroy();
	delete m_pVBO;
	m_pVBO = NULL;

	for (vector<sGroup>::iterator it = m_tGroup.begin(); it != m_tGroup.end(); it++)
		(*it).tFace.clear();
	m_tGroup.clear();
}

bool Mesh::LoadOBJ(const string& filename)	// charge un objet Wavefront 3D (.obj)
{
	ifstream fp(filename.c_str(), ios::binary);
	if (!fp) return false;

	sFace face;
	sGroup grp;
	vec3 pt3D;
	vec2 pt2D;
	//	char buftmp[64];
	long i, v, g;	// compteur pour le stockage des données lors de la seconde passe

	GLuint	indFacePosition;
	GLuint	indFaceNormal;
	GLuint	indFaceTexcoord;

	vector<vec3>	tTempPosition;
	vector<vec3>	tTempNormal;
	vector<vec2>	tTempTexcoord;

	vector<vec3>&	tPosition = m_pVBO->getPosition();
	vector<vec3>&	tNormal = m_pVBO->getNormal();
	vector<vec2>&	tTexcoord = m_pVBO->getTexcoord();

	string strBuffer;
	while (getline(fp, strBuffer)) {
		stringstream strStream(strBuffer);
		const char* buf = strBuffer.c_str();

		if (strBuffer.size() > 0) {
			string buftmp;
			strStream >> buftmp;
			if (buftmp == "#") {	// on a trouvé un commentaire, on passe
			}
			else if (buftmp == "mtllib") {	// on a trouvé un matérial à charger ?

			}
			else if (buftmp == "v") {	// on a trouvé une vertice ?
				sscanf(&strBuffer[2], "%f%f%f", &pt3D.x, &pt3D.y, &pt3D.z);
				tTempPosition.push_back(pt3D);
			}
			else if (buftmp == "vt") {	// on a trouvé une coordonnée de texture ?
				sscanf(&strBuffer[2], "%f%f", &pt2D.x, &pt2D.y);
				pt2D.t = 1.0f - pt2D.t;
				tTempTexcoord.push_back(pt2D);
			}
			else if (buftmp == "vn") {	// on a trouvé une normale ?
				sscanf(&strBuffer[2], "%f%f%f", &pt3D.x, &pt3D.y, &pt3D.z);
				pt3D.normalize();
				tTempNormal.push_back(pt3D);
			}
			else if (buftmp == "g") {	// on a trouvé un groupe ?
				if (strBuffer.size() > 1) {
					grp.strName = &buf[2];
					grp.nMaterial = 0;
					m_tGroup.push_back(grp);
				}
			}
			else if (buftmp == "usemtl") {	// on a trouvé un matérial à utiliser ?
				if (m_tGroup.size() <= 0) {
					grp.strName = "No Name";
					grp.nMaterial = 0;
					m_tGroup.push_back(grp);
					g = 0;
				}
				else {
					g = (long)m_tGroup.size() - 1;
				}
			}
			else if (buftmp == "f") {	// on a trouvé une face ?
				int max;
				if (tTempPosition.size() >= tTempNormal.size())	max = 0;
				else												max = 1;

				if (max == 0 && tTempTexcoord.size() > tTempPosition.size())	max = 2;
				if (max == 1 && tTempTexcoord.size() > tTempNormal.size())	max = 2;

				switch (max) {
				case 0: {
					tPosition.resize(tTempPosition.size());
					tNormal.resize(tTempPosition.size());
					tTexcoord.resize(tTempPosition.size());
					break; }
				case 1: {
					tPosition.resize(tTempNormal.size());
					tNormal.resize(tTempNormal.size());
					tTexcoord.resize(tTempNormal.size());
					break; }
				case 2: {
					tPosition.resize(tTempTexcoord.size());
					tNormal.resize(tTempTexcoord.size());
					tTexcoord.resize(tTempTexcoord.size());
					break; }
				}

				if (m_tGroup.size() <= 0) {
					grp.strName = "No Name";
					grp.nMaterial = 0;
					m_tGroup.push_back(grp);
					g = 0;
				}
				else {
					g = (long)m_tGroup.size() - 1;
				}
				for (i = 0; (buf[i] < '0') || (buf[i] > '9'); i++);	// on se positionne à la première valeur
				for (v = 0; v < 3; v++) { // triangles donc composés de 3 vertices
					indFacePosition = 0;
					for (; (buf[i] >= '0') && (buf[i] <= '9'); i++) {	// on la récupère
						indFacePosition *= 10;	// première vertice
						indFacePosition += buf[i] - 0x30; // 0x30 est la valeur ascii du caractère '0'
					}
					indFacePosition--;	// indice n'est pas de 1 à nbFaces mais de 0 à nbFaces-1
					NEXT_INDICE;	// on se positionne à la valeur suivante
					indFaceTexcoord = 0;
					for (; (buf[i] >= '0') && (buf[i] <= '9'); i++) {	// on la récupère
						indFaceTexcoord *= 10;	// première coordonnée de texture
						indFaceTexcoord += buf[i] - 0x30;
					}
					indFaceTexcoord--;	// indice n'est pas de 1 à nbFaces mais de 0 à nbFaces-1
					NEXT_INDICE; // ect ... il y a 9 indices à récupérer
					indFaceNormal = 0;
					for (; (buf[i] >= '0') && (buf[i] <= '9'); i++) {
						indFaceNormal *= 10;	// première normale
						indFaceNormal += buf[i] - 0x30;
					}
					indFaceNormal--;	// indice n'est pas de 1 à nbFaces mais de 0 à nbFaces-1
					if (v < 2) NEXT_INDICE;

					int idx = 0;
					switch (max) {
					case 0: {idx = indFacePosition;	break; }
					case 1: {idx = indFaceNormal;	break; }
					case 2: {idx = indFaceTexcoord;	break; }
					}

					tPosition[idx] = tTempPosition[indFacePosition];
					tNormal[idx] = tTempNormal[indFaceNormal];
					tTexcoord[idx] = tTempTexcoord[indFaceTexcoord];
					face.ind[v] = idx;

				}

				m_tGroup[g].tFace.push_back(face); // on enregistre la face récupérée
			}
		}
	}
	fp.close();

	tTempPosition.clear();
	tTempNormal.clear();
	tTempTexcoord.clear();

	GLuint nbFaces = 0;
	for (vector<sGroup>::iterator it = m_tGroup.begin(); it != m_tGroup.end(); it++)
		nbFaces += (GLuint)(*it).tFace.size();

	return true;
}

void Mesh::ComputeNormals()
{
	vector<vec3>&	tPosition = m_pVBO->getPosition();
	vector<vec3>&	tNormal = m_pVBO->getNormal();
	vector<vec2>&	tTexcoord = m_pVBO->getTexcoord();

	tNormal.assign(tNormal.size(), vec3(0.0f, 0.0f, 0.0f));
	vector<int> tNormalCount(tNormal.size(), 0);

	for (vector<sGroup>::iterator itG = m_tGroup.begin(); itG != m_tGroup.end(); itG++) {
		for (vector<sFace>::iterator itF = (*itG).tFace.begin(); itF != (*itG).tFace.end(); itF++) {
			GLuint* ind = ((GLuint*)(*itF).ind);

			vec3 v0 = tPosition[ind[0]];
			vec3 v1 = tPosition[ind[1]];
			vec3 v2 = tPosition[ind[2]];

			vec3 vect10 = v0 - v1;
			vec3 vect12 = v2 - v1;

			vec3 vNormal = Cross(vect12, vect10);
			vNormal.normalize();

			tNormal[ind[0]] += vNormal;
			tNormal[ind[1]] += vNormal;
			tNormal[ind[2]] += vNormal;

			tNormalCount[ind[0]] ++;
			tNormalCount[ind[1]] ++;
			tNormalCount[ind[2]] ++;
		}
	}

	for (int i = 0; i < (int)tNormal.size(); i++)
		if (tNormalCount[i] > 0)
			tNormal[i] /= (float)tNormalCount[i];

	tNormalCount.clear();
}

void Mesh::ComputeTangents()
{
	vector<vec3>&	tPosition = m_pVBO->getPosition();
	vector<vec3>&	tNormal = m_pVBO->getNormal();
	vector<vec2>&	tTexcoord = m_pVBO->getTexcoord();
	vector<vec3>&	tTangent = m_pVBO->getTangent();

	tTangent.resize(tNormal.size());

	for (vector<sGroup>::iterator itG = m_tGroup.begin(); itG != m_tGroup.end(); itG++) {
		for (vector<sFace>::iterator itF = (*itG).tFace.begin(); itF != (*itG).tFace.end(); itF++) {
			GLuint* ind = ((GLuint*)(*itF).ind);
			vec3 vTangent;

			vec3 v0 = tPosition[ind[0]];
			vec3 v1 = tPosition[ind[1]];
			vec3 v2 = tPosition[ind[2]];

			vec3 vect10 = v0 - v1;
			vec3 vect12 = v2 - v1;

			float deltaT10 = tTexcoord[ind[0]].t - tTexcoord[ind[1]].t;
			float deltaT12 = tTexcoord[ind[2]].t - tTexcoord[ind[1]].t;

			vTangent = deltaT12 * vect10 - deltaT10 * vect12;
			vTangent.normalize();

			//			cout << "vNormal = " << m_tNormal[ind[0]].x << ", " << m_tNormal[ind[0]].y << ", " << m_tNormal[ind[0]].z << endl;
			//			cout << "vTangent = " << vTangent.x << ", " << vTangent.y << ", " << vTangent.z << endl;

			tTangent[ind[0]] = tTangent[ind[1]] = tTangent[ind[2]] = vTangent;
		}
	}
}

void Mesh::Draw()
{
	m_pVBO->Enable();

	for (vector<sGroup>::iterator it = m_tGroup.begin(); it != m_tGroup.end(); it++)
		glDrawElements(GL_TRIANGLES, (GLsizei)(*it).tFace.size() * 3, GL_UNSIGNED_INT, &((*it).tFace[0].ind[0]));

	m_pVBO->Disable();
}

void Mesh::Draw(GLuint group)
{
	m_pVBO->Enable();

	glDrawElements(GL_TRIANGLES, (GLsizei)m_tGroup[group].tFace.size() * 3, GL_UNSIGNED_INT, &(m_tGroup[group].tFace[0].ind[0]));

	m_pVBO->Disable();
}