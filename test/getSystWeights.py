from DataFormats.FWLite import Handle, Runs
import ROOT

lheruninfo=Handle('LHERunInfoProduct')
runs=Runs('file:Events_6.root')
#runs=Runs('root://xrootd-cms.infn.it//store/mc/RunIISummer15wmLHEGS/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/GEN-SIM/MCRUN2_71_V1-v1/100000/38E08C94-0F6A-E611-B161-001E67F8FA38.root')
#runs=Runs('root://eoscms//eos/cms/store/cmst3/user/psilva/Wmass/Zj/TuneCUETP8M1/Events_0.root')
#runs=Runs('root://eoscms//eos/cms//store/cmst3/group/hintt/TT_TuneCUETP8M1_5020GeV-powheg-pythia8/a.root')
#runs=Runs('root://cms-xrd-global.cern.ch//store/himc/HINppWinter16DR/WJetsToLNu_TuneCUETP8M1_5020GeV-amcatnloFXFX-pythia8/AODSIM/75X_mcRun2_asymptotic_ppAt5TeV_v3-v1/00000/00CDB927-CBE9-E511-9809-00259021A2AA.root')
for r in runs:
    #r.getByLabel('externalLHEProducer',lheruninfo)
    r.getByLabel('source',lheruninfo)
    it=lheruninfo.product().headers_begin()
    while it!=lheruninfo.product().headers_end():
        lines=it.lines()
        allowPrint=False
        wgtCtr=0
        for i in xrange(0,lines.size()):
            linestr=lines.at(i)
            if '<weightgroup' in linestr : allowPrint=True
            if '</weightgroup' in linestr : allowPrint=False
            if not allowPrint : continue
            if 'weightgroup' in linestr :
                print '*'*50
                print linestr
                print '*'*50
            else:
                if not 'weight' in linestr : continue
                print wgtCtr,linestr
                wgtCtr+=1
        it.next()
