# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: --python_filename produceNanoAOD_cfg.py -s NANO --mc --eventcontent NANOAODSIM --datatier NANOAODSIM --filein file:/eos/user/o/ocerri/BPhysics/data/cmsMC_private/Bu_D0munu_KPimunu_NoPU_10-2-3_v0/Bu_D0munu_KPimunu_MINIAODSIM.root --no_exec --conditions 102X_upgrade2018_realistic_v12 -n -1 --era Run2_2018 --fileout file:Bu_D0munu_KPimunu_NANOAODSIM.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('NANO',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('PhysicsTools.NanoAOD.nano_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    # input = cms.untracked.int32(100)
    input = cms.untracked.int32(-1)
)

# Input source
from glob import glob
flist = glob('/eos/user/o/ocerri/BPhysics/data/cmsMC_private/HardQCD_bbar_Bu_D0munu_KPimunu_NoPU_10-2-3_v0/jobs_out/*MINIAODSIM*.root')
for i in range(len(flist)):
    flist[i] = 'file:' + flist[i]

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(tuple(flist)),
    # fileNames = cms.untracked.vstring('file:/eos/user/o/ocerri/BPhysics/data/cmsMC_private/HardQCD_bbar_Bu_D0munu_KPimunu_NoPU_10-2-3_v0/HardQCD_bbar_Bu_D0munu_KPimunu_MINIAODSIM.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('--python_filename nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.NANOAODSIMoutput = cms.OutputModule("NanoAODOutputModule",
# process.NANOAODSIMoutput = cms.OutputModule("PoolOutputModule",   #NANOAOD edm event content
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAODSIM'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:/eos/user/o/ocerri/BPhysics/data/cmsMC_private/HardQCD_bbar_Bu_D0munu_KPimunu_NoPU_10-2-3_v0/jobs_out/HardQCD_bbar_Bu_D0munu_KPimunu_NANOAODSIM_all.root'),
    # fileName = cms.untracked.string('file:/eos/user/o/ocerri/BPhysics/data/cmsMC_private/HardQCD_bbar_Bu_D0munu_KPimunu_NoPU_10-2-3_v0/HardQCD_bbar_Bu_D0munu_KPimunu_NANOAODSIM.root'),
    # outputCommands = process.NANOAODSIMEventContent.outputCommands
    outputCommands = cms.untracked.vstring(
                                            'drop *',
                                            'keep nanoaodFlatTable_*Table_*_*',
                                            'drop nanoaodFlatTable_caloMetTable_*_*',
                                            'drop nanoaodFlatTable_fatJetTable_*_*',
                                            'drop nanoaodFlatTable_genJet*_*_*',
                                            'drop nanoaodFlatTable_genSubJet*_*_*',
                                            'drop nanoaodFlatTable_genWeights*_*_*',
                                            # 'drop nanoaodFlatTable_jet*_*_*',
                                            # 'drop nanoaodFlatTable_met*_*_*',
                                            'drop nanoaodFlatTable_puppiMetTable_*_*',
                                            'drop nanoaodFlatTable_rivet*Table_*_*',
                                            'drop nanoaodFlatTable_subJetTable_*_*',
                                            'drop nanoaodFlatTable_ttbarCategoryTable_*_*',
                                            # 'keep edmTriggerResults_*_*_HLT',
                                            # 'keep nanoaodMergeableCounterTable_*Table_*_*',
                                            # 'keep nanoaodUniqueString_nanoMetadata_*_*'
                                            )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_upgrade2018_realistic_v12', '')

# Path and EndPath definitions
process.load('PhysicsTools.NanoAOD.BuToD0munuToKPimunu_cff')
process.nanoSequenceMC.insert(-1, process.BuToD0munuToKPimunuRECOSequence)
process.nanoSequenceMC.insert(-1, process.BuToD0munuToKPimunuMCSequence)

process.nanoAOD_step = cms.Path(process.nanoSequenceMC)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(process.NANOAODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeMC

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
