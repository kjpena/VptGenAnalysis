#!/usr/bin/env python

import os
import sys
import optparse
import ROOT

"""
"""
def testSeparationForVariablesIn(url1,url2,gTools):

    f1=ROOT.TFile.Open(url1)
    f2=ROOT.TFile.Open(url2)
    
    report={}
    for tkey in f1.GetListOfKeys():
        key=tkey.GetName()
        obj1,obj2=f1.Get(key),f2.Get(key)

        #require TH1 in both files
        if not obj1.InheritsFrom('TH1') : continue
        if not obj2 : continue

        #require same number of bins
        nbins=obj1.GetXaxis().GetNbins()
        if obj2.GetXaxis().GetNbins()!=nbins: continue

        rms1,rms2=obj1.GetRMS(),obj2.GetRMS()
        if rms1==0 or rms2==0 : continue

        #significance
        sig=ROOT.TMath.Abs(obj1.GetMean()-obj2.GetMean())/(rms1**2+rms2**2)

        #separation
        sep=gTools.GetSeparation(obj1,obj2)
        
        #ROC curve and area under the curve
        int1,int2=obj1.Integral(),obj2.Integral()
        rocGr=ROOT.TGraph()
        rocGr.SetName('roc_%s'%key)
        for i in xrange(0,nbins):
            rej1=1.0-obj1.Integral(i+1,nbins)/int1
            eff2=obj2.Integral(i+1,nbins)/int2
            rocGr.SetPoint(i,rej1,eff2)
        report[key]=(sig,sep,rocGr.Integral(),rocGr)
    return report


def main():

    # configure
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-o', '--output',     dest='output' , help='Name of the output. [%default]', default='SeparationPower.root')
    (opt, args) = parser.parse_args()

    if len(args)<2 :
        print 'Need at least two files to compare'
        exit(-1)

    fOut=ROOT.TFile.Open(opt.output,'RECREATE')

    gTools=ROOT.TMVA.Tools.Instance()
    for i in xrange(1,len(args)):
        
        report=testSeparationForVariablesIn(args[0],args[i],gTools)
        
        #output results for a directory
        dirName=os.path.splitext(os.path.basename(args[i]))[0]
        outDir=fOut.mkdir(dirName)
        outDir.cd()
        for var in report:
            h=ROOT.TH1F('separation_%s'%var,';;Value',3,0,3)
            h.GetXaxis().SetBinLabel(1,'S')
            h.SetBinContent(1,report[var][0])
            h.GetXaxis().SetBinLabel(2,'<S^{2}>')
            h.SetBinContent(2,report[var][1])
            h.GetXaxis().SetBinLabel(3,'AUC')
            h.SetBinContent(3,report[var][2])
            h.SetDirectory(outDir)
            h.Write()
            report[var][3].Write()
        fOut.cd()

    #all done
    fOut.Close()

if __name__ == '__main__':
    main()

