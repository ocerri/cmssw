#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include <TLorentzVector.h>
#include <TVector.h>
#include <TMatrix.h>
#include <iostream>
#include <string>
#include <regex>


//
// class declaration
//

using namespace std;

class BuToD0munuToKPimunuProducer : public edm::EDProducer {

public:

    explicit BuToD0munuToKPimunuProducer(const edm::ParameterSet &iConfig);

    ~BuToD0munuToKPimunuProducer() override {};


private:

    virtual void produce(edm::Event&, const edm::EventSetup&);

    vector<pat::Muon> TriggerObj_matching(edm::Handle<vector<pat::Muon>>, pat::TriggerObjectStandAlone);

    // ----------member data ---------------------------
    edm::EDGetTokenT<reco::BeamSpot> beamSpotSrc_;
    edm::EDGetTokenT<vector<pat::PackedCandidate>> PFCandSrc_;
    edm::EDGetTokenT<reco::VertexCollection> vertexSrc_;
    edm::EDGetTokenT<std::vector<pat::Muon>> muonSrc_;

    edm::EDGetTokenT<edm::TriggerResults> triggerBitsSrc_;
    edm::EDGetTokenT<vector<pat::TriggerObjectStandAlone>> triggerObjectsSrc_;

    double max_dR_mh_;
    double max_dz_mh_;
    // double etaMaxMu_;
    // double ptMinKaon_;
    // double etaMaxKaon_;
    // double DCASigMinKaon_;
    // double ptMinPion_;
    // double etaMaxPion_;
    // double DCASigMinPion_;

    float MuonMass_ = 0.10565837;
    float MuonMassErr_ = 3.5*1e-9;
    float KaonMass_ = 0.493677;
    float KaonMassErr_ = 1.6e-5;
    float PionMass_ = 0.139570;
    float PionMassErr_ = 3.5e-7;
    float D0Mass_ = 1864.83;
    float D0MassErr_ = 0.05;

    int verbose = 1;
};



BuToD0munuToKPimunuProducer::BuToD0munuToKPimunuProducer(const edm::ParameterSet &iConfig):
  beamSpotSrc_( consumes<reco::BeamSpot> ( iConfig.getParameter<edm::InputTag>( "beamSpot" ) ) ),
  PFCandSrc_( consumes<vector<pat::PackedCandidate>> ( iConfig.getParameter<edm::InputTag>( "PFCandCollection" ) ) ),
  vertexSrc_( consumes<reco::VertexCollection> ( iConfig.getParameter<edm::InputTag>( "vertexCollection" ) ) ),
  muonSrc_( consumes<std::vector<pat::Muon>> ( iConfig.getParameter<edm::InputTag>( "muonCollection" ) ) ),
  triggerBitsSrc_( consumes<edm::TriggerResults> ( iConfig.getParameter<edm::InputTag>("triggerBits") ) ),
  triggerObjectsSrc_(consumes<vector<pat::TriggerObjectStandAlone>> ( iConfig.getParameter<edm::InputTag>("triggerObjects") ) ),
  max_dR_mh_( iConfig.getParameter<double>( "max_dR_mh" ) ),
  max_dz_mh_( iConfig.getParameter<double>( "max_dz_mh" ) )
{
  produces<pat::CompositeCandidateCollection>();
}


void BuToD0munuToKPimunuProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    // edm::ESHandle<MagneticField> bFieldHandle;
    // iSetup.get<IdealMagneticFieldRecord>().get(bFieldHandle);
    //
    // edm::ESHandle<TransientTrackBuilder> theTTBuilder;
    // iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);

    edm::Handle<reco::BeamSpot> beamSpotHandle;
    iEvent.getByToken(beamSpotSrc_, beamSpotHandle);
    if ( ! beamSpotHandle.isValid() ) {
        edm::LogError("BuToD0munuToKPimunuProducer") << "No beam spot available from EventSetup" ;
    }
    if (verbose) {cout << "---- Evt ----" << endl;}
    // reco::BeamSpot beamSpot = *beamSpotHandle;

    edm::Handle<vector<pat::PackedCandidate>> pfCandHandle;
    iEvent.getByToken(PFCandSrc_, pfCandHandle);
    // unsigned int pfCandNumber = pfCandHandle->size();

    edm::Handle<vector<reco::Vertex>> vertexHandle;
    iEvent.getByToken(vertexSrc_, vertexHandle);
    // const reco::Vertex & PV = vertexHandle->front();

    edm::Handle<std::vector<pat::Muon>> muonHandle;
    iEvent.getByToken(muonSrc_, muonHandle);
    unsigned int muonNumber = muonHandle->size();

    edm::Handle<edm::TriggerResults> triggerBits;
    iEvent.getByToken(triggerBitsSrc_, triggerBits);

    edm::Handle<vector<pat::TriggerObjectStandAlone>> triggerObjects;
    iEvent.getByToken(triggerObjectsSrc_, triggerObjects);


    // Output collection
    std::unique_ptr<pat::CompositeCandidateCollection> result( new pat::CompositeCandidateCollection );

    //Get the triggers
    unsigned int BPH_trigger = 0;
    std::regex txt_regex_path("HLT_Mu[0-9]+_IP[0-9]+.*");
    const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
    if (verbose) {std::cout << "\nPASSED BPH TRIGGER PATHS: " << std::endl;}
    for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
      if (std::regex_match(names.triggerName(i), txt_regex_path) && triggerBits->accept(i)) {
        if (verbose) {std::cout << "Trigger " << names.triggerName(i) << ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")<< std::endl;}
        BPH_trigger = 1;
      }
    }

    vector<pat::Muon> matching_muons;
    pat::Muon * muon_trg = 0;
    pat::TriggerObjectStandAlone * obj_trg = 0;

    if(BPH_trigger) {
      if (verbose) {cout << "\nTRIGGER OBJECTS " << endl;}
      uint k = 0;
      for (pat::TriggerObjectStandAlone obj : *triggerObjects) {
        obj.unpackNamesAndLabels(iEvent, *triggerBits);
        vector pathNamesAll = obj.pathNames(false);
        vector pathNamesLast = obj.pathNames(true);

        unsigned int obj_BPH_path = 0;
        for (unsigned h = 0, n = pathNamesAll.size(); h < n; ++h) {
          if (regex_match(pathNamesAll[h], txt_regex_path)) obj_BPH_path = 1;
        }
        std::regex txt_regex_coll("hlt.*MuonCandidates::HLT");
        if (!regex_match(obj.collection(), txt_regex_coll)) obj_BPH_path *= 0;

        if (obj_BPH_path){
          if (verbose) {
            cout << "\tTrigger object:  pt " << obj.pt() << ", eta " << obj.eta() << ", phi " << obj.phi() << endl;
            // Print trigger object collection and type
            cout << "\tCollection: " << obj.collection() << endl;
            // cout << "\t   Type IDs:   ";
            // for (unsigned h = 0; h < obj.filterIds().size(); ++h) cout << " " << obj.filterIds()[h] ;
            // cout << endl;
            // // Print associated trigger filters
            // cout << "\t   Filters:    ";
            // for (unsigned h = 0; h < obj.filterLabels().size(); ++h) cout << " " << obj.filterLabels()[h];
            // cout << endl;
            // Print all trigger paths, for each one record also if the object is associated to a 'l3' filter (always true for the
            // definition used in the PAT trigger producer) and if it's associated to the last filter of a successfull path (which
            // means that this object did cause this trigger to succeed; however, it doesn't work on some multi-object triggers)
            // cout << "\t   Paths (" << pathNamesAll.size()<<"/"<<pathNamesLast.size()<<"):    ";
            // for (unsigned h = 0, n = pathNamesAll.size(); h < n; ++h) {
            //   bool isBoth = obj.hasPathName( pathNamesAll[h], true, true );
            //   bool isL3   = obj.hasPathName( pathNamesAll[h], false, true );
            //   bool isLF   = obj.hasPathName( pathNamesAll[h], true, false );
            //   bool isNone = obj.hasPathName( pathNamesAll[h], false, false );
            //   cout << "   " << pathNamesAll[h];
            //   if (isBoth) cout << "(L,3)";
            //   if (isL3 && !isBoth) cout << "(*,3)";
            //   if (isLF && !isBoth) cout << "(L,*)";
            //   if (isNone && !isBoth && !isL3 && !isLF) cout << "(*,*)";
            // }
            cout << endl;
          }

          if (obj_trg == 0 || obj_trg->pt() < obj.pt()) {
            obj_trg = (pat::TriggerObjectStandAlone *) &((*triggerObjects)[k]);

            matching_muons = TriggerObj_matching(muonHandle, obj);
            if (matching_muons.size()>0) muon_trg = &(matching_muons[0]);

            if(verbose && obj_trg->pt() < obj.pt()) {cout << "New trg candidate found" << endl;}
          }
        }
        k++;
      }

      if (verbose) {
        cout << "\n MUONS " << endl;
        for (unsigned int k = 0; k < muonNumber; ++k) {
          const pat::Muon & muon = (*muonHandle)[k];
          cout << "\t" << k << " " << muon.pdgId() << "  " << muon.pt() << "  " << muon.eta() << "  " << muon.phi() << endl;
        }
        cout << endl;
      }


    }


    if(muon_trg!=0) {
      vector<pat::PackedCandidate> Had_plus;
      vector<pat::PackedCandidate> Had_minus;

      for(pat::PackedCandidate p: *pfCandHandle) {
        float dR_mh = TMath::Hypot(muon_trg->phi()-p.phi(), muon_trg->eta()-p.eta());
        float dz_mh = fabs(muon_trg->vz()-p.vz());
        bool geom_req = dR_mh < max_dR_mh_ && dz_mh < max_dz_mh_;

        bool pid_req = abs(p.pdgId()) == 211;

        if(pid_req && geom_req) {
          if(p.charge() > 0) Had_plus.push_back(p);
          else if(p.charge() < 0) Had_minus.push_back(p);
        }
      }

      float Mp_guess, Mm_guess;
      // float dMp_guess, dMm_guess;
      if(muon_trg->charge() > 0) {
        Mp_guess = KaonMass_;
        // dMp_guess = KaonMassErr_;
        Mm_guess = PionMass_;
        // dMm_guess = PionMassErr_;
      }
      else {
        Mp_guess = PionMass_;
        // dMp_guess = PionMassErr_;
        Mm_guess = KaonMass_;
        // dMm_guess = KaonMassErr_;
      }

      uint Ncomb = 0;
      for(auto hp : Had_plus) {
        for(auto hm : Had_minus) {
          TLorentzVector pp, pm;
          pp.SetPtEtaPhiM(hp.pt(), hp.eta(), hp.phi(), Mp_guess);
          pm.SetPtEtaPhiM(hm.pt(), hm.eta(), hm.phi(), Mm_guess);
          // cout << Ncomb << " " << (pp+pm).M() << endl;
          Ncomb++;
        }
      }
    }

    pat::CompositeCandidate BToD0KpimunuCand;
    BToD0KpimunuCand.addUserInt("BPH_trigger", BPH_trigger);
    if(muon_trg != 0) {
      BToD0KpimunuCand.addUserFloat("muon_trg_pt", muon_trg->pt());
      BToD0KpimunuCand.addUserFloat("muon_trg_eta", muon_trg->eta());
      BToD0KpimunuCand.addUserFloat("muon_trg_phi", muon_trg->phi());
      float ObjdR = TMath::Hypot(muon_trg->phi()-obj_trg->phi(), muon_trg->eta()-obj_trg->eta());
      BToD0KpimunuCand.addUserFloat("muon_trg_ObjdR", ObjdR);
      float ObjdpTrel = (muon_trg->pt()-obj_trg->pt())/muon_trg->pt();
      BToD0KpimunuCand.addUserFloat("muon_trg_ObjdpTrel", ObjdpTrel);
    }
    else {
      BToD0KpimunuCand.addUserFloat("muon_trg_pt", 0);
      BToD0KpimunuCand.addUserFloat("muon_trg_eta", -999999);
      BToD0KpimunuCand.addUserFloat("muon_trg_phi", -999999);
      BToD0KpimunuCand.addUserFloat("muon_trg_ObjdR", -999999);
      BToD0KpimunuCand.addUserFloat("muon_trg_ObjdpTrel", -999999);
    }

    result->push_back(BToD0KpimunuCand);

    iEvent.put(std::move(result));
}



vector<pat::Muon> BuToD0munuToKPimunuProducer::TriggerObj_matching(edm::Handle<vector<pat::Muon>> muon_list, pat::TriggerObjectStandAlone obj) {
  double max_DeltaR = 0.005;
  double max_Delta_pt_rel = 0.01;

  vector<pat::Muon> out;

  for( auto muon : *muon_list) {
    double dEta = muon.eta() - obj.eta();
    double dPhi = muon.phi() - obj.phi();
    double deltaR = sqrt(dEta*dEta + dPhi*dPhi);

    double dpt_rel = abs(muon.pt() - obj.pt())/obj.pt();

    if (dpt_rel < max_Delta_pt_rel && deltaR < max_DeltaR) {
      if(verbose) {cout << "\tdeltaR: " << deltaR << "   dpt_rel: " << dpt_rel << endl;}
      out.push_back(muon);
    }
  }

  return out;
}



DEFINE_FWK_MODULE(BuToD0munuToKPimunuProducer);
