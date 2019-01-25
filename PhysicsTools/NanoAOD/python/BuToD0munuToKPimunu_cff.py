from PhysicsTools.NanoAOD.common_cff import *
import FWCore.ParameterSet.Config as cms

BPHTriggerPath = cms.EDFilter("BPHTriggerPathFilter",
        muonCollection = cms.InputTag("slimmedMuons","", "PAT"),
        triggerObjects = cms.InputTag("slimmedPatTrigger"),
        max_dR_mh = cms.int32(-1),
)

BuToD0munuToKPimunu = cms.EDProducer("BuToD0munuToKPimunuProducer",
    beamSpot = cms.InputTag("offlineBeamSpot"),
    PFCandCollection = cms.InputTag("packedPFCandidates"),
    vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
    muonCollection = cms.InputTag("slimmedMuons","", "PAT"),
    triggerBits = cms.InputTag("TriggerResults","","HLT"),
    triggerObjects = cms.InputTag("slimmedPatTrigger"),
    max_dR_mh = cms.double(1.5),
    max_dz_mh = cms.double(0.5)
)

BuToD0munuToKPimunuTable = cms.EDProducer("SimpleCandidateFlatTableProducer",
    src = cms.InputTag("BuToD0munuToKPimunu"),
    cut = cms.string(""),
    name = cms.string("Bu2D0Kpimunu"),
    doc = cms.string("BuToD0munuToKPimunu Variable"),
    singleton = cms.bool(False),
    extension = cms.bool(False),
    variables = cms.PSet(
        BPH_trigger = Var("userInt('BPH_trigger')", int, doc="BPH_trigger"),
        muon_trg_pt = Var("userFloat('muon_trg_pt')", float, doc="muon_trg_pt"),
        muon_trg_eta = Var("userFloat('muon_trg_eta')", float, doc="muon_trg_eta"),
        muon_trg_phi = Var("userFloat('muon_trg_phi')", float, doc="muon_trg_phi"),
        muon_trg_ObjdR = Var("userFloat('muon_trg_ObjdR')", float, doc="muon_trg_ObjdR"),
        muon_trg_ObjdpTrel = Var("userFloat('muon_trg_ObjdpTrel')", float, doc="muon_trg_ObjdpTrel")
    )
)

BuToD0munuToKPimunuMCTruth = cms.EDProducer("BuToD0munuToKPimunuMCTruthProducer",
                        PrunedGenParticlesCollection = cms.InputTag("prunedGenParticles"),
                        PackedGenParticlesCollection = cms.InputTag("packedGenParticles"),
                        PFCandCollection = cms.InputTag("packedPFCandidates"),
                        muonCollection = cms.InputTag("slimmedMuons","", "PAT")
                        )


BuToD0munuToKPimunuMCTruthTable = cms.EDProducer("SimpleCandidateFlatTableProducer",
                             src = cms.InputTag("BuToD0munuToKPimunuMCTruth"),
                             cut = cms.string(""),
                             name = cms.string("Bu2D0Kpimunu_MC"),
                             doc = cms.string("BuToD0munuToKPimunuMCTruth Variable"),
                             singleton = cms.bool(False),
                             extension = cms.bool(False),
                             variables = cms.PSet(
                                B_pdgID = Var("userInt('B_pdgID')", int, doc="B_pdgID"),
                                B_pt = Var("userFloat('B_pt')", float, doc="B_pt"),
                                B_eta = Var("userFloat('B_eta')", float, doc="B_eta"),
                                B_phi = Var("userFloat('B_phi')", float, doc="B_phi"),
                                B_D0munu = Var("userInt('B_D0munu')", int, doc="B_D0munu"),
                                D0_pdgID = Var("userInt('D0_pdgID')", int, doc="D0_pdgID"),
                                D0_pt = Var("userFloat('D0_pt')", float, doc="D0_pt"),
                                D0_eta = Var("userFloat('D0_eta')", float, doc="D0_eta"),
                                D0_phi = Var("userFloat('D0_phi')", float, doc="D0_phi"),
                                D0_dz = Var("userFloat('D0_dz')", float, doc="D0_dz"),
                                mu_pdgID = Var("userInt('mu_pdgID')", int, doc="mu_pdgID"),
                                mu_pt = Var("userFloat('mu_pt')", float, doc="mu_pt"),
                                mu_eta = Var("userFloat('mu_eta')", float, doc="mu_eta"),
                                mu_phi = Var("userFloat('mu_phi')", float, doc="mu_phi"),
                                mu_dz = Var("userFloat('mu_dz')", float, doc="mu_dz"),
                                mu_reco_pt = Var("userFloat('mu_reco_pt')", float, doc="mu_reco_pt"),
                                mu_reco_eta = Var("userFloat('mu_reco_eta')", float, doc="mu_reco_eta"),
                                mu_reco_phi = Var("userFloat('mu_reco_phi')", float, doc="mu_reco_phi"),
                                mu_reco_dz = Var("userFloat('mu_reco_dz')", float, doc="mu_reco_dz"),
                                nu_pdgID = Var("userInt('nu_pdgID')", int, doc="nu_pdgID"),
                                nu_pt = Var("userFloat('nu_pt')", float, doc="nu_pt"),
                                nu_eta = Var("userFloat('nu_eta')", float, doc="nu_eta"),
                                nu_phi = Var("userFloat('nu_phi')", float, doc="nu_phi"),
                                D0_Kpi = Var("userInt('D0_Kpi')", int, doc="D0_Kpi"),
                                pi_pdgID = Var("userInt('pi_pdgID')", int, doc="pi_pdgID"),
                                pi_pt = Var("userFloat('pi_pt')", float, doc="pi_pt"),
                                pi_eta = Var("userFloat('pi_eta')", float, doc="pi_eta"),
                                pi_phi = Var("userFloat('pi_phi')", float, doc="pi_phi"),
                                pi_dz = Var("userFloat('pi_dz')", float, doc="pi_dz"),
                                pi_reco_pt = Var("userFloat('pi_reco_pt')", float, doc="pi_reco_pt"),
                                pi_reco_eta = Var("userFloat('pi_reco_eta')", float, doc="pi_reco_eta"),
                                pi_reco_phi = Var("userFloat('pi_reco_phi')", float, doc="pi_reco_phi"),
                                pi_reco_dz = Var("userFloat('pi_reco_dz')", float, doc="pi_reco_dz"),
                                K_pdgID = Var("userInt('K_pdgID')", int, doc="K_pdgID"),
                                K_pt = Var("userFloat('K_pt')", float, doc="K_pt"),
                                K_eta = Var("userFloat('K_eta')", float, doc="K_eta"),
                                K_phi = Var("userFloat('K_phi')", float, doc="K_phi"),
                                K_dz = Var("userFloat('K_dz')", float, doc="K_dz"),
                                K_reco_pt = Var("userFloat('K_reco_pt')", float, doc="K_reco_pt"),
                                K_reco_eta = Var("userFloat('K_reco_eta')", float, doc="K_reco_eta"),
                                K_reco_phi = Var("userFloat('K_reco_phi')", float, doc="K_reco_phi"),
                                K_reco_dz = Var("userFloat('K_reco_dz')", float, doc="K_reco_dz"),
                                )
                             )

BuToD0munuToKPimunuRECOSequence = cms.Sequence(BuToD0munuToKPimunu + BuToD0munuToKPimunuTable)
# BuToD0munuToKPimunuRECOSequence = cms.Sequence(BPHTriggerPath + BuToD0munuToKPimunu + BuToD0munuToKPimunuTable)

BuToD0munuToKPimunuMCSequence = cms.Sequence(BuToD0munuToKPimunuMCTruth + BuToD0munuToKPimunuMCTruthTable)
