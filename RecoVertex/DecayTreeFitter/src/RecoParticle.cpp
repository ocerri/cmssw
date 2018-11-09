#include <stdio.h>

#include "RecoParticle.h"
#include "FitParams.h"

namespace DecayTreeFitter
{
  extern int vtxverbose ;

  RecoParticle::RecoParticle(const int pdgId, ParticleBase* mother, const reco::Candidate* recoCand)
    : ParticleBase(pdgId, mother, recoCand)
  {
  }

  RecoParticle::~RecoParticle()
  {
  }

  std::string RecoParticle::parname(int index) const
  {
    return ParticleBase::parname(index+4) ;
  }

  ErrCode
  RecoParticle::projectConstraint(Constraint::Type type,
				  const FitParams& fitparams,
				  Projection& p) const
  {
    ErrCode status ;
    switch(type) {
    case Constraint::track:
    case Constraint::photon:
      status |= projectRecoConstraint(fitparams,p) ;
      break ;
    default:
      status |= ParticleBase::projectConstraint(type,fitparams,p) ;
    }
    return status ;
  }

  double RecoParticle::chiSquare(const FitParams* fitparams) const
  {
    // project
    Projection p(fitparams->dim(),dimM()) ;
    projectRecoConstraint(*fitparams,p) ;
    return p.chiSquare() ;
  }
}
