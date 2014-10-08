import FWCore.ParameterSet.Config as cms

def customise(process):
        process.load('GeneratorInterface.RivetInterface.rivetAnalyzer_cfi')
        process.rivetAnalyzer.AnalysisNames = cms.vstring('CMS_TOP_13_007')
        process.generation_step+=process.rivetAnalyzer
        process.schedule.remove(process.RAWSIMoutput_step)
        return(process)
