#ifndef __VTKDECAYCHAIN_HH__
#define __VTKDECAYCHAIN_HH__

#include <map>
#include "ParticleBase.h"
#include "MergedConstraint.h"

namespace reco {
  class Candidate ;
  class Vertex ;
}

namespace DecayTreeFitter {

  class FitParams ;
  class ParticleBase ;

  class DecayChain
  {
  public:
    DecayChain() : m_mother(0) {}

    DecayChain(const reco::Candidate& bc, const Configuration& config)  ;
    DecayChain(const reco::Candidate& bc, const reco::Vertex& pv, const Configuration& config)  ;

    ~DecayChain() ;

    int dim() const { return m_dim ; }

    void initConstraintList() ;
    ErrCode init(FitParams& par) ;
    ErrCode filter(FitParams& par, bool firstpass=true) ;
    double chiSquare(const FitParams* par) const ;

    ParticleBase* mother() { return m_mother ; }
    const ParticleBase* cand() { return m_cand ; }
    const ParticleBase* mother() const { return m_mother ; }
    const ParticleBase* locate(const reco::Candidate& bc) const ;
    void locate( const reco::CandidateID& pid, ParticleBase::ParticleContainer& result ) ;

    int index(const reco::Candidate& bc) const ;
    int posIndex(const reco::Candidate& bc) const ;
    int momIndex(const reco::Candidate& bc) const ;
    int lenIndex(const reco::Candidate& bc) const ;
    void setOwner(bool b) { m_isOwner=b ;}
    int momIndex() const ;

    void printConstraints(std::ostream& os=std::cout) const ;
    void setMassConstraint( const reco::Candidate& bc, bool add=true) ;
    void setMassConstraint( const reco::Candidate& bc, double mass) ;
    void setMassConstraint( const reco::CandidateID& pid, bool add=true) ;
    void setMassConstraint( const reco::CandidateID& pid, double mass) ;

    ChiSquare chiSquare( const reco::Candidate& bc, const FitParams& fitpars ) const ;
  private:
    int m_dim ;
    ParticleBase* m_mother ;     // head of decay tree
    const ParticleBase* m_cand ; // fit candidate (not same to mother in case of bs/be constraint)
    ParticleBase::constraintlist m_constraintlist ;
    std::vector<Constraint*> m_mergedconstraintlist ;
    MergedConstraint m_mergedconstraint ;
    typedef std::map<const reco::Candidate*,const ParticleBase*> ParticleMap ;
    mutable ParticleMap m_particleMap ;
    bool m_isOwner ;
  } ;

}



#endif
