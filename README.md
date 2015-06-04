RivetAnalysis
=============

# Analysis in RIVET

```
a=(TT_UEP11_8TeV_pythia6_tauola TT_UEP11noCR_8TeV_pythia6_tauola TT_UEP11noMPI_8TeV_pythia6_tauola TT_UEZ2lep_8TeV_pythia6_tauola)
for i in ${a[@]}; do
    python scripts/submitRivetRun.py  -j 1 -n 10000 -o results -q 1nh -c UserCode/RivetAnalysis/python/${i}_cfi.py;
done
```
Will sumbit rivet runs for different configuration files. By default it runs TOP-13-007 routine. You can change it with the following option
```
-r UserCode/RivetAnalysis/rivet_customise.mycustomise
```
where the function mycustomise has to be implemented in python/rivet_customise.py.
Notice also that events may only need to be hadronized, in case a LHE is provided. In this case you can run with
```
-i mcdb:6721 -c UserCode/RivetAnalysis/python/TTfromME_Z2lep_8TeV_pythia6_tauola_cfi
```
In all cases the output will be stored in different sub-folders under results.

```
rivet-mkhtml -s --mc-errs -o ~/public/html/TOP-13-007 --times -c data/CMS_TOP_13_007.plot \
	     results/TT_UEP11_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11' \
	     results/TT_UEP11noCR_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11 noCR' \
	     results/TT_UEP11noMPI_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11 noMPI' \
	     results/TT_UEZ2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'Z2 LEP' 
```
Will plot the results for comparison
An html page will be generated and can be opened, e.g. as
```
firefox "http://cmsdoc.cern.ch/~`whoami`/TOP-13-007/index.html"
```

# Installation

```
cmsrel CMSSW_7_4_0
cd CMSSW_7_4_0/src
cmsenv
git cms-addpkg GeneratorInterface/RivetInterface 
git clone git@github.com:pfs/RivetAnalysis.git UserCode/RivetAnalysis
scram b -j 9
```
