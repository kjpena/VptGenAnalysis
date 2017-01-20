import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *

def getUEParameters(ueName):
    if 'AZ' in hardProc:
        ueParameters = cms.vstring( 'Tune:pp 17',
                                    'Tune:ee 7',
                                    'PDF:pSet=LHAPDF6:%s'%pdfSet )

    if 'CUEP8M2T4' in hardProc:
        if 'down' in hardProc:
            ueParameters = cms.vstring( 'Tune:pp 14',
                                        'Tune:ee 7',
                                        'MultipartonInteractions:ecmPow=0.25208',
                                        'SpaceShower:alphaSvalue=0.1108',
                                        'PDF:pSet=LHAPDF6:%s'%pdfSet,
                                        'MultipartonInteractions:pT0Ref=2.268694e+00',
                                        'MultipartonInteractions:expPow=1.561995e+00',
                                        'ColourReconnection:range=8.714042e+00',
                                        )
        elif 'up' in hardProc:
            ueParameters = cms.vstring( 'Tune:pp 14',
                                        'Tune:ee 7',
                                        'MultipartonInteractions:ecmPow=0.25208',
                                        'SpaceShower:alphaSvalue=0.1108',
                                        'PDF:pSet=LHAPDF6:%s'%pdfSet,
                                        'MultipartonInteractions:pT0Ref=2.127913e+00',
                                        'MultipartonInteractions:expPow=1.710694e+00',
                                        'ColourReconnection:range=6.500048e+00', 
                                        )
        else:
            ueParameters = cms.vstring( 'Tune:pp 14',
                                        'Tune:ee 7',
                                        'MultipartonInteractions:ecmPow=0.25208',
                                        'SpaceShower:alphaSvalue=0.1108',
                                        'PDF:pSet=LHAPDF6:%s'%pdfSet,
                                        'MultipartonInteractions:pT0Ref=2.197139e+00',
                                        'MultipartonInteractions:expPow=1.608572e+00',
                                        'ColourReconnection:range=6.593269e+00',
                                        )

    if 'FSRup'   in hardProc: ueParameters.extend( ['TimeShower:renormMultFac   = 4.0'] )
    if 'FSRdown' in hardProc: ueParameters.extend( ['TimeShower:renormMultFac   = 0.25'] )
    if 'ISRup'   in hardProc: ueParameters.extend( ['SpaceShower:renormMultFac  = 4.0'] )
    if 'ISRdown' in hardProc: ueParameters.extend( ['SpaceShower:renormMultFac  = 0.25'] )
    if 'primordialKToff' in hardProc : ueParameters.extend( ['BeamRemnants:primordialKT = off'] )

    return ueParameters
