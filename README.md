RivetAnalysis
=============

Analysis in RIVET

### sumbit rivet runs for different configuration files

a=(TT_UEP11_8TeV_pythia6_tauola TT_UEP11noCR_8TeV_pythia6_tauola TT_UEZ2lep_8TeV_pythia6_tauola)
for i in ${a[@]}; do
    python scripts/submitRivetRun.py  -j 1 -n 50000 -o results -q 1nh -c UserCode/RivetAnalysis/python/${i}_cfi.py;
done

The output will be stored in different sub-folders under results.

### plot results

rivet-mkhtml -s --mc-errs -o ~/public/html/TOP-13-007 --times -c CMS_TOP_13_007.yoda \
	     results/TT_UEP11_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11' \
	     results/TT_UEP11noCR_8TeV_pythia6_tauola_cfi/out_1.yoda:'P11 noCR' \
	     results/TT_UEZ2lep_8TeV_pythia6_tauola_cfi/out_1.yoda:'Z2 LEP' 

An html page will be generated and can be opened, e.g. as

firefox "http://cmsdoc.cern.ch/~`whoami`/TOP-13-007/index.html"
