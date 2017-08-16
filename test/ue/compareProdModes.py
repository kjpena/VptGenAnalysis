import ROOT
import sys

ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
c=ROOT.TCanvas('c','c',500,300)
c.SetLeftMargin(0.12)
c.SetTopMargin(0.02)
c.SetRightMargin(0.01)
c.SetBottomMargin(0.15)

fIn=ROOT.TFile.Open(sys.argv[1])
vars=[('mll','M(ll) [GeV]'),
      ('mbb','M(bb) [GeV]'),
      ('mllbb','M(llbb) [GeV]'),
      ('mllbbmet','M(llbb+E_{T}^{miss} [GeV]'),
      ('mttbar','M(t#bar{t}) [GeV'),
      ('ptll','p_{T}(ll) [GeV]'),
      ('ptbb','p_{T}(bb) [GeV]'),
      ('ptllbb','p_{T}(llbb) [GeV]'),
      ('ptllbbmet','p_{T}(llbb+E_{T}^{miss}) [GeV]'),
      ('ptttbar','p_{T}(t#bar{t}) GeV'),
      ('dphill','#Delta#phi(ll) [deg]'),
      ('dphibb','#Delta#phi(bb) [deg]'),
      ('detall','#Delta#eta(ll)'),
      ('detabb','#Delta#eta(bb)'),
      ('phistarll','#phi^{*}(ll)'),
      ('phistarbb','#phi^{*}(bb)')]
for var,xtitle in vars:
    incH=fIn.Get('h_inc_%s'%var)

    stack=ROOT.THStack()
    colors=[ROOT.kMagenta, ROOT.kMagenta+2, ROOT.kMagenta-9]
    allH=[]
    for prod in ['qq','qg','gg']:
        
        allH.append( fIn.Get('h_%s_%s'%(prod,var)) )
        allH[-1].Divide(incH)
        allH[-1].SetDirectory(0)
        allH[-1].SetTitle(prod)
        allH[-1].SetFillColor(colors[len(allH)-1])
        allH[-1].SetFillStyle(1001)
        stack.Add(allH[-1],'hist')

    c.Clear()
    frame=incH.Clone('frame')
    frame.Reset('ICE')
    frame.Draw()
    frame.GetYaxis().SetRangeUser(0,1)
    frame.GetXaxis().SetTitleSize(0.06)
    frame.GetYaxis().SetTitleSize(0.06)
    frame.GetXaxis().SetLabelSize(0.06)
    frame.GetYaxis().SetLabelSize(0.06)
    frame.GetYaxis().SetTitle('Fraction')
    frame.GetXaxis().SetTitle(xtitle)
    stack.Draw('histsame')
    leg=ROOT.TLegend(0.15,0.9,0.9,0.7)
    for h in allH: leg.AddEntry(h,h.GetTitle(),'f')
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)
    leg.SetTextFont(42)
    leg.SetTextSize(0.06)
    leg.SetNColumns(3)
    leg.Draw()
    if 'phistar' in var: c.SetLogx(True)
    else : c.SetLogx(False)
    c.Modified()
    c.Update()
    c.SaveAs('%s_comp.png'%var)
    raw_input()
    
    frame.Delete()
