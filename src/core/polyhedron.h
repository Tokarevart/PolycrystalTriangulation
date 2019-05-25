// Copyright © 2018-2019 Tokarev Artem Alekseevich. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include <array>
#include <list>
#include <vector>
#include "core/front/face.h"
#include "core/front/edge.h"
#include "core/shell/face.h"
#include "core/shell/edge.h"
#include "core/shell/vert.h"
#include "core/tetr.h"
#include "core/face.h"
#include "core/edge.h"
#include "core/vert.h"
#include "helpers/spatial/vec.h"
#include "core/relations.h"
#include "real-type.h"
#include "core/genparams.h"

#include "core/polyhedral-set.h"

#include "definitions.h"


namespace pmg {

class Polyhedron
{
    using pair_rr = std::pair<real_t, real_t>;

public:    
    real_t preferredLength() const;

    void addToShell( const shell::Face* shellFace );
    void addToShell( const shell::Edge* shellEdge );
    void addToShell( const shell::Vert* shellVert );

    bool shellContains( const shell::Face* shellFace ) const;
    bool shellContains( const shell::Edge* shellEdge ) const;
    bool shellContains( const shell::Vert* shellVert ) const;

    const std::list<Tetr*>& innerTetrs() const;
    const std::list<Face*>& innerFaces() const;
    const std::list<Vert*>& innerVerts() const;

    const std::list<front::Face*>& frontFaces() const;
    const std::list<front::Edge*>& frontEdges() const;

    // Returns minimum and average tetrahedrons quality or absGrad.
    pair_rr analyzeMeshQuality( std::list<Tetr*>::iterator* out_minQualityTetr = nullptr );
    pair_rr analyzeMeshAbsGrad();
    void    tetrahedralize( real_t preferredLength, genparams::Volume genParams = genparams::Volume() );
    void    smoothMesh( std::size_t nIters );

    Polyhedron();
    Polyhedron( PolyhedralSet* polyhset );
    ~Polyhedron();


private:
    enum class ExhaustType
    {
        DontExhaust,
        WithoutNewVert,
        WithNewVert
    };

    using pair_ff = std::pair<front::Face*, front::Face*>;

    genparams::Volume m_genparams;

    pair_rr m_meshQuality;
    pair_rr m_meshAbsGrad;
    bool m_isQualityAnalyzed     = false;
    bool m_isMeshAbsGradAnalyzed = false;

    real_t m_prefLen;

    // TODO: create and use separate Mesher (maybe template <std::size_t Dim>)
    // TODO: replace raw pointers with smart pointers and then make benchmark
    PolyhedralSet* m_polyhset = nullptr;

    // TODO: use Shell class instead
    std::vector<shell::Face*> m_shellFaces;
    std::vector<shell::Edge*> m_shellEdges;
    std::vector<shell::Vert*> m_shellVerts;

    // TODO: create and use Volume class instead
    std::list<pmg::Tetr*> m_innerTetrs;
    std::list<pmg::Face*> m_innerFaces;
    std::list<pmg::Edge*> m_innerEdges;
    std::list<pmg::Vert*> m_innerVerts;

    // TODO: create and use Front class instead
    // TODO: add front::Vert class
    std::list<front::Face*> m_frontFaces; // TODO: use std::set 'cause finding will be faster and m_frontEdges is also std::set
    std::list<front::Edge*> m_frontEdges; // TODO: use std::set sorting by angle (first larger) instead

    bool shellContains( const pmg::Vert* vert ) const;

    shell::Edge* findShellEdge( const shell::Vert* v0, const shell::Vert* v1 ) const;
    front::Face* findFrontFace( const pmg::Face* face ) const;
    std::vector<front::Edge*> findFEdge( const pmg::Vert* v0, const pmg::Vert* v1 ) const;
    std::vector<front::Edge*> findFEdge( const pmg::Edge* edge ) const;

    // Adds new front Face and corresponding Face.
    front::Face* addToFront( const pmg::Face* face, bool addInner = true );
    front::Edge* addToFront( const pmg::Edge* edge, bool addInner = true );

    void removeFromFront( front::Face* fFace );
    void removeFromFront( front::Edge* fEdge );

    // This section is about various front intersection checks
    // TODO: move that and other methods to relations.h later
    // TODO: change methods names
    bool segmentIntersectMesh(  const spt::vec3& v0, const spt::vec3& v1 ) const;
    bool segmentIntersectFront( const spt::vec3& v0, const spt::vec3& v1 ) const;
    bool edgeIntersectFront( const pmg::Vert* v0, const spt::vec3& v1 ) const;
    bool edgeIntersectFront( const pmg::Vert* v0, const pmg::Vert* v1 ) const;
    bool edgeIntersectAnyFace( const pmg::Edge* edge ) const;
    bool potentialEdgeIntersectFront( front::Edge* fEdge ) const;
    bool anyEdgeIntersectFace( const pmg::Vert* v0, const pmg::Vert* v1, const spt::vec3& v2 ) const;
    bool anyEdgeIntersectFace( const pmg::Vert* v0, const pmg::Vert* v1, const pmg::Vert* v2 ) const;
    bool anyEdgeIntersectPotentialFaces( front::Edge* fEdge ) const;
    bool anyVertInsidePotentialTetrCheck( front::Edge* fEdge ) const;
    bool parallelFacesCheck( front::Edge* fEdge ) const;
    // TODO: add more proximity checks
    bool doesFrontIntersectSphere( const spt::vec3& center, real_t radius ) const;
    bool frontSplitCheck(    front::Edge* fEdge, front::Edge* oppFEdge = nullptr ) const;
    bool frontCollapseCheck( front::Edge* fEdge, front::Edge* oppFEdge = nullptr ) const;

    static pair_rr computeMinMaxEdgesLengths(    const spt::vec3& p0, const spt::vec3& p1, const spt::vec3& p2, const spt::vec3& p3 );
    static pair_rr computeMinMaxEdgesSqrLengths( const spt::vec3& p0, const spt::vec3& p1, const spt::vec3& p2, const spt::vec3& p3 );
    static real_t  computeTetrSimpleQuality(     const spt::vec3& p0, const spt::vec3& p1, const spt::vec3& p2, const spt::vec3& p3 );
    static real_t  computeTetrSimpleSqrQuality(  const spt::vec3& p0, const spt::vec3& p1, const spt::vec3& p2, const spt::vec3& p3 );

    front::Edge* currentFrontEdge( real_t maxCompl ) const; // TODO: use std::map::upper_bound(...) method instead
    bool exhaustWithoutNewVertPriorityPredicate( front::Edge* fEdge );
    bool exhaustWithNewVertPriorityPredicate(    front::Edge* fEdge );
    ExhaustType computeExhaustionTypeQualityPriority(
        front::Edge* fEdge,
        front::Face*& out_withNWFFace, spt::vec3*& out_withNWNewVertPos );

    spt::vec3 computeNormalInTetr( const front::Face* fFace, const spt::vec3& oppVertPos ) const;
    spt::vec3 computeNormalInTetr( const front::Face* fFace, const pmg::Edge* oneOfRemainingEdges ) const;
    spt::vec3 computeNormalInTetr( const spt::vec3& fFacePos0, const spt::vec3& fFacePos1, const spt::vec3& fFacePos2, const spt::vec3& oppVertPos ) const;

    // TODO: add known neighbors initializations while creating new Tetr
    void assignFEdgesInFrontSplit( const front::Edge* fEdge, std::array<front::Edge*, 2> newOppFEdges, std::array<front::Face*, 2> newFFaces, pair_ff oppFFaces ) const;
    void exhaustFrontCollapse( front::Edge* fEdge, front::Edge* oppFEdge );
    void exhaustFrontSplit( front::Edge* fEdge, front::Edge* oppFEdge );
    void exhaustWithoutNewVertOppEdgeExists( front::Edge* fEdge, front::Edge* oppFEdge );
    void exhaustWithoutNewVertOppEdgeDontExists( front::Edge* fEdge );
    void exhaustWithoutNewVert( front::Edge* fEdge, bool doesOppEdgeExists = true, front::Edge* oppFEdge = nullptr );


    bool tryComputeNewVertPosType3(
            front::Face* fFace, spt::vec3& out_pos );
    bool tryComputeNewVertPosType2(
            front::Face* frontFace, spt::vec3& out_pos,
            std::size_t smallAngleIdx0, std::size_t smallAngleIdx1 );
    bool tryComputeNewVertPosType1(
            front::Face* fFace, spt::vec3& out_pos,
            std::size_t smallAngleIdx);
    bool tryComputeNewVertPosType0(
            front::Face* fFace, spt::vec3& out_pos );
    bool tryComputeNewVertPos( front::Face* fFace, spt::vec3& out_pos );

    real_t sqr4FaceArea( const front::Face* fFace ) const;
    front::Face* chooseFaceForExhaustionWithNewVert( front::Edge* fEdge );
    void         exhaustWithNewVert( front::Face* fFace, const spt::vec3& vertPos );

    bool tryExhaustWithoutNewVert( front::Edge* fEdge );
    bool tryExhaustWithNewVert(    front::Edge* fEdge );

    bool globalIntersectionCheck();

    bool isFrontExhausted();
    void processAngles();

    void debug();

    void flip( std::list<pmg::Tetr*>::iterator tetr );
    void flipWhile( real_t qualityLessThan );

    void computeFrontNormals();
    void initializeFFaceFEdges( front::Face* fFace ) const;
    void initializeFront();
};

} // namespace pmg