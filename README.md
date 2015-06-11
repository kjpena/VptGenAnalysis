RivetAnalysis
=============

## setup environment to use yoda scripts
yodaver=`scram tool list | grep -ir yoda | awk '{print $2}'`
export PATH=${PATH}:/cvmfs/cms.cern.ch/$SCRAM_ARCH/external/yoda/$yodaver/bin

## Submitting jobs

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
-i lhe:6721 -c UserCode/RivetAnalysis/python/TTfromME_Z2lep_8TeV_pythia6_tauola_cfi
```
Two full examples below
```
a=(6877 6721 6878 6742 6743)
for i in ${a[@]}; do
    python scripts/submitRivetRun.py  -j 10 -n 50000 -q 1nh -o results_${i} ---filein mcdb:${i} -c UserCode/RivetAnalysis/python/TTfromME_Z2lep_8TeV_pythia6_tauola_cfi  -r UserCode/RivetAnalysis/rivet_customise.customiseTOPDileptons
done
```
```
a=(IC3phiq_m1 NoBSM RC3phiq_m1 RCtW_m1)
for i in ${a[@]}; do
    python scripts/submitRivetRun.py  -j 1 -n 50000 -q 1nh -o results_${i} -i /store/cmst3/user/psilva/TopEft/${i}.lhe -c UserCode/RivetAnalysis/python/TTfromMEnoMatching_Z2lep_8TeV_pythia6_tauola_cfi  -r UserCode/RivetAnalysis/rivet_customise.customiseTOPDileptons
done
```
In all cases the output will be stored in different sub-folders under results.
The results can be merged by running:
```
for i in ${a[@]}; do
    yodamerge -o results_${i}/outMerged.yoda  results_${i}/out_*.yoda
done
```

## Plotting

```
rivet-mkhtml -s --mc-errs -o ~/public/html/TOP-13-007 --times -c data/CMS_TOP_13_007.plot \
	     results/TT_UEP11_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11' \
	     results/TT_UEP11noCR_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11 noCR' \
	     results/TT_UEP11noMPI_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11 noMPI' \
	     results/TT_UEZ2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'Z2 LEP' 
```
Will plot the results for comparison. Another example below:
```
rivet-mkhtml -s --mc-errs -o ~/public/html/TOPDileptons --times -c data/CMS_TOP_Dileptons.plot ptpos.yoda:data \
	      results_6721/TTfromME_Z2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'MG+PY6' \
	      results_6742/TTfromME_Z2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'MG+PY6 ($\mu_R/\mu_F$ down)' \
	      results_6743/TTfromME_Z2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'MG+PY6 ($\mu_R/\mu_F$ up)'
```
and another one
```
rivet-mkhtml -s --mc-errs -o ~/public/html/TOPDileptons --times -c data/CMS_TOP_Dileptons.plot ptpos.yoda:data \
	      results_6721/TTfromME_Z2lep_8TeV_pythia6_tauola_cfi/outMerged.yoda:'MG+PY6' \
	      results_RCtW_m1/TTfromMEnoMatching_Z2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'$RCtW=-1$' \
	      results_RC3phiq_m1/TTfromMEnoMatching_Z2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'$RC3phiq=-1$' \
	      results_IC3phiq_m1/TTfromMEnoMatching_Z2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'$IC3phiq=-1$'
An html page will be generated and can be opened, e.g. as
```
firefox "http://cmsdoc.cern.ch/~`whoami`/TOP-13-007/index.html"
```

## Installation

```
cmsrel CMSSW_7_4_0
cd CMSSW_7_4_0/src
cmsenv
git cms-addpkg GeneratorInterface/RivetInterface 
git clone git@github.com:pfs/RivetAnalysis.git UserCode/RivetAnalysis
scram b -j 9
```
