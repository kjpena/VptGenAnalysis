import FWCore.ParameterSet.Config as cms
import re

def getUEParameters(ueName='CUEP8M2T4',pdfSet='NNPDF30_lo_as_0130'):
    if 'AZ' in ueName:
        ueParameters = cms.vstring( 'Tune:pp 17',
                                    'Tune:ee 7',
                                    'PDF:pSet=LHAPDF6:%s'%pdfSet )
    if 'CUETP8M1' in ueName:
        ueParameters = cms.vstring( 'Tune:pp 14',
                                    'Tune:ee 7',
                                    'MultipartonInteractions:pT0Ref=2.4024',
                                    'MultipartonInteractions:ecmPow=0.25208',
                                    'MultipartonInteractions:expPow=1.6',
                                    'PDF:pSet=LHAPDF6:%s'%pdfSet )

    if 'CUEP8M2T4' in ueName:
        if 'down' in ueName:
            ueParameters = cms.vstring( 'Tune:pp 14',
                                        'Tune:ee 7',
                                        'MultipartonInteractions:ecmPow=0.25208',
                                        'SpaceShower:alphaSvalue=0.1108',
                                        'PDF:pSet=LHAPDF6:%s'%pdfSet,
                                        'MultipartonInteractions:pT0Ref=2.268694e+00',
                                        'MultipartonInteractions:expPow=1.561995e+00',
                                        'ColourReconnection:range=8.714042e+00',
                                        )
        elif 'up' in ueName:
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

    if 'FSRup'   in ueName: ueParameters.extend( ['TimeShower:renormMultFac   = 4.0'] )
    if 'FSRdown' in ueName: ueParameters.extend( ['TimeShower:renormMultFac   = 0.25'] )
    if 'ISRup'   in ueName: ueParameters.extend( ['SpaceShower:renormMultFac  = 4.0'] )
    if 'ISRdown' in ueName: ueParameters.extend( ['SpaceShower:renormMultFac  = 0.25'] )
    if 'primordialKToff' in ueName : ueParameters.extend( ['BeamRemnants:primordialKT = off'] )
    for param in ['SpaceShower:pT0Ref','SpaceShower:pTmin']:
        if not param in ueName : continue
        newVal=re.findall( "\d+\.\d+", re.search(r'%s=(.*)'%param, ueName).groups()[0] )[0]
        print 'Setting',param,'to',newVal
        ueParameters.extend( ['%s=%s'%(param,newVal) ] )
    


    return ueParameters
