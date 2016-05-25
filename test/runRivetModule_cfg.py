import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.register('output', 
		 'out.yoda',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "Output file name"
                 )
options.register('module',
		 'TOPRadius',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "module to run"
                 )
options.register('input', 
		 'root://eoscms//eos/cms/store/cmst3/user/psilva/CompositeTop/Rt_0.00E+00_kappat_0.00E+00_xqcut_20.0_njetmax_0_qcut_40.0.root',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "input file to process"
                 )
options.parseArguments()


process = cms.Process('RIVET')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.input),
    secondaryFileNames = cms.untracked.vstring()
)

process.generation_step = cms.Path()
process.RAWSIMoutput_step = cms.EndPath()
process.schedule = cms.Schedule(process.generation_step,process.RAWSIMoutput_step)

#customise RIVET routine to run
from UserCode.RivetAnalysis.rivet_customise import *
if options.module=='TOPRadius':    process = customiseTOPRadius(process)
if options.module=='TOP13007':     process = customiseTOP13007(process)
if options.module=='TOPDileptons': process = customiseTOPDileptons(process)

process.rivetAnalyzer.OutputFile = cms.string(options.output)
process.RandomNumberGeneratorService.generator.initialSeed=cms.untracked.uint32(1)
process.source.skipEvents=cms.untracked.uint32(0)
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

