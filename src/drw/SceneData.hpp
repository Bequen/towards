#pragma once

#include "Vertex.hpp"
#include "drw/scene/Transform.hpp"
#include "drw/scene/SceneGraph.hpp"
#include "drw/ImageData.hpp"
#include "drw/MaterialData.hpp"


#include <cglm/mat4.h>
#include <stdio.h>
#include <filesystem>
#include <paths.h>
#include <string.h>
#include <string>

struct Mesh {
	unsigned int primitiveIdx;
	unsigned int numPrimitives;
};

struct Primitive {
	unsigned int offset;
	unsigned int count;
	unsigned int baseVertex;
	unsigned int materialIdx;
};

/* Holds mesh data. There should not be any other structure holding actual data. They should all just be pointers. */
/* TODO: Turn pointers into LinearAllocators */
class SceneData {
public:
	std::string m_dir;
	std::string m_filename;

	MaterialData *pMaterials;
	unsigned long maxMaterials;
	unsigned long numMaterials;

	ImageData *pTextures;
	unsigned long maxTextures;
	unsigned long numTextures;

	/* Information about meshes */
	Vertex *pVertices;
	unsigned long maxVertices;
	unsigned long numVertices;

	Index *pIndices;
	unsigned long maxIndices;
	unsigned long numIndices;

	/* How to interpret the vertex and index data */
	Mesh *pMeshes;
	unsigned long maxMeshes;
	unsigned long numMeshes;

	Primitive *pPrimitives;
	unsigned long maxPrimitives;
	unsigned int numPrimitives;

	/* How and where draw these meshes */
	SceneGraph *pGraph;

public:
	void push_material(MaterialData data) {
		pMaterials[numMaterials++] = data;
	}
	void set_path(std::string path);
	std::string relative_path_of(std::string path);

	SceneGraph *graph() { return pGraph; }

	Mesh *mesh(unsigned int idx) { return &pMeshes[idx]; }

	Vertex *get_vertices() {
		return pVertices;
	}
	unsigned long get_num_vertices() {
		return numVertices;
	}

	Index *get_indices() {
		return pIndices;
	}
	unsigned long get_num_indices() {
		return numIndices;
	}

	Mesh *get_meshes() {
		return pMeshes;
	}
	unsigned long get_num_meshes() {
		return numMeshes;
	}

	/*
	 * Allocates scene data with numVertices and numIndices.
	 */
	SceneData(unsigned long numNodes,
			  unsigned long numMeshes,
			  unsigned long numPrimitives,
			  unsigned long numVertices,
			  unsigned long numIndices,
			  unsigned long numMaterials, 
			  unsigned long numTextures);

	Vertex *suballoc_vertices(unsigned long count);

	Index *suballoc_indices(unsigned long count);

	Mesh *suballoc_mesh();

	int push_mesh(unsigned int numPrimitives) {
		pMeshes[numMeshes++] = {
			.primitiveIdx = this->numPrimitives - numPrimitives,
			.numPrimitives = numPrimitives
		};

		return numMeshes - 1;
	}

	void push_primitive(unsigned int indexOffset, unsigned int indexCount, unsigned int baseVertex, unsigned int materialIdx) {
		pPrimitives[numPrimitives++] = {
			.offset = indexOffset,
			.count = indexCount,
			.baseVertex = baseVertex,
			.materialIdx = materialIdx
		};
	}
};
