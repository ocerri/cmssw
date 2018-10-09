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

    edm::EDGetTokenT<std::vector<reco::GenParticle>> PrunedParticlesSrc_;
    edm::EDGetTokenT<std::vector<pat::PackedGenParticle>> PackedParticlesSrc_;
    int verbose = 0;
};



BuToD0munuToKPimunuMCTruthProducer::BuToD0munuToKPimunuMCTruthProducer(const edm::ParameterSet &iConfig):
  PrunedParticlesSrc_( consumes<std::vector<reco::GenParticle>> ( iConfig.getParameter<edm::InputTag>( "PrunedGenParticlesCollection" ) ) ),
  PackedParticlesSrc_( consumes<std::vector<pat::PackedGenParticle>> ( iConfig.getParameter<edm::InputTag>( "PackedGenParticlesCollection" ) ) )

{
    produces<pat::CompositeCandidateCollection>();
}


void BuToD0munuToKPimunuMCTruthProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    // edm::Handle<std::vector<pat::Muon>> muonHandle;
    // edm::Handle<edm::View<pat::PackedGenParticle>> GenParticlesHandle;
    // iEvent.getByToken(muonSrc_, muonHandle);
    // unsigned int muonNumber = muonHandle->size();

    // Get prunedGenParticles
    edm::Handle<std::vector<reco::GenParticle>> PrunedGenParticlesHandle;
    iEvent.getByToken(PrunedParticlesSrc_, PrunedGenParticlesHandle);
    unsigned int PrunedGenParticlesNumber = PrunedGenParticlesHandle->size();

    // Get packedGenParticles
    edm::Handle<std::vector<pat::PackedGenParticle>> PackedGenParticlesHandle;
    iEvent.getByToken(PackedParticlesSrc_, PackedGenParticlesHandle);
    unsigned int PackedGenParticlesNumber = PackedGenParticlesHandle->size();


    // Output collection
    std::unique_ptr<pat::CompositeCandidateCollection> result( new pat::CompositeCandidateCollection );

    if (verbose) {cout << "EVT" << endl;}

    for(unsigned int i = 0; i < PrunedGenParticlesNumber; i++) {
      const reco::GenParticle & p = (*PrunedGenParticlesHandle)[i];
      if ( abs(p.pdgId()) == 521 ) {
        if (verbose) {cout << "Found: " << p.pdgId() << endl;}
        pat::CompositeCandidate Bmeson;
        // Bmeson.addDaughter( muon1, "muon1");

        reco::Candidate * D0 = 0;
        pat::PackedGenParticle * pi = 0;
        pat::PackedGenParticle * K = 0;
        reco::Candidate * mu = 0;
        reco::Candidate * nu = 0;

        int BtoD0munu = 0;
        if (p.numberOfDaughters() == 3) {
          for(unsigned int k = 0; k < p.numberOfDaughters(); k++) {
            reco::Candidate * d = (reco::Candidate *) p.daughter(k);
            if (verbose) {cout << "\t" << k << " " << d->pdgId() << endl;}
            if ( abs(d->pdgId()) == 13) mu = (reco::Candidate *) p.daughter(k);
            else if ( abs(d->pdgId()) == 14) nu = (reco::Candidate *) p.daughter(k);
            else if ( abs(d->pdgId()) == 421) D0 = (reco::Candidate *) p.daughter(k);
          }
        }
        if ( D0!=0 && mu!=0 && nu!=0) {
          if (verbose) {cout << "\tIt's a BtoD0munu" << endl;}
          BtoD0munu = 1;
        }

        int D0toKpi = 0;
        if(BtoD0munu) {
          if (D0->numberOfDaughters() == 2) {
            if (verbose) {cout << "\t\t Two doughters" << endl;}
            for(unsigned int k = 0; k < PackedGenParticlesNumber; k++) {
              pat::PackedGenParticle* p = (pat::PackedGenParticle*) &((*PackedGenParticlesHandle)[k]);

              if(p->mother(0) == D0) {
                if (verbose) {
                  cout << "\t\t" << k << " " << p->pdgId() << endl;
                  cout << "\t\t" << k << " " << p->pt() << endl;
                  cout << "\t\t" << k << " " << p->eta() << endl;
                  cout << "\t\t" << k << " " << p->phi() << endl;
                }

                if ( abs(p->pdgId()) == 211) pi = p;
                else if ( abs(p->pdgId()) == 321 ) K = p;
              }
            }
          }
        }
        if (pi!=0 && K!=0) {
          if (verbose) {cout << "It's a D0toKpi" << endl;}
          D0toKpi = 1;
        }

        Bmeson.addUserInt("B_pdgID", p.pdgId());
        Bmeson.addUserFloat("B_pt", p.pt());
        Bmeson.addUserFloat("B_eta", p.eta());
        Bmeson.addUserFloat("B_phi", p.phi());
        Bmeson.addUserInt("B_D0munu", BtoD0munu);

        if(BtoD0munu) {
          Bmeson.addUserInt("D0_pdgID", D0->pdgId());
          Bmeson.addUserFloat("D0_pt", D0->pt());
          Bmeson.addUserFloat("D0_eta", D0->eta());
          Bmeson.addUserFloat("D0_phi", D0->phi());
          Bmeson.addUserFloat("D0_dz", D0->vz());

          Bmeson.addUserInt("mu_pdgID", mu->pdgId());
          Bmeson.addUserFloat("mu_pt", mu->pt());
          Bmeson.addUserFloat("mu_eta", mu->eta());
          Bmeson.addUserFloat("mu_phi", mu->phi());
          Bmeson.addUserFloat("mu_dz", mu->vz());

          Bmeson.addUserInt("nu_pdgID", nu->pdgId());
          Bmeson.addUserFloat("nu_pt", nu->pt());
          Bmeson.addUserFloat("nu_eta", nu->eta());
          Bmeson.addUserFloat("nu_phi", nu->phi());
        }
        else {
          Bmeson.addUserInt("D0_pdgID", 0);
          Bmeson.addUserFloat("D0_pt", 0);
          Bmeson.addUserFloat("D0_eta", 0);
          Bmeson.addUserFloat("D0_phi", 0);
          Bmeson.addUserFloat("D0_dz", -999999);
          Bmeson.addUserInt("mu_pdgID", 0);
          Bmeson.addUserFloat("mu_pt", 0);
          Bmeson.addUserFloat("mu_eta", 0);
          Bmeson.addUserFloat("mu_phi", 0);
          Bmeson.addUserFloat("mu_dz", -999999);
          Bmeson.addUserInt("nu_pdgID", 0);
          Bmeson.addUserFloat("nu_pt", 0);
          Bmeson.addUserFloat("nu_eta", 0);
          Bmeson.addUserFloat("nu_phi", 0);
        }

        Bmeson.addUserInt("D0_Kpi", D0toKpi);
        if (D0toKpi) {
          Bmeson.addUserInt("pi_pdgID", pi->pdgId());
          Bmeson.addUserFloat("pi_pt", pi->pt());
          Bmeson.addUserFloat("pi_eta", pi->eta());
          Bmeson.addUserFloat("pi_phi", pi->phi());
          Bmeson.addUserFloat("pi_dz", pi->dz());

          Bmeson.addUserInt("K_pdgID", K->pdgId());
          Bmeson.addUserFloat("K_pt", K->pt());
          Bmeson.addUserFloat("K_eta", K->eta());
          Bmeson.addUserFloat("K_phi", K->phi());
          Bmeson.addUserFloat("K_dz", K->dz());
        }
        else {
          Bmeson.addUserInt("pi_pdgID", 0);
          Bmeson.addUserFloat("pi_pt", 0);
          Bmeson.addUserFloat("pi_eta", 0);
          Bmeson.addUserFloat("pi_phi", 0);
          Bmeson.addUserFloat("pi_dz", -999999);
          Bmeson.addUserInt("K_pdgID", 0);
          Bmeson.addUserFloat("K_pt", 0);
          Bmeson.addUserFloat("K_eta", 0);
          Bmeson.addUserFloat("K_phi", 0);
          Bmeson.addUserFloat("K_dz", -999999);
        }

        result->push_back(Bmeson);
      }
    }

    iEvent.put(std::move(result));

}

DEFINE_FWK_MODULE(BuToD0munuToKPimunuMCTruthProducer);
