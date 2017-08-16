#!/bin/bash

currentDir=$pwd

yodaDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged
rootDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/rootFiles

for ptsqmin in central ptsqmin4; do
    for tuneId in {21..23}; do

        tune=tune${tuneId}
        tags=${ptsqmin}_${tune}
        echo $tags

        mkdir $rootDir/ZJ_${tags}       
        cd $rootDir/ZJ_${tags}

        for j in {0..120}; do
            yoda2root.py ${yodaDir}/ZJ_${tags}/ZJ_${tags}.w${j}.yoda
        done

    done
done

cd ${currentDir}
