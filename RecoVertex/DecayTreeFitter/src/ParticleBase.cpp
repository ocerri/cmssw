#include <iomanip>
#include <float.h>

// #include "Event/Particle.h"
// #include "LoKi/ParticleProperties.h"

#include "ParticleBase.h"
// #include "InternalParticle.h"
// #include "RecoComposite.h"
// #include "RecoResonance.h"
// #include "RecoTrack.h"
// #include "RecoPhoton.h"
// #include "RecoMergedPi0.h"
// #include "Resonance.h"
// #include "MissingParticle.h"
// #include "JetMomentum.h"
// #include "InternalRecoTrack.h"
// #include "ConvertedPhoton.h"
#include "FitParams.h"
#include "Configuration.h"

// #include "Kernel/IParticlePropertySvc.h"
// #include "Kernel/ParticleProperty.h"
// #include "GaudiKernel/Service.h"

namespace DecayTreeFitter
{

  //template<class T>
  //inline T sqr(T x) { return x*x ; }

  int vtxverbose=0 ;

  ParticleBase::ParticleBase(const int pdgId, ParticleBase* mother, const reco::Candidate* recoCand)
    : m_recoCand(recoCand),m_mother(mother),
      // m_prop(LoKi::Particles::ppFromPID(particle.particleID())),
      m_index(0),m_pdgId(pdgId),
      m_pdtMass(0),m_pdtWidth(0),m_pdtCLifeTime(0),m_charge(0),
      m_name("Unknown"), m_hasMassConstraint(false)
  {
    switch(abs(m_pdgId)) {
      case 13:
        m_pdtMass      = 0.1056583754; //GeV
        m_pdtWidth     = 0.0000000024; //GeV
        m_pdtCLifeTime = 658.6384E3; //mm
        m_charge       = m_pdgId>0? -1 : 1;
        m_name         = m_pdgId>0? "mu-" : "mu+";
        break;
      case 211:
        m_pdtMass      = 0.13957018; //GeV
        m_pdtWidth     = 0.00000035; //GeV
        m_pdtCLifeTime = 7.80445E3; //mm
        m_charge       = m_pdgId>0? 1 : -1;
        m_name         = m_pdgId>0? "pi+" : "pi-";
        break;
      case 321:
        m_pdtMass      = 0.493677; //GeV
        m_pdtWidth     = 0.000016; //GeV
        m_pdtCLifeTime = 3.711E3; //mm
        m_charge       = m_pdgId>0? 1 : -1;
        m_name         = m_pdgId>0? "pi+" : "pi-";
        break;
      case 421:
        m_pdtMass      = 1.86483; //GeV
        m_pdtWidth     = 0.00005; //GeV
        m_pdtCLifeTime = 122.9E-3; //mm
        m_charge       = 0;
        m_name         = "D0";
        break;
      case 521:
        m_pdtMass      = 5.27931; //GeV
        m_pdtWidth     = 0.00015; //GeV
        m_pdtCLifeTime = 491.1E-3; //mm
        m_charge       = m_pdgId>0? 1 : -1;
        m_name         = m_pdgId>0? "B+" : "B-";
        break;
      default:
      std::cout << "PID " << m_pdgId << " not supported yet" << std::endl;
    }

    // if(m_mother) m_mother->addDaughter(this);
  }


  ParticleBase::ParticleBase(const std::string& name)
    : m_recoCand(0),m_mother(0),
      // m_prop(0),
      m_index(0),m_pdgId(0),
      m_pdtMass(0),m_pdtWidth(0),m_pdtCLifeTime(0),m_charge(0),
      m_name(name), m_hasMassConstraint(false)
  {
  }

  ParticleBase::~ParticleBase()
  {
    for(daucontainer::iterator it = m_daughters.begin() ;
        it != m_daughters.end() ; ++it)
      delete *it ;
    m_daughters.clear() ;
  }

  void ParticleBase::updateIndex(int& offset)
  {
    // first the daughters
    for(const_iterator it = begin(); it!= end(); ++it) (*it)->updateIndex(offset) ;
    // now the real work
    m_index = offset ;
    offset += dim() ;
  }

  void ParticleBase::addDaughter(ParticleBase* pb)
  {
    m_daughters.push_back(pb);
    pb->setMother(this);
  }

  void ParticleBase::removeDaughter(const ParticleBase* pb)
  {
    daucontainer::iterator it = std::find(m_daughters.begin(),m_daughters.end(),pb) ;
    if(it != m_daughters.end() ) {
      ParticleBase* _dp = *it ;
      m_daughters.erase(it) ;
      delete _dp ;
    } else {
      std::cout << "ERROR: cannot remove particle, because not found ..." << std::endl ;
    }
  }

  /*
  ParticleBase*
  ParticleBase::createParticle(const LHCb::Particle& particle,
                               const ParticleBase* mother,
                               const Configuration& config)
  {
    // This routine interpretes a beta candidate as one of the
    // 'Particles' used by the fitter.

    const LHCb::ParticleProperty* prop =
      LoKi::Particles::ppFromPID(particle.particleID()) ;

    ParticleBase* rc=0 ;
    //bool bsconstraint = false ;

    // We refit invalid fits, kinematic fits and composites with beamspot
    // constraint if not at head of tree.
    bool validfit = !config.forceFitAll() && particle.endVertex() != 0
      && particle.endVertex()->nDoF() > 0
      && particle.endVertex()->position() != Gaudi::XYZPoint(0,0,0)
      && particle.endVertex()->covMatrix()(2,2)>0 ;
    bool iscomposite = particle.daughters().size()>0 ;
    bool isresonance = iscomposite && prop && isAResonance(*prop) ;
    bool isconversion = iscomposite && prop->particleID().pid()==22 ;
    const LHCb::ProtoParticle* proto = particle.proto() ;
    bool hastrack = proto && proto->track() ;
    bool hascalo  = proto && !(proto->calo().empty()) ;

    if(!mother) { // 'head of tree' particles
      //if ( bsconstraint )
      //rc = new InteractionPoint(particle,forceFitAll) ;
      //else
      if( iscomposite ) {
	if( hastrack ) {
	  rc = new InternalRecoTrack(particle,0,config) ;
	} else {
	  rc = new InternalParticle(particle,0,config) ; // still need proper head-of-tree class
	}
      } else {
        std::cout << "VtkParticleBase::createParticle: You are fitting a decay tree that exists of "
                  << "a single, non-composite particle and which does not have a beamconstraint."
                  << "I do not understand what you want me to do with this." << std::endl ;
        rc = new InternalParticle(particle,0,config) ; // still need proper head-of-tree class
      }
    } else if( !iscomposite ) { // external particles
      if( hastrack )
        rc = new RecoTrack(particle,mother,config) ;  // reconstructed track
      else if( hascalo && particle.particleID().pid()==111)
	rc = new RecoMergedPi0(particle,mother) ; // this better be a merged pi0!
      else if( hascalo )
        rc = new RecoPhoton(particle,mother) ; // reconstructed photon
      else if( validfit ) {  // fitted composites w/o daughters?
        if( isresonance )
          rc = new RecoResonance(particle,mother) ;
        else
          rc = new RecoComposite(particle,mother) ;
      } else if(particle.particleID().pid()==22) { // jet constituent
	rc = new JetMomentum(particle,mother) ;
      } else // missing particle!
        rc = new MissingParticle(particle,mother) ;
    } else { // 'internal' particles
    if( validfit) {  // fitted composites
    // if( validfit || isconversion ) {  // fitted composites
        if( isresonance )
          rc = new RecoResonance(particle,mother) ;
        else
          rc = new RecoComposite(particle,mother) ;
      } else {         // unfited composites
	if( isconversion )
	  rc = new ConvertedPhoton(particle,mother,config) ;
        else if( isresonance )
          rc = new Resonance(particle,mother,config) ;
        else
          rc = new InternalParticle(particle,mother,config) ;
      }
    }

    if(vtxverbose>=2)
      std::cout << "ParticleBase::createParticle returns "
	        << rc->name() << " "
                << rc->type() << " " << rc->index() << std::endl ;
    return rc ;
  }
  */

  bool ParticleBase::isAResonance() {
    bool rc = false ;
    switch(m_pdgId) {
    case 22: // conversions are not treated as a resonance
      rc = false;
      break ;
    case 11: // bremstrahlung is treated as a resonance
    case -11:
      rc = true ;
      break ;
    default: // this should take care of the pi0 and D*
      rc = m_pdtCLifeTime < 10E-3 ;
    }
    return rc ;
  }

  void ParticleBase::collectVertexDaughters( daucontainer& particles, int posindex )
  {
    // collect all particles emitted from vertex with position posindex
    if( mother() && mother()->posIndex() == posindex )
      particles.push_back( this ) ;
    for( daucontainer::const_iterator idau = daughters().begin() ;
         idau != daughters().end() ; ++idau )
      (*idau)->collectVertexDaughters(particles,posindex ) ;
  }

  ErrCode ParticleBase::initCov(FitParams* fitparams) const
  {
    ErrCode status ;
    for(daucontainer::const_iterator it = m_daughters.begin() ;
        it != m_daughters.end() ; ++it)
      status |= (*it)->initCov(fitparams) ;

    if(vtxverbose>=2) {
      std::cout << "ParticleBase::initCov for " << name() << std::endl ;
    }

    // position
    int posindex = posIndex() ;
    if( posindex>=0 ) {
      const double sigx   = 100 ;
      const double sigy   = 100 ;
      const double sigz   = 500 ;
      fitparams->cov().fast(posindex+1,posindex+1) = sigx*sigx ;
      fitparams->cov().fast(posindex+2,posindex+2) = sigy*sigy ;
      fitparams->cov().fast(posindex+3,posindex+3) = sigz*sigz ;
    }

    // momentum
    int momindex = momIndex() ;
    if(momindex>=0) {
      if( daughters().empty() ) {
      	// TODO: calo at high energy?!
      	const double sigmom = 10 ; // GeV
      	int maxrow = hasEnergy() ? 4 : 3 ;
      	for(int row=momindex+1; row<=momindex+maxrow; ++row)
      	  fitparams->cov() .fast (row,row) = sigmom*sigmom ;
      } else {
      	// what a mess!
      	for(int row=1; row<=3; ++row) fitparams->cov().fast(momindex+row,momindex+row) = 0 ;
      	for(daucontainer::const_iterator it = m_daughters.begin(); it != m_daughters.end() ; ++it) {
      	  int daumomindex = (*it)->momIndex() ;
      	  for(int row=1; row<=3; ++row)
      	    fitparams->cov().fast(momindex+row,momindex+row) += fitparams->cov().fast(daumomindex+row,daumomindex+row) ;
      	}
      	if(hasEnergy())
      	  fitparams->cov().fast(momindex+4,momindex+4) = fitparams->cov().fast(momindex+3,momindex+3) ;
      }
    }

    // lifetime
    int lenindex = lenIndex() ;
    if(lenindex>=0) {
      const double sigz   = 500 ;
      fitparams->cov().fast (lenindex+1,lenindex+1) = sigz*sigz ;
    }

    return status ;
  }


  std::string ParticleBase::parname(int thisindex) const
  {
    std::string rc = name() ;
    switch(thisindex) {
    case 0: rc += " x  " ; break ;
    case 1: rc += " y  " ; break ;
    case 2: rc += " z  " ; break ;
    case 3: rc += " len" ; break ;
    case 4: rc += " px " ; break ;
    case 5: rc += " py " ; break ;
    case 6: rc += " pz " ; break ;
    case 7: rc += " E  " ; break ;
    default: ;
    }
    return rc ;
  }

  std::ostream& ParticleBase::fillStream(std::ostream& os, const FitParams* fitpar) const
  {
    os << std::setw(5) << "[" << type() << "]" << std::setw(15) << std::flush << name().c_str()
       << " val" << std::setw(15) << "err" << std::endl ;
    os << std::setprecision(5) ;
    for(int i=0; i<dim(); ++i) {
      int theindex = index()+i ;
      os << std::setw(2) << theindex << " "
      << std::setw(20) << parname(i).c_str()
      << std::setw(15) << fitpar->par()(theindex+1)
      << std::setw(15) << sqrt(fitpar->cov()(theindex+1,theindex+1))
      << std::setw(15) << fitpar->cov()(theindex+1,theindex+1) <<std::endl ;
    }
    if( hasEnergy() ) {
      int momindex = momIndex() ;
      double E  = fitpar->par()(momindex+4) ;
      double px = fitpar->par()(momindex+1) ;
      double py = fitpar->par()(momindex+2) ;
      double pz = fitpar->par()(momindex+3) ;
      double mass2 = E*E-px*px-py*py-pz*pz ;
      double mass = mass2>0 ? std::sqrt(mass2) : -std::sqrt(-mass2) ;

      double masserr = 0 ;
      if( !hasMassConstraint() ) {
      	CLHEP::HepSymMatrix cov = fitpar->cov().sub(momindex+1,momindex+4) ;
      	CLHEP::HepVector G(4,0) ;
      	G(1) = -px/mass ;
      	G(2) = -py/mass ;
      	G(3) = -pz/mass ;
      	G(4) =   E/mass ;
      	masserr = std::sqrt(cov.similarity(G)) ;
      }
      os << std::setw(2) << std::setw(20) << "mass: "
      << std::setw(15) << mass
      << std::setw(15) << masserr << std::endl ;
    }

    for(daucontainer::const_iterator it = m_daughters.begin() ;
        it != m_daughters.end() ; ++it)
      (*it)->fillStream(os,fitpar) ;
    return os ;
  }

  const ParticleBase* ParticleBase::locate(const reco::Candidate* p) const
  {
    const ParticleBase* rc = m_recoCand == p ? this : 0 ;
    for(daucontainer::const_iterator it = m_daughters.begin() ;
        !rc && it != m_daughters.end(); ++it)
      rc = (*it)->locate(p) ;
    return rc ;
  }

  void ParticleBase::locate( const int pdgId, ParticleContainer& result )
  {
    /// @attention Comparison by ABSPID!
    if ( abs(m_pdgId) == abs(pdgId) ) result.push_back(this) ;

    for( daucontainer::iterator it = m_daughters.begin() ;
         it != m_daughters.end(); ++it)
    { (*it)-> locate( pdgId, result ) ; }
  }

  void ParticleBase::retrieveIndexMap(indexmap& anindexmap) const
  {

    anindexmap.push_back(std::pair<const ParticleBase*,int>(this,index())) ;
    for(daucontainer::const_iterator it = m_daughters.begin() ;
        it != m_daughters.end() ; ++it)
      (*it)->retrieveIndexMap(anindexmap) ;
  }


  ErrCode ParticleBase::projectGeoConstraint(const FitParams& fitparams, Projection& p) const
  {
    // implements the constraint

    //  vec{x}_decay = vec{x}_production + decaylength * vec{p} / p
    int posindexmother = mother()->posIndex() ;
    int posindex = posIndex();
    int lenindex = lenIndex() ;
    int momindex = momIndex() ;
    assert(posindexmother>=0 && posindex>=0 && lenindex>=0 && momindex>=0) ;

    // decay length
    double len =  fitparams.par()(lenindex+1) ;

    // size of momentum
    double px = fitparams.par()(momindex+1) ;
    double py = fitparams.par()(momindex+2) ;
    double pz = fitparams.par()(momindex+3) ;
    double p2 = px*px+py*py+pz*pz ;
    double mom  = std::sqrt(p2) ;

    // lineair approximation is fine for now
    for(int row=1; row<=3; ++row) {
      double posxmother = fitparams.par()(posindexmother+row) ;
      double posx       = fitparams.par()(posindex+row) ;
      double momx       = fitparams.par()(momindex+row) ;
      p.r(row) = posxmother - posx + len*momx/mom ;
      p.H(row,posindexmother+row) = 1 ;
      p.H(row,posindex+row)       = -1 ;
      p.H(row,lenindex+1)         = momx/mom ;
    }
    // still need these as well
    p.H(1,momindex+1)  = len/mom*( 1 - px*px/p2 ) ;
    p.H(1,momindex+2)  = len/mom*( 0 - px*py/p2 ) ;
    p.H(1,momindex+3)  = len/mom*( 0 - px*pz/p2 ) ;
    p.H(2,momindex+1)  = len/mom*( 0 - py*px/p2 ) ;
    p.H(2,momindex+2)  = len/mom*( 1 - py*py/p2 ) ;
    p.H(2,momindex+3)  = len/mom*( 0 - py*pz/p2 ) ;
    p.H(3,momindex+1)  = len/mom*( 0 - pz*px/p2 ) ;
    p.H(3,momindex+2)  = len/mom*( 0 - pz*py/p2 ) ;
    p.H(3,momindex+3)  = len/mom*( 1 - pz*pz/p2 ) ;

    //     if( false && charge()!=0 ) {
    //       double lambda = bFieldOverC() * charge() ;
    //       double px0 = fitparams.par()(momindex+1) ;
    //       double py0 = fitparams.par()(momindex+2) ;
    //       double pt0 = sqrt(px0*px0+py0*py0) ;
    //       const double posprecision = 1e-4 ; // 1mu
    //       if( fabs(pt0*lambda*tau*tau) > posprecision ) {
    //  // use the helix, but as if it were a 'correction'
    //  double sinlt = sin(lambda*tau) ;
    //  double coslt = cos(lambda*tau) ;
    //  double px = px0*coslt - py0*sinlt ;
    //  double py = py0*coslt + px0*sinlt ;

    //  p.r(1) += -tau*px0 + (py-py0)/lambda ;
    //  p.r(2) += -tau*py0 - (px-px0)/lambda ;

    //  p.H(1,lenindex+1) += -px0 + px ;
    //  p.H(1,momindex+1) += -tau + sinlt/lambda ;
    //  p.H(1,momindex+2) +=        (coslt-1)/lambda ;
    //  p.H(2,lenindex+1) += -py0 + py ;
    //  p.H(2,momindex+1) +=      - (coslt-1)/lambda ;
    //  p.H(2,momindex+2) += -tau + sinlt/lambda ;

    //  if(vtxverbose>=2)
    //    std::cout << "Using helix for position of particle: " << name().c_str() << " "
    //         << lambda << " " << lambda*tau
    //         << "  delta-x,y: " << -tau*px0 + (py-py0)/lambda << "  "
    //         << -tau*py0 - (px-px0)/lambda << std::endl ;
    //       }
    //  }
    p.setParticle( *mother() ) ;
    return ErrCode::success ;
  }

  ErrCode ParticleBase::projectMassConstraint(const FitParams& fitparams,
                                              Projection& p) const
  {
    double mass  = pdtMass() ;
    double mass2 = mass*mass ;
    int momindex = momIndex() ;

    // initialize the value
    double px = fitparams.par()(momindex+1) ;
    double py = fitparams.par()(momindex+2) ;
    double pz = fitparams.par()(momindex+3) ;
    double E  = fitparams.par()(momindex+4) ;
    p.r(1) = E*E-px*px-py*py-pz*pz-mass2 ;

    // calculate the projection matrix
    p.H(1,momindex+1) = -2.0*px ;
    p.H(1,momindex+2) = -2.0*py ;
    p.H(1,momindex+3) = -2.0*pz ;
    p.H(1,momindex+4) =  2.0*E ;

    // set the variance in the residual
    double width = pdtWidth() ;
    p.Vfast(1,1) = 4*mass*mass*width*width ;

    return ErrCode::success ;
  }

  ErrCode ParticleBase::projectConstraint(Constraint::Type atype, const FitParams&, Projection&) const
  {
    std::cout << "no method to project this constaint: "
              << name() << " " << type() << " " << atype << std::endl ;
    return ErrCode::badsetup ;
  }

  //   double ParticleBase::bFieldOverC()
  //   {
  //     static const BField* bfield =  gblEnv->getTrk()->magneticField();
  //     static const double Bz = BField::cmTeslaToGeVc*bfield->bFieldNominal() ;
  //     return Bz ;
  //   }

  ErrCode ParticleBase::initTau(FitParams* fitparams) const
  {
    int lenindex = lenIndex() ;
    if(lenindex>=0 && hasPosition() ) {
      const ParticleBase* amother = mother() ;
      int momposindex = amother ? amother->posIndex() : -1 ;
      int posindex = posIndex() ;
      int momindex = momIndex() ;
      assert(momposindex>=0) ; // check code logic: no mother -> no tau
      //assert(fitparams->par(momposindex+1)!=0 ||fitparams->par(momposindex+2)!=0
      //      ||fitparams->par(momposindex+3)!=0) ; // mother must be initialized
      double momdX(0) ;
      double mom2(0) ;
      for(int irow=1; irow<=3; ++irow) {
        double dx = fitparams->par(posindex+irow) - fitparams->par(momposindex+irow) ;
        double px = fitparams->par(momindex+irow) ;
        momdX += px*dx ;
        mom2 += px*px ;
      }
      double len = momdX / std::sqrt(mom2) ;
      // FIXME: we don't like 0 and we don't like very negative values
      //if( tau==0 ) tau=pdtTau() ;
      //tau = tau==0 ? pdtTau() : std::max(tau,-pdtTau()) ;
      fitparams->par(lenindex+1) = len ;
    }
    return ErrCode::success ;
  }

  double ParticleBase::chiSquare(const FitParams* fitparams) const
  {
    double rc = 0;
    for(daucontainer::const_iterator it = m_daughters.begin() ;
        it != m_daughters.end(); ++it)
      rc += (*it)->chiSquare(fitparams) ;
    return rc ;
  }

  int ParticleBase::nFinalChargedCandidates() const {
    int rc=0;
    for(daucontainer::const_iterator it = m_daughters.begin() ;
        it != m_daughters.end() ; ++it)
      rc +=(*it)->nFinalChargedCandidates() ;
    return rc ;
  }

  ChiSquare ParticleBase::chiSquare( const FitParams& params ) const
  {
    ChiSquare chi2 ;
    // add contribution from daughters
    for(daucontainer::const_iterator it = m_daughters.begin() ;
        it != m_daughters.end() ; ++it) {
      chi2 += (*it)->chiSquare(params) ;
    }
    // add own chisquare, adjust for number of parameters
    chi2 += params.chiSquare( *this ) ;
    chi2 += ChiSquare( 0, -dim() ) ;
    return chi2 ;
  }
}
