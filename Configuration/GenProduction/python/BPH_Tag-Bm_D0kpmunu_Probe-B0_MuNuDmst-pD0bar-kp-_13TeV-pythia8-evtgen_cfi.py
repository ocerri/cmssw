import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *

generator = cms.EDFilter("Pythia8GeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    comEnergy = cms.double(13000.0),
    ExternalDecays = cms.PSet(
        EvtGen130 = cms.untracked.PSet(
            decay_table = cms.string('GeneratorInterface/EvtGenInterface/data/DECAY_2014_NOLONGLIFE.DEC'),
            particle_property_file = cms.FileInPath('GeneratorInterface/EvtGenInterface/data/evt_2014.pdl'),
            # list_forced_decays = cms.vstring('B+','B-'),
            # operates_on_particles = cms.vint32(521),    # we care just about our signal particles
            convertPythiaCodes = cms.untracked.bool(False),
            user_decay_embedded= cms.vstring(
"""
Alias      MyD0   D0
Alias      Myanti-D0   anti-D0
ChargeConj MyD0   Myanti-D0
#
Decay MyD0
1.000      K-  pi+                        PHSP;
Enddecay
CDecay Myanti-D0
#
#
Decay B-
1.000      MyD0  mu-  anti-nu_mu            PHOTOS  ISGW2;
Enddecay
#
#
Alias     MyD*-    D*-
Decay My-D*-
1.000      Myanti-D0 pi-                 VSS;
Enddecay
#
Decay B0
1.000     MyD*- mu+ nu_mu               PHOTOS  ISGW2;
Enddecay
#
End
"""
                                            )
        ),
        parameterSets = cms.vstring('EvtGen130')
    ),
    PythiaParameters = cms.PSet(
        pythia8CommonSettingsBlock,
        pythia8CP5SettingsBlock,
        processParameters = cms.vstring(
                                        # 'SoftQCD:nonDiffractive = on',
                                        'HardQCD:hardbbbar = on',
                                        'PhaseSpace:pTHatMin = 5'
            ),
        parameterSets = cms.vstring('pythia8CommonSettings',
                                    'pythia8CP5Settings',
                                    'processParameters',
                                    )
    )
)


###### Filters ##########

tagfilter = cms.EDFilter(
    "PythiaDauVFilter",
    NumberDaughters    = cms.untracked.int32(3),
    ParticleID         = cms.untracked.int32(-521),  ## B-
    ChargeConjugation  = cms.untracked.bool(False),
    DaughterIDs        = cms.untracked.vint32(421, 13, -14), ## D0 mu+ nu_mu
    MinPt              = cms.untracked.vdouble(-1., 6.5, -1.),
    MinEta             = cms.untracked.vdouble(-9999999., -2.6, -9999999.),
    MaxEta             = cms.untracked.vdouble( 9999999.,  2.6, 9999999.)
)

probefilter = cms.EDFilter(
    "PythiaDauVFilter",
    ParticleID         = cms.untracked.int32(511),  ## B0
    ChargeConjugation  = cms.untracked.bool(False)
)


ProductionFilterSequence = cms.Sequence(generator + tagfilter + probefilter)
