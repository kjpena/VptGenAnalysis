#!/bin/bash


files=yoda
#files=root

yodaDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged
rootDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/rootFiles

#yodaDir=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged
#rootDir=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/rootFiles

for ptsqmin in central ptsqmin4; do
    for tuneId in {0..1}; do

        tune=tune${tuneId}
        tags=${ptsqmin}_${tune}_PDFpSet13
        echo $tags

        for j in {0..120}; do
            if [ $files == "yoda" ]; then
                if [ ! -f "${yodaDir}/ZJ_${tags}/ZJ_${tags}.w${j}.yoda" ]; then
                    echo "File 'ZJ_${tags}.w${j}.yoda' does not exist"
                elif [ ! -s "${yodaDir}/ZJ_${tags}/ZJ_${tags}.w${j}.yoda" ]; then
                    echo "File 'ZJ_${tags}.w${j}.yoda' is size zero"
                fi 
            elif [ $files == "root" ]; then
                if [ ! -f "${rootDir}/ZJ_${tags}/ZJ_${tags}.w${j}.root" ]; then
                    echo "File 'ZJ_${tags}.w${j}.root' does not exist"
                elif [ ! -s "${rootDir}/ZJ_${tags}/ZJ_${tags}.w${j}.root" ]; then
                    echo "File 'ZJ_${tags}.w${j}.root' is size zero"
                fi
            fi
        done

    done
done

