# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: UserCode/RivetAnalysis/python/Hadronizer_TuneCUETP8M1_8TeV_powhegEmissionVeto_2p_LHE_pythia8_cff.py --filein /store/cmst3/user/psilva/Wmass/powhegbox_Zj/seed_6_pwgevents.lhe --fileout file:Events_6.root --mc --eventcontent RAWSIM --datatier GEN --step GEN -n -1 --conditions 80X_mcRun2_asymptotic_2016_v1
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
		 'ZPt',
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
options.register('saveEDM',
                 False,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool,
                 "event weight to use"
                 )
options.register('input', 
		 '/store/cmst3/user/psilva/Wmass/powhegbox_Zj/seed_6_pwgevents.lhe',
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "input file to process"
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

process.generator = cms.EDFilter("Pythia8HadronizerFilter",
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring('pythia8CommonSettings', 
            'pythia8CUEP8M1Settings', 
            'pythia8PowhegEmissionVetoSettings', 
            'processParameters'),
        processParameters = cms.vstring('POWHEG:nFinal = 2'),
        pythia8CUEP8M1Settings = cms.vstring('Tune:pp 14', 
            'Tune:ee 7', 
            'MultipartonInteractions:pT0Ref=2.4024', 
            'MultipartonInteractions:ecmPow=0.25208', 
            'MultipartonInteractions:expPow=1.6'),
        pythia8CommonSettings = cms.vstring('Tune:preferLHAPDF = 2', 
            'Main:timesAllowErrors = 10000', 
            'Check:epTolErr = 0.01', 
            'Beams:setProductionScalesFromLHEF = off', 
            'SLHA:keepSM = on', 
            'SLHA:minMassSM = 1000.', 
            'ParticleDecays:limitTau0 = on', 
            'ParticleDecays:tau0Max = 10', 
            'ParticleDecays:allowPhotonRadiation = on'),
        pythia8PowhegEmissionVetoSettings = cms.vstring('POWHEG:veto = 1', 
            'POWHEG:pTdef = 1', 
            'POWHEG:emitted = 0', 
            'POWHEG:pTemt = 0', 
            'POWHEG:pThard = 0', 
            'POWHEG:vetoCount = 100', 
            'SpaceShower:pTmaxMatch = 2', 
            'TimeShower:pTmaxMatch = 2')
    ),
    comEnergy = cms.double(8000.0),
    filterEfficiency = cms.untracked.double(1.0),
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(1)
)


process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
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
						fileName = cms.untracked.string('file:Events_6.root'),
						outputCommands = process.RAWSIMEventContent.outputCommands,
						splitLevel = cms.untracked.int32(0)
						)
	
	process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)
	process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step,process.RAWSIMoutput_step)
else:
	process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 


#customise RIVET routine to run
from UserCode.RivetAnalysis.rivet_customise import *
if options.module=='TOPRadius':    process = customiseTOPRadius(process)
if options.module=='TOP13007':     process = customiseTOP13007(process)
if options.module=='TOPDileptons': process = customiseTOPDileptons(process,options.LHEweightNumber)
if options.module=='ZPt'      :    process = customiseZPt(process,options.LHEweightNumber)
process.rivetAnalyzer.OutputFile = cms.string(options.output)
process.rivetAnalyzer.HepMCCollection = cms.InputTag('generatorSmeared')

process.MessageLogger.cerr.FwkReport.reportEvery = 5000
