import ROOT
import sys

fIn=ROOT.TFile.Open(sys.argv[1])
h=fIn.Get('incpartons_inc')
print sys.argv[1]
print 'gg/qq/qg [%%] = %3.2f/%3.2f/%3.2f'%( h.GetBinContent(1)/h.Integral(),
                                           h.GetBinContent(3)/h.Integral(),
                                           h.GetBinContent(2)/h.Integral()
                                           )
fIn.Close()
