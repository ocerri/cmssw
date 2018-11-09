#ifndef DECAYTREEFITTER_INTERACTIONPOINT_H
#define DECAYTREEFITTER_INTERACTIONPOINT_H

#include "AlgebricObj.h"
#include "ParticleBase.h"
#include "DataFormats/VertexReco/interface/Vertex.h"


namespace DecayTreeFitter
{

  class InteractionPoint : public ParticleBase
  {
  public:
    InteractionPoint(const reco::Vertex& ipvertex);

    virtual int dim() const { return 3 ; } // (x,y,z)
    virtual ErrCode initPar1(FitParams*) ;
    virtual ErrCode initPar2(FitParams*) ;
    virtual ErrCode initCov(FitParams*) const ;

    virtual int type() const { return kInteractionPoint ; }

    virtual double chiSquare(const FitParams* par) const ;

    ErrCode projectIPConstraint(const FitParams& fitpar, Projection&) const ;
    virtual ErrCode projectConstraint(Constraint::Type, const FitParams&, Projection&) const ;

    virtual void addToConstraintList(constraintlist& alist, int depth) const ;

    virtual int posIndex() const { return index() ; }

  private:
    AlgebraicVector3     m_ipPos ;    // interaction point position
    AlgebraicSymMatrix33 m_ipCov ;    // cov matrix
    AlgebraicSymMatrix33 m_ipCovInv ; // inverse of cov matrix
  } ;

}


#endif
