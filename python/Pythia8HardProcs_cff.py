import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from UserCode.RivetAnalysis.UEParameters_cff import *

def getGeneratorFor(hardProc='ZToMuMu_CUEP8M2T4',pdfSet='NNPDF30_lo_as_0130',process=None):
    
    hardProcParameters=None
    if 'ZToMuMu' in hardProc:
        hardProcParameters = cms.vstring( 'WeakBosonAndParton:qqbar2gmZg = on',
                                          'WeakBosonAndParton:qg2gmZq = on',
                                          '23:onMode = off',
                                          '23:onIfAny = 13',
                                          '23:mMin = 60.0',
                                          'TimeShower:mMaxGamma = 1.0' )

    if 'WToMuNu' in hardProc:
        hardProcParameters = cms.vstring( 'WeakSingleBoson:ffbar2W = on',
                                          '24:onMode = off',
                                          '24:onIfAny = 13 14' )

    ueParameters=getUEParameters(ueName=hardProc,pdfSet=pdfSet)

    print '*'*50
    print hardProc
    print 'Hard process cfg',hardProcParameters
    print 'UE parameters cfg',ueParameters    
    print '*'*50

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

