
from WMCore.Configuration import Configuration
import os
config = Configuration()

config.section_("General")
config.General.requestName = "pythia8_tuneAZ"
config.General.workArea = "grid"
config.General.transferOutputs=True

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/UserCode/VptGenAnalysis/test/runAnalysis_cfg.py"
config.JobType.disableAutomaticOutputCollection = True
config.JobType.outputFiles = ['%s.root'%config.General.requestName] 
config.JobType.outputFiles += ['%s.w0.yoda'%config.General.requestName]
config.JobType.pyCfgParams = [
			   'output=%s'%config.General.requestName,
			   #'ueTune=_TUNE_',
			   #'photos=False',
			   #'nFinal=2',
			   'doRivetScan=False'
			   #'usePoolSource=True'
			   ]
config.section_("Data")
config.Data.inputDataset = "/DYToMuMu_M_50_TuneAZ_8TeV_pythia8/RunIISummer15GS-GenOnly_MCRUN2_71_V1-v3/GEN-SIM"
config.Data.inputDBS = "global"
config.Data.splitting = "EventAwareLumiBased" #"EventBased"
config.Data.unitsPerJob = 5000
#config.Data.totalUnits = config.Data.unitsPerJob * 10
config.Data.publication = False
config.Data.ignoreLocality = False
config.Data.outLFNDirBase = '/store/group/cmst3/group/wmass/kjpena/%s'%config.General.requestName

config.section_("Site")
config.Site.storageSite = "T2_CH_CERN"
config.Site.whitelist = [ "T2_CH_CERN" ]
