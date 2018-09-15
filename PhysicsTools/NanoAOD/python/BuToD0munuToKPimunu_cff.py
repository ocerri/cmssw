from PhysicsTools.NanoAOD.common_cff import *
import FWCore.ParameterSet.Config as cms


BuToD0munuToKPimunuMCTruth=cms.EDProducer("BuToD0munuToKPimunuMCTruthProducer",
                        GenParticlesCollection=cms.InputTag("prunedGenParticles"),
                        )


BuToD0munuToKPimunuMCTruthTable=cms.EDProducer("SimpleCompositeCandidateFlatTableProducer",
                             src=cms.InputTag("BuToD0munuToKPimunuMCTruth"),
                             cut=cms.string(""),
                             name=cms.string("BuToD0munuToKPimunuMCTruth"),
                             doc=cms.string("BuToD0munuToKPimunuMCTruth Variable"),
                             singleton=cms.bool(False),
                             extension=cms.bool(False),
                             variables=cms.PSet(
                                B_pdgID=Var("userInt('B_pdgID')", int,doc="B_pdgID"),
                                B_pt=Var("userFloat('B_pt')", float,doc="B_pt"),
                                )
                             )

BuToD0munuToKPimunuMCSequence=cms.Sequence(BuToD0munuToKPimunuMCTruth + BuToD0munuToKPimunuMCTruthTable)
