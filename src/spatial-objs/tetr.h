#pragma once
#include <algorithm>
#include "spatial-objs/vertex.h"

#include "definitions.h"


namespace pmg {

class Tetr
{
public:
    Vertex* verts[4];

    double computeVolume()  const;
    double computeQuality() const;

    Tetr( const Vertex* vert0,
          const Vertex* vert1,
          const Vertex* vert2,
          const Vertex* vert3 );
};

} // namespace pmg