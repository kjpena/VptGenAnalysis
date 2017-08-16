import FWCore.ParameterSet.Config as cms

def removeOutputStep(process):
	try:
		process.schedule.remove(process.RAWSIMoutput_step)
	except:
		pass

def customiseTOP13007(process):
        process.load('GeneratorInterface.RivetInterface.rivetAnalyzer_cfi')
        process.rivetAnalyzer.AnalysisNames = cms.vstring('CMS_TOP_13_007')
	process.rivetAnalyzer.HepMCCollection = cms.InputTag('generator')
        process.generation_step+=process.rivetAnalyzer
        return(process)

def customiseTOPDileptons(process):
        process.load('GeneratorInterface.RivetInterface.rivetAnalyzer_cfi')
        process.rivetAnalyzer.AnalysisNames = cms.vstring('CMS_TOP_Dileptons')
        process.generation_step+=process.rivetAnalyzer
        return(process)

def customiseTOPRadius(process,LHEweightNumber=0):
        process.load('GeneratorInterface.RivetInterface.rivetAnalyzer_cfi')
        process.rivetAnalyzer.AnalysisNames = cms.vstring('CMS_TOP_Radius')
	process.rivetAnalyzer.UseExternalWeight = cms.bool(True)
	process.rivetAnalyzer.useLHEweights = cms.bool(True)
	process.rivetAnalyzer.LHEweightNumber = cms.int32(LHEweightNumber)
	process.rivetAnalyzer.HepMCCollection = cms.InputTag('generator')
	process.rivetAnalyzer.LHECollection = cms.InputTag('externalLHEProducer')
        process.generation_step+=process.rivetAnalyzer
        return(process)

def customiseZPt(process,LHEweightNumber=0):
        process.load('GeneratorInterface.RivetInterface.rivetAnalyzer_cfi')
        process.rivetAnalyzer.AnalysisNames = cms.vstring('ATLAS_2015_I1408516_MU')        
	process.rivetAnalyzer.UseExternalWeight = cms.bool(False)
        process.rivetAnalyzer.useLHEweights = cms.bool(False)
	#process.rivetAnalyzer.UseExternalWeight = cms.bool(True)
	#process.rivetAnalyzer.useLHEweights = cms.bool(True)
	process.rivetAnalyzer.LHEweightNumber = cms.int32(LHEweightNumber)
	process.rivetAnalyzer.HepMCCollection = cms.InputTag('generator')
        process.generation_step+=process.rivetAnalyzer
        return(process)


