// $Id: Fitter.h,v 1.7 2010-05-29 13:39:56 ibelyaev Exp $
// ============================================================================
#ifndef DECAYTREEFITTER_FITTER_HH
#define DECAYTREEFITTER_FITTER_HH
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace DecayTreeFitter
{
  class DecayChain ;
  class FitParams ;
  class ParticleBase ;
  class ChiSquare ;
}
// ============================================================================
namespace DecayTreeFitter
{
  // ==========================================================================
  /** @class Fitter DecayTreeFitter/Fitter.h
   *  ``Decay-Tree-Fitter''`
   *  @author Wouter Hulsbergen  Wouter.Hulsbergen@nikhef.nl
   */
  class Fitter
  {
  public:
    // ========================================================================
    enum FitStatus {
      UnFitted     = -1 ,
      Success      =  0 ,
      Failed            ,
      BadInput          ,
      NonConverged
    } ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the particle (decay head)
    Fitter ( const reco::Candidate&   bc                  ,
             const bool              forceFitAll  = true );
             // const ITrackStateProvider* extrapolator = 0    ) ;
    /// constructor from the particle (decay head)
    Fitter ( const reco::Candidate&   bc                  ,
             // const ITrackStateProvider* extrapolator        ,
             const bool              forceFitAll  = true ) ;
    /// constructor from the particle (decay head) and primary vertex
    Fitter ( const reco::Candidate&   bc                  ,
             const reco::Vertex& pv                  ,
             const bool              forceFitAll  = true ) ;
    /// constructor from the particle (decay head) and primary vertex
    Fitter ( const reco::Candidate&   bc                  ,
             const reco::Vertex& pv                  ,
             // const ITrackStateProvider* extrapolator        ,
             const bool              forceFitAll  = true ) ;
    /// destructor
    ~Fitter() ;                                                   // destructor
    // ========================================================================
  public:
    // ========================================================================
    /// Add or remove a mass constraint
    void setMassConstraint ( const reco::Candidate&   cand , bool add = true ) ;
    /// Add a constraint to a mass different from the property table mass
    void setMassConstraint ( const reco::Candidate&   cand , double mass ) ;
    /// Add or remove a mass constraintfor a certain ParticleID
    void setMassConstraint ( const reco::CandidateID& pid  , bool add = true ) ;
    /// Add a constraint to a mass different from the property table mass
    void setMassConstraint ( const reco::CandidateID& pid  , double mass ) ;
    /// Fit the decay tree
    void fit ( int     maxNumberOfIterations  = 10   ,
               double deltaChisquareConverged = 0.01 ) ;
    /// Fit just one step
    void fitOneStep() ;
    /// Print the result of the fit
    std::string   print() const ;
    /// Print the result of the fit
    std::ostream& fillStream ( std::ostream& s ) const ;
    /// The top level particle that is fitted
    const reco::Candidate* particle() const { return m_particle ; }
    /** Currently the only accessor to the actual fitted data
     *  @param p (INPUT) the particle
     *  @retrn the fitted parameters ( 0 for invaild parameters/fits)
     */
    const Gaudi::Math::ParticleParams*
    fitParams ( const reco::Candidate* p = 0 ) const ;
    /// Total chisquare
    double     chiSquare () const { return m_chiSquare ; }
    /// Total number of DOFs
    int        nDof      () const ;
    /// Status of fit
    FitStatus  status    () const { return m_status ; }
    /// Number of iterations used by vertex fit
    int        nIter     () const { return m_niter  ; }
    /// get the chisquare of everything 'downstream' of a particle
    ChiSquare chiSquare( const reco::Candidate& p ) const ;
    /**  Compute the decay length sum of two particles in
     *   the decay tree (useful for e.g. B->DD)
     */
    Gaudi::Math::ValueWithError
    decayLengthSum ( const reco::Candidate&, const reco::Candidate& ) const ;
    /** return an updated decay tree.
     *  this is not a final solution. will
     *  try to move more info to Particle
     */
    LHCb::DecayTree getFittedTree() const ;
    /** methods to retrieve the result in terms of reco::Candidates
     * (note: mother vertex is not updated, and decay length cannot be
     * stored anywhere. Use fitParams instead
     */
    reco::Candidate getFitted() const ;
    /** methods to retrieve the result in terms of reco::Candidates
     * (note: mother vertex is not updated, and decay length cannot be
     * stored anywhere. Use fitParams instead
     */
    reco::Candidate getFitted(const reco::Candidate& cand) const ;
    /// update a particlular candidate in the tree
    bool updateCand ( reco::Candidate& cand ) const ;
    /// update a particlular candidate in the tree
    bool updateTree ( reco::Candidate& cand ) const ;
    /// error code
    int  errCode  () { return m_errCode ; }
    /// set the verbosity level (for debugging only)
    static void setVerbose(int i) ;

    // ========================================================================
  public:
    // ========================================================================
    /// get the extrapolator
    // const ITrackStateProvider* extrapolator() const { return m_extrapolator ; }
    /// set the track extrapolator
    // void setStateProvider ( const ITrackStateProvider* extrapolator ) ;
    // ========================================================================
  protected:
    // ========================================================================
    // expert interface. not yet for real consumption
    Gaudi::Math::ParticleParams
    fitParams(const ParticleBase& pb) const ;
    /// Name of a particle in the decay tree
    std::string name ( const reco::Candidate& cand) const ;
    // ========================================================================
    Gaudi::Math::ValueWithError
    decayLengthSum(const ParticleBase&,const ParticleBase&) const ;
    // ========================================================================
    DecayChain*       decaychain ()       { return m_decaychain  ; }
    FitParams*        fitparams  ()       { return m_fitparams   ; }
    const DecayChain* decaychain () const { return m_decaychain  ; }
    const FitParams*  fitparams  () const { return m_fitparams   ; }
    // ========================================================================
    double globalChiSquare() const ;
    // ========================================================================
    // must be moved to derived class or so ...
    double add     ( const reco::Candidate& cand ) ;
    double remove  ( const reco::Candidate& cand ) ;
    void   updateIndex() ;
    // ========================================================================
    reco::Candidate* fittedCand ( const reco::Candidate& cand       ,
                                 reco::Candidate*       headoftree ) const ;
    void            updateCand ( const ParticleBase&   pb         ,
                                 reco::Candidate&       cand       ) const ;
    // ========================================================================
  private:
    // ========================================================================
    /// default constructor is disabled
    Fitter () ;                             //  default constructor is disabled
    /// copy constructor is disabled
    Fitter ( const Fitter& ) ;              //     copy constructor is disabled
    /// assignement operator is disabled
    Fitter& operator=( const Fitter& ) ;    // assignement operator is disabled
    // ========================================================================
  private:
    // ========================================================================
    const reco::Candidate* m_particle   ;
    DecayChain*           m_decaychain ;
    FitParams*            m_fitparams  ;
    FitStatus             m_status     ;
    double                m_chiSquare  ;
    int                   m_niter      ;
    int                   m_errCode    ;
    // ========================================================================
    typedef std::map<const reco::Candidate*, Gaudi::Math::ParticleParams> Map ;
    mutable Map m_map ;
    // ========================================================================
    /// track extrapolator (if needed)
    // SmartIF<ITrackStateProvider> m_extrapolator ; // track extrapolator
    // ========================================================================
  } ;
  // ==========================================================================
}
// ============================================================================
// The END
// ============================================================================
#endif
// ============================================================================
