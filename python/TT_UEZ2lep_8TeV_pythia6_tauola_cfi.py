import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUEZ2starLEPSettings_cfi import pythiaUESettingsBlock

generator = cms.EDFilter("Pythia6GeneratorFilter",  ExternalDecays = cms.PSet( Tauola = cms.untracked.PSet( UseTauolaPolarization = cms.bool(True),
													    InputCards = cms.PSet( mdtau = cms.int32(0),
																   pjak2 = cms.int32(0),
																   pjak1 = cms.int32(0)
																   )
													    ),
									       parameterSets = cms.vstring('Tauola')
									       ),
			 maxEventsToPrint = cms.untracked.int32(0),
			 pythiaPylistVerbosity = cms.untracked.int32(1),
			 filterEfficiency = cms.untracked.double(1.0),
			 pythiaHepMCVerbosity = cms.untracked.bool(False),
			 comEnergy = cms.double(8000.0),
			 crossSection = cms.untracked.double(131.7),
			 UseExternalGenerators = cms.untracked.bool(True),
			 PythiaParameters = cms.PSet( pythiaUESettings=pythiaUESettingsBlock.pythiaUESettings,
                                                      processParameters = cms.vstring('MSEL=0         ! User defined processes', 
                                                                                      'MDME(190,1) = 0   !W decay into dbar u',
                                                                                      'MDME(191,1) = 0   !W decay into dbar c',
                                                                                      'MDME(192,1) = 0   !W decay into dbar t',
                                                                                      'MDME(194,1) = 0   !W decay into sbar u',
                                                                                      'MDME(195,1) = 0   !W decay into sbar c',
                                                                                      'MDME(196,1) = 0   !W decay into sbar t',
                                                                                      'MDME(198,1) = 0   !W decay into bbar u',
                                                                                      'MDME(199,1) = 0   !W decay into bbar c',
                                                                                      'MDME(200,1) = 0   !W decay into bbar t',
                                                                                      'MDME(206,1) = 1   !W decay into e+ nu_e',
                                                                                      'MDME(207,1) = 1   !W decay into mu+ nu_mu',
                                                                                      'MDME(208,1) = 0   !W decay into tau+ nu_tau',
                                                                                      'MSUB(81)  = 1     ! qqbar to QQbar', 
                                                                                      'MSUB(82)  = 1     ! gg to QQbar', 
                                                                                      'MSTP(7)   = 6     ! flavor = top', 
                                                                                      'PMAS(6,1) = 172.5  ! top quark mass'),
                                                      parameterSets = cms.vstring('pythiaUESettings',  'processParameters')
                                                      )
			 )
