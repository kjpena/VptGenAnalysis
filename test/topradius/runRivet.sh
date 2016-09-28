#!/bin/bash

WHAT=${1}

case $WHAT in 
    RIVET )

	a=(
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8
	    #TTTo2L2Nu_noSC_TuneCUETP8M1_13TeV-powheg-pythia8
	    #TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8
	)
	for wgt in `seq 0 9`; do
	    for i in ${a[@]}; do
		tag=${i%.*}
		echo "Analyzing ${tag}"
		echo "cmsRun test/runRivetModule_cfg.py input=${i} output=${tag}_w${wgt}.yoda LHEweightNumber=${wgt}"
		cmsRun test/runRivetModule_cfg.py input=${i} output=${tag}_w${wgt}.yoda LHEweightNumber=${wgt} &
	    done
	done
	exit -1


	a=(
	    Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
	    Rt_1.00E_03_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
	    Rt_5.00E_03_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
	    Rt_0.00E+00_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.root
	    Rt_0.00E+00_kappat_5.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.root
	)
	baseDir=/store/cmst3/user/psilva/CompositeTop
	for i in ${a[@]}; do
	    tag=${i%.*}
	    echo "Analyzing ${tag}"
	    echo "cmsRun test/runRivetModule_cfg.py input=${baseDir}/${i} output=${tag}.yoda"
	    cmsRun test/runRivetModule_cfg.py input=${baseDir}/${i} output=${tag}.yoda &
	done
	;;
    RIVETPLOT )

	outDir=~/public/html/TopRadius/Rivet

	rivet-mkhtml -s -o ${outDir}/Powheg --times --config=data/CMS_TOP_Radius.plot \
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_w0.yoda:'$(\mu_R,\mu_F)=(1,1)$' \
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_w1.yoda:'$(1,2)$' \
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_w2.yoda:'$(2,1)$' \
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_w3.yoda:'$(2,2)$' \
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_w4.yoda:'$(2,1/2)$' \
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_w6.yoda:'$(1/2,1)$' \
	    TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_w8.yoda:'$(1/2,1/2)$'
	
	exit -1

	rivet-mkhtml -s -o ${outDir}/Madgraph --times --config=data/CMS_TOP_Radius.plot \
	    Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'SM' \
            Rt_1.00E_03_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_t=10^{-3}~\kappa=0$' \
            Rt_5.00E_03_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_t=5\cdot 10^{-3}~\kappa=0$' \
            Rt_0.00E+00_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_t=0~\kappa=10^{-1}$' \
            Rt_0.00E+00_kappat_5.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_t=0~\kappa=5\cdot 10^{-1}$';

	;;
    RIVET2ROOT )
	a=(
	    Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda
	    Rt_1.00E_03_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda
	    Rt_5.00E_03_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda
	    Rt_0.00E+00_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.yoda
	    Rt_0.00E+00_kappat_5.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.yoda
	)
	for i in ${a[@]}; do 
	    yoda2root.py ${i}; 
	done
	;;
esac

