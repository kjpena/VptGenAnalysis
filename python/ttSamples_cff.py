import FWCore.ParameterSet.Config as cms


"""
"""
def getSamples(tag):
    if tag=='TT_TuneCUETP8M1mpiOFF_13TeV-powheg-pythia8':
        samples=cms.untracked.vstring(
            '/store/mc/RunIIWinter15GS/TT_TuneCUETP8M1mpiOFF_13TeV-powheg-pythia8/GEN-SIM/MCRUN2_71_V1_ext4-v1/20000/28A835C4-D17C-E511-91F4-02163E0114BC.root'
            )
    elif tag=='TT_TuneCUETP8M1_13TeV-powheg-pythia8':
        samples=cms.untracked.vstring(
            '/store/mc/RunIISummer15GS/TT_TuneCUETP8M1_13TeV-powheg-pythia8/GEN-SIM/MCRUN2_71_V1_ext3-v1/00000/04A2C640-F156-E511-80B4-002590A887FE.root',
            '/store/mc/RunIISummer15GS/TT_TuneCUETP8M1_13TeV-powheg-pythia8/GEN-SIM/MCRUN2_71_V1_ext3-v1/00000/143F35DD-DE56-E511-9989-002590200A7C.root',
            '/store/mc/RunIISummer15GS/TT_TuneCUETP8M1_13TeV-powheg-pythia8/GEN-SIM/MCRUN2_71_V1_ext3-v1/00000/14600E79-DF56-E511-A8A8-002590A80D9C.root',
            '/store/mc/RunIISummer15GS/TT_TuneCUETP8M1_13TeV-powheg-pythia8/GEN-SIM/MCRUN2_71_V1_ext3-v1/00000/1A9FB9B6-DE56-E511-9413-001E67398791.root',
            '/store/mc/RunIISummer15GS/TT_TuneCUETP8M1_13TeV-powheg-pythia8/GEN-SIM/MCRUN2_71_V1_ext3-v1/00000/1E5D5688-F456-E511-95C6-002481E7632A.root'
            )
    return samples
