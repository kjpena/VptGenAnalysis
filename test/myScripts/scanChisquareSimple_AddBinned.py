#!/usr/bin/env python

import ROOT
import math


def getPlotFrom(url,name,tag):
    fIn=ROOT.TFile.Open(url)
    h=fIn.Get(name).Clone(tag)
    h.SetDirectory(0)
    fIn.Close()
    return h

#Options
reducedMuSpace=True

#ptsqmin='central'
ptsqmin='ptsqmin4'

plotPhi=True


if plotPhi :
    plotList=[#('h_d15_x01_y04','1/#sigma d#sigma/d#phi*_{#eta} 66#leqm_{ll}/GeV#leq116, |y_{ll}|<2.4'),
               ('h_d05_x01_y04','1/#sigma d#sigma/d#phi*_{#eta} 66#leqm_{ll}/GeV#leq116, |y_{ll}|<0.4'),
               ('h_d06_x01_y04','1/#sigma d#sigma/d#phi*_{#eta} 66#leqm_{ll}/GeV#leq116, 0.4<|y_{ll}|<0.8'),
               ('h_d07_x01_y04','1/#sigma d#sigma/d#phi*_{#eta} 66#leqm_{ll}/GeV#leq116, 0.8<|y_{ll}|<1.2'),
               ('h_d08_x01_y04','1/#sigma d#sigma/d#phi*_{#eta} 66#leqm_{ll}/GeV#leq116, 1.2<|y_{ll}|<1.6'),
               ('h_d09_x01_y04','1/#sigma d#sigma/d#phi*_{#eta} 66#leqm_{ll}/GeV#leq116, 1.6<|y_{ll}|<2.0'),
               ('h_d10_x01_y04','1/#sigma d#sigma/d#phi*_{#eta} 66#leqm_{ll}/GeV#leq116, 2.0<|y_{ll}|<2.4'),
               ]

else :
    plotList=[#('h_d27_x01_y04','1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, |y_{ll}|<2.4'),
               ('h_d17_x01_y04','1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, |y_{ll}|<0.4'),
               ('h_d18_x01_y04','1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, 0.4<|y_{ll}|<0.8'),
               ('h_d19_x01_y04','1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, 0.8<|y_{ll}|<1.2'),
               ('h_d20_x01_y04','1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, 1.2<|y_{ll}|<1.6'),
               ('h_d21_x01_y04','1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, 1.6<|y_{ll}|<2.0'),
               ('h_d22_x01_y04','1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, 2.0<|y_{ll}|<2.4'),
               ]

wgtList=[]
with open('/afs/cern.ch/user/p/psilva/public/forLuca/ZJ_weightList.txt','r')  as f:
    line_words = [line.split() for line in f]
    line_words = filter(lambda x : len(x)>0 and x[0].isdigit(), line_words)
    for i in xrange(0,121):
        x=line_words[i]
        wgtList.append( (int(x[0]),
                         float(x[3].split('=')[1]),
                         float(x[4].split('=')[1])) )


ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
ROOT.gROOT.SetBatch(True)
c=ROOT.TCanvas('c','c',500,500)
c.SetTopMargin(0.01)
c.SetLeftMargin(0.12)
c.SetRightMargin(0.12)
c.SetBottomMargin(0.1)
c.SetLogz()
c.SetLogx()
c.SetLogy()

A=1
chi2scan=ROOT.TGraph2D()
muRmin,muFmin,chi2min=1,1,9999999
wgtIdxmin=1

for wgtIdx,muR,muF in wgtList: #Loop over weights
    if reducedMuSpace and (muR==8 or muR==0.125 or muF==8 or muF==0.125) : continue

    chi2,ndof=0.,0

    for name,title in plotList: #Loop over plots d##-x01-y04
        dataH=getPlotFrom('plots/ATLAS_2015_I1408516_MU.root',name,'data') #Get data for this plot
        dataN=dataH.Integral(dataH.GetXaxis().FindBin(0),dataH.GetXaxis().FindBin(30)-1,"width")

        h=getPlotFrom('plots/ZJ_%s.w%d.root'%(ptsqmin,wgtIdx),name,'mc') #Get MC for this plot and weight

        if 'p_{T}^{ll}' in title : A=dataN/h.Integral(h.GetXaxis().FindBin(0),h.GetXaxis().FindBin(30)-1,"width")

        for xbin in xrange(1,h.GetNbinsX()+1): #Loop over bins to compute the chi2 value
            xcen=h.GetXaxis().GetBinCenter(xbin)
            if 'p_{T}^{ll}' in title and xcen>30 : break #EDIT: break instead of continue
            err2=dataH.GetBinError(xbin)**2+(A*h.GetBinError(xbin))**2
            diff2=(A*(h.GetBinContent(xbin))-dataH.GetBinContent(xbin))**2
            if err2==0 : continue
            chi2 += diff2/err2
            ndof += 1
        h.Delete()
        dataH.Delete()

    if chi2>1000: continue
    if chi2<chi2min:  muRmin,muFmin,chi2min,wgtIdxmin=muR,muF,chi2,wgtIdx
    chi2scan.SetPoint(chi2scan.GetN(),muR,muF,chi2)

#if chi2scan.GetN()<2 : continue
    
c.Clear()
chi2scan.Draw('colz')
chi2scanH=chi2scan.GetHistogram()
chi2scanH.GetZaxis().SetTitleOffset(0.8)
chi2scanH.GetZaxis().SetTitle('#chi^{2}')
chi2scanH.GetXaxis().SetTitle('#mu_{R}')
chi2scanH.GetYaxis().SetTitle('#mu_{F}')
chi2scanH.GetXaxis().SetMoreLogLabels()
chi2scanH.GetYaxis().SetMoreLogLabels()
chi2scanH.GetZaxis().SetRangeUser(0.1,1e3)
if reducedMuSpace :
    chi2scanH.GetXaxis().SetLimits(0.25,4)
    chi2scanH.GetYaxis().SetLimits(0.25,4)

txt=ROOT.TLatex()
txt.SetNDC(True)
txt.SetTextFont(42)
txt.SetTextSize(0.03)
txt.SetTextAlign(12)
txt.DrawLatex(0.15,0.95,'#bf{CMS} #it{simulation}, #bf{ATLAS} #it{data}')
txt.DrawLatex(0.15,0.9,'1/#sigma d#sigma/dp_{T}^{ll} 66#leqm_{ll}/GeV#leq116, Sum of rapidity bins')
   
gr=ROOT.TGraph()
gr.SetMarkerStyle(20)
gr.SetPoint(0,muRmin,muFmin)
gr.Draw('p')

if plotPhi : 
    name='h_d15s-x01-y04'
else :
    name='h_d27s-x01-y04'

print name,wgtIdxmin,muRmin,muFmin,chi2min

c.Modified()
c.Update()

if reducedMuSpace :
    c.SaveAs('~/www/chi2_plots/ReducedMuSpace/ZJ_%s/%s.png'%(ptsqmin,name))
    c.SaveAs('~/www/chi2_plots/ReducedMuSpace/ZJ_%s/%s.pdf'%(ptsqmin,name))
    c.SaveAs('~/www/chi2_plots/ReducedMuSpace/ZJ_%s/%s.root'%(ptsqmin,name))

else :
    c.SaveAs('~/www/chi2_plots/FullMuSpace/ZJ_%s/%s.png'%(ptsqmin,name))
    c.SaveAs('~/www/chi2_plots/FullMuSpace/ZJ_%s/%s.pdf'%(ptsqmin,name))
    c.SaveAs('~/www/chi2_plots/FullMuSpace/ZJ_%s/%s.root'%(ptsqmin,name))
