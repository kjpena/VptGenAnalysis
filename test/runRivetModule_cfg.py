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
		 'TOP13007',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "module to run"
                 )
options.register('LHEweightNumber',
                 0,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "event weight to use"
                 )
options.register('input', 
		 'root://eoscms//eos/cms/store/cmst3/user/psilva/CompositeTop/Rt__5.00E_04_kappat__5.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.root',
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

print options.maxEvents
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# Input source
from UserCode.RivetAnalysis.ttSamples_cff import getSamples
samples=getSamples(options.input)
if len(samples)==0: samples=cms.untracked.vstring(options.input.split(','))
process.source = cms.Source("PoolSource",
                            fileNames = samples,
                            secondaryFileNames = cms.untracked.vstring(),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
                            )

process.generation_step = cms.Path()
process.RAWSIMoutput_step = cms.EndPath()
process.schedule = cms.Schedule(process.generation_step,process.RAWSIMoutput_step)

#customise RIVET routine to run
from UserCode.RivetAnalysis.rivet_customise import *
if options.module=='TOPRadius':    process = customiseTOPRadius(process)
if options.module=='TOP13007':     process = customiseTOP13007(process)
if options.module=='TOPDileptons': process = customiseTOPDileptons(process,options.LHEweightNumber)
if options.module=='ZPt'      :    process = customiseZPt(process,options.LHEweightNumber)

process.rivetAnalyzer.OutputFile = cms.string(options.output)
process.RandomNumberGeneratorService.generator.initialSeed=cms.untracked.uint32(1)
process.source.skipEvents=cms.untracked.uint32(0)
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

