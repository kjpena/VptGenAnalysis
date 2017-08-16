from DataFormats.FWLite import Handle, Events
import ROOT
import sys
import os

def getFirstChargedLepton(g):
    for k in xrange(0,g.numberOfDaughters()):
        gdid=abs(g.daughter(k).pdgId())
        if gdid in [11,13] : return g.daughter(k)
        if gdid==15        : return getFirstChargedLepton(g.daughter(k))
    return None



if len(sys.argv)==1:
    print ' '
    print 'python ttGenLevel.py [input file name]'
    print ' '
    exit(-1)


prepend='root://eoscms//eos/cms'
inName=prepend+sys.argv[1]
outName=os.path.basename(sys.argv[1]).replace('.root','_acc.root')

print '-'*50
print 'Analysing %s and output is %s'%(inName,outName)

#control histograms
histos={}
for sel in ['_inc','_fid']:
    histos['avgx'+sel]=ROOT.TH1F('avgx'+sel,';<x>;Events',100,0,0.6)
    histos['qscale'+sel]=ROOT.TH1F('qscale'+sel,';#sqrt{Q^{2}} [GeV];Events',100,150,1000)
    histos['qscalevsavgx'+sel]=ROOT.TH2F('qscalevsavgx'+sel,';<x>;#sqrt{Q^{2}} [GeV];Events',100,0,0.6,150,0,1000)
    histos['incpartons'+sel]=ROOT.TH1F('incpartons'+sel,';Initial state;Events',3,0,3)
    histos['incpartons'+sel].GetXaxis().SetBinLabel(1,'gg')
    histos['incpartons'+sel].GetXaxis().SetBinLabel(2,'qg')
    histos['incpartons'+sel].GetXaxis().SetBinLabel(3,'qq')
histos['cutflow']=ROOT.TH1F('cutflow',';Selection cut;Events',5,0,5)
histos['cutflow'].GetXaxis().SetBinLabel(1,'total')
histos['cutflow'].GetXaxis().SetBinLabel(2,'#geq 2l')
histos['cutflow'].GetXaxis().SetBinLabel(3,'M(ll)>20')
histos['cutflow'].GetXaxis().SetBinLabel(4,'#geq 1j')
histos['cutflow'].GetXaxis().SetBinLabel(5,'#geq 2j')
histos['finalstates']=ROOT.TH1F('finalstates',';Final states;Events',6,0,6)
histos['finalstates'].GetXaxis().SetBinLabel(1,'all had')
histos['finalstates'].GetXaxis().SetBinLabel(2,'l+jets')
histos['finalstates'].GetXaxis().SetBinLabel(3,'ee')
histos['finalstates'].GetXaxis().SetBinLabel(4,'#mu#mu')
histos['finalstates'].GetXaxis().SetBinLabel(5,'e#mu')
histos['finalstates'].GetXaxis().SetBinLabel(6,'others')
for h in histos: 
    histos[h].SetDirectory(0)
    histos[h].Sumw2()


#loop over the events to analyze @ gen level
events = Events(inName)
genParticles = Handle("std::vector<reco::GenParticle>")
genEventInfo = Handle("GenEventInfoProduct")
genJets      = Handle('std::vector<reco::GenJet>')
ntot=0
for event in events:
    ntot+=1
    if ntot%1000==0 : 
        sys.stdout.write('.')
        sys.stdout.flush()

    #generator info
    event.getByLabel("generator", genEventInfo)
    wgt        = genEventInfo.product().weight()
    pdf        = genEventInfo.product().pdf()
    x1,x2      = pdf.x.first,pdf.x.second
    avgx       = 0.5*(x1+x2)
    id1,id2    = pdf.id.first,pdf.id.second
    incpartbin = 0 if id1==21 and id2==21 else 1
    if id1!=21 and id2!=21 : incpartbin=2    
    pdfScale   = pdf.scalePDF
    histos['avgx_inc'].Fill(avgx,wgt)
    histos['incpartons_inc'].Fill(incpartbin,wgt)
    histos['qscale_inc'].Fill(pdfScale,wgt)
    histos['qscalevsavgx_inc'].Fill(avgx,pdfScale,wgt)

    #loop over gen particles
    event.getByLabel("genParticles", genParticles) 
    nj,ne,nm,nthad=0,0,0,0
    lp4=[]
    for g in genParticles.product():
        
        #check if particle comes from a W decay
        try:
            if abs(g.mother().pdgId())!=24: continue
        except:
            continue

        #select charged leptons
        gid=abs(g.pdgId())
        if not gid in [11,13,15]: 
            if gid<6 : nj+=1
            continue

        #check lepton type (trace if tau decays leptonically)
        if gid==11 : 
            lp4.append(ROOT.TLorentzVector(g.px(),g.py(),g.pz(),g.energy())) 
            ne+=1
        if gid==13 : 
            lp4.append(ROOT.TLorentzVector(g.px(),g.py(),g.pz(),g.energy())) 
            nm+=1
        if gid==15:
            gDaughter=getFirstChargedLepton(g)
            try:
                gdid=abs(gDaughter.pdgId())
                if gdid==11 : ne+=1
                if gdid==13 : nm+=1
                lp4.append(ROOT.TLorentzVector(gDaughter.px(),
                                              gDaughter.py(),
                                              gDaughter.pz(),
                                              gDaughter.energy()))
            except:
                nthad+=1

    if nthad>0              : histos['finalstates'].Fill(5,wgt)
    elif ne==2              : histos['finalstates'].Fill(2,wgt)
    elif nm==2              : histos['finalstates'].Fill(3,wgt)
    elif ne==1 and nm==1    : histos['finalstates'].Fill(4,wgt)
    elif nj>0 and ne+nm==1  : histos['finalstates'].Fill(1,wgt)
    else                    : histos['finalstates'].Fill(0,wgt)

    #select gen jets
    event.getByLabel("ak4GenJets", genJets)
    nj=0
    for j in genJets.product():          
        if j.pt()<25 or ROOT.TMath.Abs(j.eta())>3 : continue
        jp4=ROOT.TLorentzVector(j.px(),j.py(),j.pz(),j.energy())
        overlap=False
        for il in xrange(0,len(lp4)):
            if jp4.DeltaR(lp4[il])<0.4 :
                overlap=True
        if overlap : continue
        nj+=1

    #dilepton acceptance analysis
    if ne+nm!=2 : continue
    histos['cutflow'].Fill(0,wgt)
    if len(lp4)<2 : continue
    if lp4[0].Pt()<20 or lp4[1].Pt()<20 : continue
    if ROOT.TMath.Abs(lp4[0].Eta())>2.5 or ROOT.TMath.Abs(lp4[1].Eta())>2.5 : continue
    histos['cutflow'].Fill(1,wgt)
    ll=lp4[0]+lp4[1]
    if ll.M()<20 : continue
    histos['cutflow'].Fill(2,wgt)
    if nj>0 :     histos['cutflow'].Fill(3,wgt)
    if nj<2 : continue
    histos['cutflow'].Fill(4,wgt)
    histos['avgx_fid'].Fill(avgx,wgt)    
    histos['incpartons_fid'].Fill(incpartbin,wgt)
    histos['qscale_fid'].Fill(pdfScale,wgt)
    histos['qscalevsavgx_fid'].Fill(avgx,pdfScale,wgt)

#save results to file
print 'Summary histograms can be found in %s'%outName
print '-'*50
fOut=ROOT.TFile(outName,'RECREATE')
for h in histos: 
    histos[h].SetDirectory(fOut)
    histos[h].Write()
fOut.Close()
