#!/bin/bash

WHAT=${1}

case $WHAT in 
    RIVETSUBMIT )
	a=(
	    TT_TuneCUETP8M1_13TeV_powheg_pythia8
	    TT_TuneCUETP8M1noCR_13TeV_powheg_pythia8
	    TT_TuneEE5C_13TeV_powheg_herwigpp
	)
	for i in ${a[@]}; do
	    crab submit crab_${i}.py
	done	
	exit -1
	;;

    RIVETMERGE )
	#mount EOS locally
	/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount eos;

	#loop over directories of interest
	a=(TT_TuneCUETP8M1_13TeV-powheg-pythia8  
	    TT_TuneCUETP8M1noCR_13TeV-powheg-pythia8
	    TT_TuneEE5C_13TeV-powheg-herwigpp)
	for i in ${a[@]}; do 
	    b=(`ls eos/cms/store/group/phys_top/psilva/UE_2f64282/${i}/*/*/*/*.yoda`)
	    toMerge=""
	    for j in ${b[@]}; do 
		toMerge="${toMerge} ${j}"; 
	    done
	    echo "Merging ${i} from ${#b[@]} output files"
	    yodamerge -o ${i}.yoda --s1dmode=assume_mean  ${toMerge}
	done
	
	#unmount EOS
	/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount eos;
	;;

    RIVETPLOT )
	outDir=~/public/html/TopUE/Rivet
	rivet-mkhtml -s -o ${outDir}/Powheg --times --config=data/CMS_TOP_13_007.plot \
	    TT_TuneCUETP8M1_13TeV-powheg-pythia8.yoda:'PW+PY8 CUETP8M1' \
	    TT_TuneCUETP8M1noCR_13TeV-powheg-pythia8.yoda:'PW+PY8 CUETP8M1noCR' \
	    TT_TuneEE5C_13TeV-powheg-herwigpp.yoda:'PW+HW++ EE5C'
	;;

esac

