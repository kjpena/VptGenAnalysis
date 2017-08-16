
from WMCore.Configuration import Configuration
import os
config = Configuration()

config.section_("General")
config.General.requestName = "ZJ_central_tune11"
config.General.workArea = "grid"
config.General.transferOutputs=True

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/UserCode/VptGenAnalysis/test/runGENFromLHEandAnalysis_cfg.py"
config.JobType.disableAutomaticOutputCollection = True
#config.JobType.outputFiles = ['%s.root'%config.General.requestName] 
config.JobType.outputFiles = ['%s.w%d.yoda'%(config.General.requestName,x) for x in xrange(0,121)]
config.JobType.pyCfgParams = [
			   'output=%s'%config.General.requestName,
			   'ueTune=CUEP8M2T4:SpaceShower:alphaSvalue=0.120:BeamRemnants:primordialKThard=1.5:SpaceShower:pTmin=1.0',
			   'photos=False',
			   'nFinal=2',
			   'doRivetScan=True',
			   'usePoolSource=True'
			   ]
config.section_("Data")
config.Data.inputDataset = "/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_central/RunIIWinter15wmLHE-MCRUN2_71_V1-v1/LHE"
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
