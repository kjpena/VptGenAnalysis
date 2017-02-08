import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8PowhegEmissionVetoSettings_cfi import *
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from UserCode.RivetAnalysis.UEParameters_cff import *

def getGeneratorFor(ueTune='CUEP8M2T4',nFinal=2,pdfSet='NNPDF30_lo_as_0130',process=None,addPhotos=False):

    ueParameters=getUEParameters(ueTune,pdfSet)

    if addPhotos:
        process.generator = cms.EDFilter("Pythia8HadronizerFilter",
                                         maxEventsToPrint = cms.untracked.int32(1),
                                         pythiaPylistVerbosity = cms.untracked.int32(1),
                                         filterEfficiency = cms.untracked.double(1.0),
                                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                                         comEnergy = cms.double(8000.),
                                         ExternalDecays = cms.PSet( Photos = cms.untracked.PSet(),
                                                                    parameterSets = cms.vstring( "Photos" )
                                                                    ),
                                         PythiaParameters = cms.PSet( pythia8CommonSettingsBlock,
                                                                      pythia8PowhegEmissionVetoSettingsBlock,
                                                                      processParameters = cms.vstring( 'POWHEG:nFinal = %d'%nFinal,   
                                                                                                       ## Number of final state particles
                                                                                                       ## (BEFORE THE DECAYS) in the LHE
                                                                                                       ## other than emitted extra parton
                                                                                                       'TimeShower:mMaxGamma = 1.0',#cutting off lepton-pair production
                                                                                                       ##in the electromagnetic shower
                                                                                                       ##to not overlap with ttZ/gamma* samples
                                                                                                       ),
                                                                      ueParameters = ueParameters,
                                                                      fsrParameters = cms.vstring('TimeShower:QEDshowerByL=off',
                                                                                                  'TimeShower:QEDshowerByQ = off',
                                                                                                  'TimeShower:QEDshowerByGamma = off',
                                                                                                  'ParticleDecays:allowPhotonRadiation = off'),
                                                                      parameterSets = cms.vstring('pythia8CommonSettings',
                                                                                                  'pythia8PowhegEmissionVetoSettings',
                                                                                                  'processParameters',
                                                                                                  'ueParameters',
                                                                                                  'fsrParameters'
                                                                                                  )
                                                                      )
                                         )
    else:
        process.generator = cms.EDFilter("Pythia8HadronizerFilter",
                                         maxEventsToPrint = cms.untracked.int32(1),
                                         pythiaPylistVerbosity = cms.untracked.int32(1),
                                         filterEfficiency = cms.untracked.double(1.0),
                                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                                         comEnergy = cms.double(8000.),
                                         PythiaParameters = cms.PSet( pythia8CommonSettingsBlock,
                                                                      pythia8PowhegEmissionVetoSettingsBlock,
                                                                      ueParameters = ueParameters,
                                                                      processParameters = cms.vstring( 'POWHEG:nFinal = %d'%nFinal,   
                                                                                                       ## Number of final state particles
                                                                                                       ## (BEFORE THE DECAYS) in the LHE
                                                                                                       ## other than emitted extra parton
                                                                                                       'TimeShower:mMaxGamma = 1.0',#cutting off lepton-pair production
                                                                                                       ##in the electromagnetic shower
                                                                                                       ##to not overlap with ttZ/gamma* samples
                                                                                                       ),
                                                                      parameterSets = cms.vstring('pythia8CommonSettings',
                                                                                                  'pythia8PowhegEmissionVetoSettings',
                                                                                                  'processParameters',
                                                                                                  'ueParameters'
                                                                                                  )
                                                                      )
                                         )


