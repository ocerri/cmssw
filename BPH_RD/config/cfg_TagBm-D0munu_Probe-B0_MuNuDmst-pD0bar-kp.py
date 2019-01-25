# files=['/afs/cern.ch/user/o/ocerri/cernbox/BPhysics/data/cmsMC_private/BPH_Tag-Bm_D0kpmunu_Probe-Bp_D0kpmunu_NoPU_10-2-3_v0/BPH_Tag-Bm_D0kpmunu_Probe-Bp_D0kpmunu_MINIAODSIM.root']
#
# outfile=files[0].replace('MINIAODSIM.root', 'BPHMC.root')

from glob import glob
files=glob('/afs/cern.ch/user/o/ocerri/cernbox/BPhysics/data/cmsMC_private/BPH_Tag-Bm_D0kpmunu_Probe-B0_MuNuDmst-pD0bar-kp-_NoPU_10-2-3_v1/jobs_out/*merge*.root')
# files=glob('/afs/cern.ch/user/o/ocerri/cernbox/BPhysics/data/cmsMC_private/BPH_Tag-Bm_D0kpmunu_Probe-B0_MuNuDmst-pD0bar-kp-_NoPU_10-2-3_v0/jobs_out/*_MINIAODSIM_*.root')


# outfile = files[0].replace('jobs_out/', '')
# outfile = outfile.replace('MINIAODSIM', 'BPHMC')
outfile = '/afs/cern.ch/user/o/ocerri/cernbox/BPhysics/data/cmsMC_private/BPH_Tag-Bm_D0kpmunu_Probe-B0_MuNuDmst-pD0bar-kp-_NoPU_10-2-3_v1/BPH_test.root'


#load specific moduels
from MC_Process import MC_Process
from BPHTriggerPath import BPHTriggerPath
from ReCo_Decay_B0_DstMu_D0piMu_KpipiMu import ReCo_Decay_B0_DstMu_D0piMu_KpipiMu

exe_seq = [
           BPHTriggerPath(mu_charge=-1, filter=True),
           MC_Process(probe_pdgId = 511, tag_pdgId = -521),
           ReCo_Decay_B0_DstMu_D0piMu_KpipiMu()
          ]
