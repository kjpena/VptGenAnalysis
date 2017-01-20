import FWCore.ParameterSet.Config as cms
from UserCode.RivetAnalysis.UEParameters_cff import *

from Configuration.Generator.Pythia8PowhegEmissionVetoSettings_cfi import *

def getGeneratorFor(ueTune='CUEP8M2T4',nFinal=2,process=None):

    ueParameters=getUEParameters(ueTune)

    process.generator = cms.EDFilter("Pythia8HadronizerFilter",
                                     maxEventsToPrint = cms.untracked.int32(1),
                                     pythiaPylistVerbosity = cms.untracked.int32(1),
                                     filterEfficiency = cms.untracked.double(1.0),
                                     pythiaHepMCVerbosity = cms.untracked.bool(False),
                                     comEnergy = cms.double(8000.),
                                     PythiaParameters = cms.PSet( pythia8CommonSettingsBlock,
                                                                  pythia8CUEP8M1SettingsBlock,
                                                                  pythia8PowhegEmissionVetoSettingsBlock,
                                                                  ueParameters = ueParameters,
                                                                  processParameters = cms.vstring( 'POWHEG:nFinal = %d'%nFinal,   
                                                                                                   ## Number of final state particles
                                                                                                   ## (BEFORE THE DECAYS) in the LHE
                                                                                                   ## other than emitted extra parton
                                                                                                   'TimeShower:mMaxGamma = 1.0',#cutting off lepton-pair production
                                                                                                   ##in the electromagnetic shower
                                                                                                   ##to not overlap with ttZ/gamma* samples
                                                                                                   'Tune:pp 14',
                                                                                                   'Tune:ee 7'                                                              
                                                                                                   ),
                                                                  parameterSets = cms.vstring('pythia8CommonSettings',
                                                                                              'pythia8PowhegEmissionVetoSettings',
                                                                                              'processParameters',
                                                                                              'ueParameters'
                                                                                              )
                                                                  )
                                     )

