import FWCore.ParameterSet.Config as cms

from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import pythia8CUEP8M1SettingsBlock as UESettingsBlock

generator = cms.EDFilter("Pythia8generatorFilter",
                         ExternalDecays = cms.PSet( Tauola = cms.untracked.PSet( UseTauolaPolarization = cms.bool(True),
                                                                                 InputCards = cms.PSet( mdtau = cms.int32(0),
                                                                                                        pjak2 = cms.int32(0),
                                                                                                        pjak1 = cms.int32(0)
                                                                                                        )
                                                                                 ),
                                                    parameterSets = cms.vstring('Tauola')
                                                    ),
                         maxEventsToPrint = cms.untracked.int32(1),
                         pythiaPylistVerbosity = cms.untracked.int32(1),
                         filterEfficiency = cms.untracked.double(1.0),
                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                         comEnergy = cms.double(8000.),
                         PythiaParameters = cms.PSet(pythia8CommonSettingsBlock,
                                                     UESettingsBlock,
                                                     processParameters = cms.vstring('Main:timesAllowErrors = 10000',
                                                                                     'ParticleDecays:limitTau0 = on',
                                                                                     'ParticleDecays:tauMax = 10',
                                                                                     'Top:gg2ttbar = on',
                                                                                     'Top:qqbar2ttbar = on',
                                                                                     '6:m0 = 172.5', #Top mass                                                      
                                                                                     'ProcessLevel:resonanceDecays = on', #W leptonic decays
                                                                                     '24:onMode = off',
                                                                                     "24:onIfAny=11 13 15",
                                                                                     '24:mayDecay = on'                                                 
                                                                                     ),
                                                     parameterSets = cms.vstring('pythia8CommonSettingsBlock',
                                                                                 'UESettingsBlock',
                                                                                 'processParameters',
                                                                                 )
                                                     )
                         )


