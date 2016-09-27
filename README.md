RivetAnalysis
=============

## Installation

Install CMSSW and download this package and the generator configuration files
```
cmsrel CMSSW_8_0_8_patch1
cd CMSSW_8_0_8_patch1/src
cmsenv
git cms-addpkg GeneratorInterface/RivetInterface 
git clone git@github.com:pfs/RivetAnalysis.git UserCode/RivetAnalysis
cp UserCode/RivetAnalysis/data/ATLAS* GeneratorInterface/RivetInterface/data/
scram b -j 8
```
If you need to use yoda scripts, you can use the following extra variables
```
yodaver=`scram tool list > a.out && grep -ir yoda a.out | awk '{print $2}'`
export PATH=${PATH}:/cvmfs/cms.cern.ch/$SCRAM_ARCH/external/yoda/$yodaver/bin
```

## Running RIVET plugins

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

