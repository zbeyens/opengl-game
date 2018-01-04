#pragma once

#include "vertexBufferObject.h"

int GenerateTorus(CVertexBufferObject &vboDest, float fRadius, float fTubeRadius, int iSubDivAround, int iSubDivTube);
int GenerateCylinder(CVertexBufferObject &vboDest, float fRadius, float fHeight, int iSubDivAround, float fMapU = 1.0f, float fMapV = 1.0f);
void CreateStaticSceneObjects(UINT* uiVAO, CVertexBufferObject& vboDest);

extern int iSphereFaces;