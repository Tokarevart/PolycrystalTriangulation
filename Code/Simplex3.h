#pragma once
#include <algorithm>
#include <memory>
#include "Definitions.h"
#include "Inclusions.h"
#include "unique_ptr_helper.h"

using std::unique_ptr;

class Simplex3 : public unique_ptr_helper<Simplex3>
{
public:
	unique_ptr<Vertex3>* vertexes[4];

	double computeVolume() const;
	double computeQuality() const;

	Simplex3();
	Simplex3(
		Vertex3 &vert0,
		Vertex3 &vert1,
		Vertex3 &vert2,
		Vertex3 &vert3);
	~Simplex3();
};