#include "SceneBuffer.hpp"
#include "drw/ArrayTextureStorage.hpp"
#include "drw/Format.hpp"

#include <glad.h>

#include <string.h>

SceneBuffer::SceneBuffer(SceneData *pData) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);	

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pData->get_num_vertices() * sizeof(Vertex), pData->get_vertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pData->get_num_indices() * sizeof(Index), pData->get_indices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, norm)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tangent)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
	glEnableVertexAttribArray(3);

	m_pMaterialBuffer = new MaterialBuffer(64, pData->numMaterials);
	m_pMaterialBuffer->push_materials(pData->numMaterials, pData->pMaterials);

	this->pMeshes = pData->pMeshes;
	this->numMeshes = pData->numMeshes;

	this->pPrimitives = pData->pPrimitives;
	this->numPrimitives = pData->numPrimitives;

	this->pGraph = pData->graph();
}

void SceneBuffer::use(drw::Pipeline *pPipeline) {
	glBindVertexArray(vao);
	m_pMaterialBuffer->use(pPipeline);
}
