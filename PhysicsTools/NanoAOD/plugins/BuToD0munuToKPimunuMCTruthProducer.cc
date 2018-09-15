#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <TLorentzVector.h>
#include <TVector.h>
#include <TMatrix.h>


//
// class declaration
//

using namespace std;

class BuToD0munuToKPimunuMCTruthProducer : public edm::EDProducer {

public:

    explicit BuToD0munuToKPimunuMCTruthProducer(const edm::ParameterSet &iConfig);

    ~BuToD0munuToKPimunuMCTruthProducer() override {};


private:

    virtual void produce(edm::Event&, const edm::EventSetup&);

    // ----------member data ---------------------------

    edm::EDGetTokenT<std::vector<reco::GenParticle>> GenParticlesSrc_;
};



BuToD0munuToKPimunuMCTruthProducer::BuToD0munuToKPimunuMCTruthProducer(const edm::ParameterSet &iConfig):
  GenParticlesSrc_( consumes<std::vector<reco::GenParticle>> ( iConfig.getParameter<edm::InputTag>( "GenParticlesCollection" ) ) )
{
    produces<pat::CompositeCandidateCollection>();
}


void BuToD0munuToKPimunuMCTruthProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    // edm::Handle<std::vector<pat::Muon>> muonHandle;
    // edm::Handle<edm::View<pat::PackedGenParticle>> GenParticlesHandle;
    // iEvent.getByToken(muonSrc_, muonHandle);
    // unsigned int muonNumber = muonHandle->size();

    edm::Handle<std::vector<reco::GenParticle>> GenParticlesHandle;
    iEvent.getByToken(GenParticlesSrc_, GenParticlesHandle);
    unsigned int GenParticlesNumber = GenParticlesHandle->size();

    // Output collection
    std::unique_ptr<pat::CompositeCandidateCollection> result( new pat::CompositeCandidateCollection );

    cout << "EVT" << endl;

    for(unsigned int i = 0; i < GenParticlesNumber; i++) {
      const reco::GenParticle & p = (*GenParticlesHandle)[i];
      if ( abs(p.pdgId()) == 521 ) {
        cout << "Found: " << p.pdgId() << endl;
        pat::CompositeCandidate Bmeson;
        // Bmeson.addDaughter( muon1, "muon1");
        Bmeson.addUserInt("B_pdgID", p.pdgId());
        Bmeson.addUserFloat("B_pt", p.pt());
        Bmeson.addUserFloat("B_eta", p.eta());

        for(unsigned int k = 0; k < p.numberOfDaughters(); k++) {
          const reco::Candidate * d = p.daughter(k);
          cout << "\t" << k << " " << d->pdgId() << endl;
        }

        result->push_back(Bmeson);
      }
    }

    iEvent.put(std::move(result));

}

DEFINE_FWK_MODULE(BuToD0munuToKPimunuMCTruthProducer);
