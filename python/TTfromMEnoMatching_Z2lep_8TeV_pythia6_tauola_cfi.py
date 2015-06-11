import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUEZ2starLEPSettings_cfi import pythiaUESettingsBlock

generator = cms.EDFilter("Pythia6HadronizerFilter", 
                         ExternalDecays = cms.PSet(Tauola = cms.untracked.PSet(UseTauolaPolarization = cms.bool(True),
                                                                               InputCards = cms.PSet(mdtau = cms.int32(0),
                                                                                                     pjak2 = cms.int32(0),
                                                                                                     pjak1 = cms.int32(0)
                                                                                                     )
                                                                               ),
                                                   parameterSets = cms.vstring('Tauola')
                                                   ),
                         UseExternalGenerators = cms.untracked.bool(True),
                         pythiaPylistVerbosity = cms.untracked.int32(1),
                         pythiaHepMCVerbosity = cms.untracked.bool(True),
                         comEnergy = cms.double(8000.0),
                         maxEventsToPrint = cms.untracked.int32(0),
                         PythiaParameters = cms.PSet( pythiaUESettings=pythiaUESettingsBlock.pythiaUESettings,
                                                      processParameters = cms.vstring('MSEL=0         ! User defined processes',
                                                                                      'PMAS(5,1)=4.8   ! b quark mass',
                                                                                      'PMAS(6,1)=172.5 ! t quark mass',
                                                                                      'MSTJ(1)=1       ! Fragmentation/hadronization on or off',
                                                                                      'MSTP(61)=1      ! Parton showering on or off'),
                                                      parameterSets = cms.vstring('pythiaUESettings',  'processParameters')
                                                      )
			 )
