from WMCore.Configuration import Configuration
import os
config = Configuration()

config.section_("General")
config.General.requestName = "TT_TuneCUETP8M1mpiOFF_13TeV-powheg-pythia8"
config.General.workArea = "grid"
config.General.transferOutputs=True

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "/afs/cern.ch/user/p/psilva/work/Wmass/CMSSW_8_0_8_patch1/src/UserCode/RivetAnalysis/test/runRivetModule_cfg.py"
config.JobType.outputFiles = ['out.yoda']

config.section_("Data")
config.Data.inputDataset = "/TT_TuneCUETP8M1mpiOFF_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9_ext4-v2/AODSIM"
config.Data.inputDBS = "global"
config.Data.splitting = "FileBased"
config.Data.unitsPerJob = 5
config.Data.publication = False
config.Data.ignoreLocality = True
config.Data.outLFNDirBase = '/store/group/phys_top/psilva/UE_2f64282/'

config.section_("Site")
config.Site.storageSite = "T2_CH_CERN"
