import ROOT
import sys
from rounding import *

ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)

fIn=[]
for entry in sys.argv[1].split(','):
    url,title=entry.split(':')
    fIn.append( (title,ROOT.TFile.Open(url) ) )

c=ROOT.TCanvas('c','c',500,500)
c.SetLeftMargin(0.12)
c.SetRightMargin(0.03)
c.SetTopMargin(0.03)
colors=[1,ROOT.kMagenta, ROOT.kMagenta+2, ROOT.kMagenta-9,ROOT.kViolet+2]
for h in ['avgx_inc','qscale_inc']:

    c.Clear()
    drawOpt='hist'
    allH=[]
    for title,f in fIn:
        allH.append( f.Get(h) )
        allH[-1].Scale( 1./allH[-1].Integral() )
        title='#splitline{%s}{#scale[0.7]{#bar{%s}=%s}}'%(title,
                                         allH[-1].GetXaxis().GetTitle(),
                                         toROOTRounded(allH[-1].GetMean(),allH[-1].GetMeanError()))
        #title=title.replace('\pm','#pm')
        #title=title.replace('$','')
        #title=title.replace('\times','#cdot')
        print title
        allH[-1].SetTitle(title)
        allH[-1].Draw(drawOpt)
        allH[-1].SetDirectory(0)
        allH[-1].SetLineWidth(2)
        allH[-1].SetLineColor( colors[len(allH)-1] )
        allH[-1].GetYaxis().SetTitleSize(0.04)
        allH[-1].GetYaxis().SetTitleOffset(1.2)
        allH[-1].GetYaxis().SetTitle('Events (normalized)')
        allH[-1].GetXaxis().SetTitleSize(0.04)
        drawOpt='histsame'

    leg=c.BuildLegend(0.6,0.95,0.95,0.5)
    leg.SetFillStyle(0)
    leg.SetTextFont(42)
    leg.SetTextSize(0.035)
    leg.SetBorderSize(0)
    leg.SetHeader('#splitline{Madgraph 5 + Pythia 8}{Simulation at #sqrt{s}=13 TeV}')
    c.Modified()
    c.Update()
    raw_input()

