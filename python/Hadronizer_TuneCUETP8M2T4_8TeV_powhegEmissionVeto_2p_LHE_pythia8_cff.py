import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *
from Configuration.Generator.Pythia8PowhegEmissionVetoSettings_cfi import *

generator = cms.EDFilter("Pythia8HadronizerFilter",
                         maxEventsToPrint = cms.untracked.int32(1),
                         pythiaPylistVerbosity = cms.untracked.int32(1),
                         filterEfficiency = cms.untracked.double(1.0),
                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                         comEnergy = cms.double(8000.),
                         PythiaParameters = cms.PSet( pythia8CommonSettingsBlock,
                                                      pythia8CUEP8M1SettingsBlock,
                                                      pythia8PowhegEmissionVetoSettingsBlock,
                                                      processParameters = cms.vstring( 'POWHEG:nFinal = 2',   
                                                                                       ## Number of final state particles
                                                                                       ## (BEFORE THE DECAYS) in the LHE
                                                                                       ## other than emitted extra parton
                                                                                       'TimeShower:mMaxGamma = 1.0',#cutting off lepton-pair production
                                                                                       ##in the electromagnetic shower
                                                                                       ##to not overlap with ttZ/gamma* samples
                                                                                       'Tune:pp 14',
                                                                                       'Tune:ee 7',
                                                                                       'MultipartonInteractions:ecmPow=0.25208',
                                                                                       'SpaceShower:alphaSvalue=0.1108',
                                                                                       'PDF:pSet=LHAPDF6:NNPDF30_lo_as_0130',
                                                                                       'MultipartonInteractions:pT0Ref=2.197139e+00',
                                                                                       'MultipartonInteractions:expPow=1.608572e+00',
                                                                                       'ColourReconnection:range=6.593269e+00',
                                                                                       ),
                                                      parameterSets = cms.vstring('pythia8CommonSettings',
                                                                                  #'pythia8CUEP8M1Settings',
                                                                                  'pythia8PowhegEmissionVetoSettings',
                                                                                  'processParameters'
                                                                                  )
                                                      )
                         )

ProductionFilterSequence = cms.Sequence(generator)
