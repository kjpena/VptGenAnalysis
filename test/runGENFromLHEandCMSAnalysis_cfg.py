# Auto generated configuration file
# using:mz 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: UserCode/RivetAnalysis/python/Hadronizer_TuneCUETP8M1_8TeV_powhegEmissionVeto_2p_LHE_pythia8_cff.py --filein /store/cmst3/user/psilva/Wmass/powhegbox_Zj/seed_6_pwgevents.lhe --fileout file:Events_6.root --mc --eventcontent RAWSIM --datatier GEN --step GEN -n -1 --conditions 80X_mcRun2_asymptotic_2016_v1
import FWCore.ParameterSet.Config as cms


from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.register('output', 
		 'data.root',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "Output file name"
                 )
options.register('usePoolSource',
                 False,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool,
                 "use LHE from EDM format"
                 )
options.register('saveEDM',
                 False,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool,
                 "save EDM output"
                 )
options.register('photos',
                 False,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool,
                 "add Photos for QED"
                 )
options.register('doRivetScan',
                 False,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool,
                 "do rivet scan, no ntuple"
                 )
options.register('meWeight',
                 0,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "ME weight to apply in RIVET"
                 )
options.register('seed',
                 123456789,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "seed to use"
                 )
options.register('input', 
		 '/store/cmst3/user/psilva/Wmass/Wminusj/seed_9_pwgevents.lhe',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "input file to process"
                 )
options.register('ueTune',
		 'CUETP8M2T41',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "hardcoded UE snippet to use"
                 )
options.register('pdfSet',
		 'LHAPDF6:NNPDF30_lo_as_0130',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "hardcoded UE snippet to use"
                 )
options.register('nFinal',
		 2,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "n particles in final state"
                 )

options.parseArguments()


process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# Input source
process.source=cms.Source('EmptySource')
if options.usePoolSource:
	process.source = cms.Source("PoolSource",
				    fileNames = cms.untracked.vstring(options.input.split(',')),
				    inputCommands = cms.untracked.vstring('keep *')
				    )
else:
	process.source = cms.Source("LHESource",
				    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
				    fileNames = cms.untracked.vstring(options.input.split(',')),
				    inputCommands = cms.untracked.vstring('keep *')
				    )

process.options = cms.untracked.PSet(
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('UserCode/RivetAnalysis/python/Hadronizer_TuneCUETP8M1_8TeV_powhegEmissionVeto_2p_LHE_pythia8_cff.py nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_v1', '')

#generator definition
from UserCode.RivetAnalysis.PowhegEmissioVeton_Pythia8_cff import getGeneratorFor
getGeneratorFor(ueTune=options.ueTune,nFinal=options.nFinal,pdfSet=options.pdfSet,process=process,addPhotos=options.photos)

process.RandomNumberGeneratorService.generator.initialSeed=cms.untracked.uint32(options.seed)
print 'Seed initiated to %d'%options.seed
process.ProductionFilterSequence = cms.Sequence(process.generator)

#tfile service                                                                                                                                                                
process.TFileService = cms.Service("TFileService",
				   fileName = cms.string(options.output+'.root')
				   )


# add analysis
process.load('UserCode.VptGenAnalysis.vptAnalysis_cff')


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
#process.analysis_step = cms.Path(process.analysis)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
if options.saveEDM:
	process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
						SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('generation_step')
			),
						dataset = cms.untracked.PSet(
			dataTier = cms.untracked.string('GEN'),
			filterName = cms.untracked.string('')
			),
						eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
						fileName = cms.untracked.string('file:EDMEvents.root'),
						outputCommands = process.RAWSIMEventContent.outputCommands,
						splitLevel = cms.untracked.int32(0)
						)
	
	process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)
	#process.schedule = cms.Schedule(process.generation_step, process.analysis_step, process.genfiltersummary_step, process.endjob_step,process.RAWSIMoutput_step)
        process.schedule = cms.Schedule(process.generation_step, process.genfiltersummary_step, process.endjob_step,process.RAWSIMoutput_step)
else:
	#process.schedule = cms.Schedule(process.generation_step, process.analysis_step, process.genfiltersummary_step, process.endjob_step)
        process.schedule = cms.Schedule(process.generation_step, process.genfiltersummary_step, process.endjob_step)


# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq

#add RIVET routine
from UserCode.RivetAnalysis.rivet_customise import *
if options.doRivetScan:	
	for i in xrange(0,121):
		from GeneratorInterface.RivetInterface.rivetAnalyzer_cfi import rivetAnalyzer
		LHECollection = cms.InputTag('externalLHEProducer') if options.usePoolSource else cms.InputTag('source')
		setattr(process,
			'rivetAnalyzer%d'%i,
			rivetAnalyzer.clone( AnalysisNames = cms.vstring('CMS_2017_I1471281_Z'),
					     UseExternalWeight = cms.bool(True),
					     useLHEweights = cms.bool(True),
					     LHEweightNumber = cms.int32(i),
					     LHECollection = LHECollection,
					     HepMCCollection = cms.InputTag('generatorSmeared'),
					     OutputFile = cms.string( '%s.w%d.yoda'%(options.output,i)),
					     )
			)
		process.generation_step+=getattr(process,'rivetAnalyzer%d'%i)
else:
	process = customiseZPt(process,options.meWeight)
	process.rivetAnalyzer.AnalysisNames = cms.vstring('CMS_2017_I1471281_Z')
	process.rivetAnalyzer.OutputFile = cms.string(options.output + '.w%d.yoda'%options.meWeight)
	process.rivetAnalyzer.HepMCCollection = cms.InputTag('generatorSmeared')

process.MessageLogger.cerr.FwkReport.reportEvery = 5000
