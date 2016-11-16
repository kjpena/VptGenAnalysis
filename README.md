RivetAnalysis
=============

## Installation

Install CMSSW and download this package and the generator configuration files
```
cmsrel CMSSW_8_0_5_patch1
cd CMSSW_8_0_5_patch1/src
cmsenv

git-cms-init
git-cms-addpkg GeneratorInterface/RivetInterface
git-cms-addpkg Configuration/Generator
git-cms-merge-topic jhgoh:TOP-RIVET-80X

mkdir TopMonteCarlo
cd TopMonteCarlo
git clone https://:@gitlab.cern.ch:8443/CMS-TOP-Rivet/RivetTop.git
git clone https://:@gitlab.cern.ch:8443/CMS-TOP-Rivet/Configuration.git
cd -

git cms-addpkg GeneratorInterface/RivetInterface 
git clone git@github.com:pfs/RivetAnalysis.git UserCode/RivetAnalysis
cp UserCode/RivetAnalysis/data/ATLAS* GeneratorInterface/RivetInterface/data/

scram b -j 8
```
To run a RIVET plugin starting from a LHE file can use the following cfg
```
cmsRun test/runGENRivetModule_cfg.py module=ZPt output=out_1.yoda LHEweightNumber=0 input=/store/cmst3/user/psilva/Wmass/powhegbox_Zj/seed_1_pwgevents.lhe saveEDM=False
```
The output of the example above can be plotted with the following command
```
rivet-mkhtml -s -o ~/public/html/Zpt --times out_1.yoda:'Zj' ../../GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'data' --config=../../GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot --config=data/CMS_Z_Pt.plot
```

## Other notes

If you need to use yoda scripts, you can use the following extra variables
```
yodaver=`scram tool list > a.out && grep -ir yoda a.out | awk '{print $2}'`
export PATH=${PATH}:/cvmfs/cms.cern.ch/$SCRAM_ARCH/external/yoda/$yodaver/bin
```
There are several plugins implemented under src/. 
   * CMS_TOP_13_007.cc - TOP-13-007 UE measurement in the dileptons channel  
   * CMS_TOP_Dileptons.cc - TOP-16-002 mass from lepton kinematics
   * CMS_TOP_Radius.cc - composite top analysis
A wrapper script to call the RIVET plugin while generating events on the fly is provided 
under scripts/. The script takes a configuration file used to generate the events and 
customizes the run according to the plugin to be used.
An example is as follows
```
python scripts/submitRivetRun.py  -n -1 -q local \
       -o results_Rt_0_kappa_0 \
       -i /store/cmst3/user/psilva/CompositeTop/Rt_0.00E+00_kappat_0.00E+00_xqcut_20.0_njetmax_1_qcut_30.0.root \
       -c UserCode/RivetAnalysis/python/Hadronizer_MgmMatchTuneCUEP8S1CTEQ6L1_13TeV_madgraph_pythia8_Tauola_cff \
       -r UserCode/RivetAnalysis/rivet_customise.customiseTOPRadius
```
If local is changed to a batch queue (e.g. 8nh) the job is submited to lxbatch for execution
The output of the job can be plotted using the standard tools

```
rivet-mkhtml -s --mc-errs -o ~/public/html/TopRadius --times \
	     results_Rt_0_kappa_0/Hadronizer_MgmMatchTuneCUEP8S1CTEQ6L1_13TeV_madgraph_pythia8_Tauola_cff/out_1.yoda:'R_{t}=0 \kappa=0'
```
An html page will be generated and can be opened from a browser @
```
http://cmsdoc.cern.ch/~`whoami`/TopRadius/index.html
```

