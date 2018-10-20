#ifndef DECAYTREEFITTER_INTERACTIONPOINT_H
#define DECAYTREEFITTER_INTERACTIONPOINT_H

#include "ParticleBase.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Candidate/interface/Candidate.h"

namespace reco
{
  class Vertex ;
}

namespace DecayTreeFitter
{

  class InteractionPoint : public ParticleBase
  {
  public:
    InteractionPoint(const reco::Vertex& ipvertex, const reco::Candidate& daughter,
		     const Configuration& config) ;

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
    double m_ipPos[3];    // interaction point position
    CovarianceMatrix m_ipCov ;    // cov matrix
    CovarianceMatrix m_ipCovInv ; // inverse of cov matrix
  } ;

}


#endif
