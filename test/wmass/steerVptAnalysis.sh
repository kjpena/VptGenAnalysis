#!/bin/bash

WHAT=${1}

case $WHAT in
    RIVET )

	script=${CMSSW_BASE}/src/UserCode/RivetAnalysis/scripts/wrapLocalAnalysisRun.sh;
	for i in `seq 1 20`; do 
	    commonOpts=test/runGENRivetModule_cfg.py module=ZPt input=/store/cmst3/user/psilva/Wmass/powhegbox_Zj/seed_${i}_pwgevents.lhe;
	    bsub -q 8nh $script "cmsRun $commonOpts output=${CMSSW_BASE}/src/UserCode/RivetAnalysis/py8_noprimkt_${i}.yoda hadronizer=powhegEmissionVeto_2p_LHE_pythia8:primordialKToff";
	    bsub -q 8nh $script "cmsRun $commonOpts output=${CMSSW_BASE}/src/UserCode/RivetAnalysis/hwpp_${i}.yoda hadronizer=TuneEE_5C_8TeV_Herwigpp";
	    for  w in `seq 0 170`; do
		bsub -q 8nh $script "cmsRun $commonOpts LHEweightNumber=${w} output=py8_w${w}_${i}.yoda hadronizer=powhegEmissionVeto_2p_LHE_pythia8";
	    done
	done
	;;
    MERGE )
	;;
    RIVETPLOT )
	;;
esac