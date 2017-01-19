import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *


def getGeneratorFor(hardProc,process):
    
    hardProcParameters=None
    ueParameters=None
    if 'ZToMuMu' in hardProc:
        hardProcParameters = cms.vstring( 'WeakBosonAndParton:qqbar2gmZg = on',
                                          'WeakBosonAndParton:qg2gmZq = on',
                                          '23:onMode = off',
                                          '23:onIfAny = 13',
                                          '23:mMin = 60.0',
                                          'TimeShower:mMaxGamma = 1.0' )

    if 'CUEP8M2T4' in hardProc:
        ueParameters = cms.vstring( 'Tune:pp 14',
                                    'Tune:ee 7',
                                    'MultipartonInteractions:ecmPow=0.25208',
                                    'SpaceShower:alphaSvalue=0.1108',
                                    'PDF:pSet=LHAPDF6:NNPDF30_lo_as_0130',
                                    'MultipartonInteractions:pT0Ref=2.197139e+00',
                                    'MultipartonInteractions:expPow=1.608572e+00',
                                    'ColourReconnection:range=6.593269e+00',
                                    )
    if 'CUEP8M2T4down' in hardProc:
        ueParameters = cms.vstring( 'Tune:pp 14',
                                    'Tune:ee 7',
                                    'MultipartonInteractions:ecmPow=0.25208',
                                    'SpaceShower:alphaSvalue=0.1108',
                                    'PDF:pSet=LHAPDF6:NNPDF30_lo_as_0130',
                                    'MultipartonInteractions:pT0Ref=2.268694e+00',
                                    'MultipartonInteractions:expPow=1.561995e+00',
                                    'ColourReconnection:range=8.714042e+00',
                                    )
    if 'CUEP8M2T4up' in hardProc:
        ueParameters = cms.vstring( 'Tune:pp 14',
                                    'Tune:ee 7',
                                    'MultipartonInteractions:ecmPow=0.25208',
                                    'SpaceShower:alphaSvalue=0.1108',
                                    'PDF:pSet=LHAPDF6:NNPDF30_lo_as_0130',
                                    'MultipartonInteractions:pT0Ref=2.127913e+00',
                                    'MultipartonInteractions:expPow=1.710694e+00',
                                    'ColourReconnection:range=6.500048e+00', 
                                    )
    if 'FSRup'   in hardProc: ueParameters.extend( ['TimeShower:renormMultFac   = 4.0'] )
    if 'FSRdown' in hardProc: ueParameters.extend( ['TimeShower:renormMultFac   = 0.25'] )
    if 'ISRup'   in hardProc: ueParameters.extend( ['SpaceShower:renormMultFac  = 4.0'] )
    if 'ISRdown' in hardProc: ueParameters.extend( ['SpaceShower:renormMultFac  = 0.25'] )
            
    print 'Hard process',hardProcParameters
    print 'UE parameters',ueParameters
    
    #setup Pythia8 generator
    process.generator = cms.EDFilter("Pythia8GeneratorFilter",
                                     maxEventsToPrint = cms.untracked.int32(1),
                                     pythiaPylistVerbosity = cms.untracked.int32(1),
                                     pythiaHepMCVerbosity = cms.untracked.bool(False),
                                     filterEfficiency = cms.untracked.double(1.0),
                                     comEnergy = cms.double(8000.),
                                     PythiaParameters = cms.PSet( pythia8CommonSettingsBlock,
                                                                  hardProcParameters=hardProcParameters,
                                                                  ueParameters=ueParameters,
                                                                  parameterSets = cms.vstring('pythia8CommonSettings',
                                                                                              'hardProcParameters',
                                                                                              'ueParameters'
                                                                                              )
                                                                  )
                                     )

