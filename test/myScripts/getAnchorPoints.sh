#!/bin/bash

currentDir=$pwd

yodaDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged
profDir=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/myScripts/prof
tuneDef=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/myScripts/crab/configFiles/tunes.txt

while IFS=' :=' read -ra line || [[ -n "$line" ]]; do
    #Line format: "2 CUEP8M2T4:SpaceShower:alphaSvalue=0.110:BeamRemnants:primordialKThard=1.5:SpaceShower:pTmin=1.0"

    tuneId=${line[0]}

    if [[ ${tuneId} -ge 2 ]]; then

        tune=tune${tuneId}

        for ptsqmin in central ptsqmin4; do
 
            tags=${ptsqmin}_${tune}
            echo $tags

            printf -v tuneId "%03.f" "${tuneId}"

            #mkdir $profDir/ZJ_${ptsqmin}_tune/mc/${tuneId} 
            cd $profDir/ZJ_${ptsqmin}_tune/mc/${tuneId}

            #cp ${yodaDir}/ZJ_${tags}/ZJ_${tags}.w48.yoda $profDir/ZJ_${ptsqmin}_tune/mc/${tuneId}/yoda.out
            #rm out.yoda 
            yoda2aida ${yodaDir}/ZJ_${tags}/ZJ_${tags}.w48.yoda out.aida
      
            echo "${line[2]}:${line[3]} ${line[4]}" > $profDir/ZJ_${ptsqmin}_tune/mc/${tuneId}/used_params 
            echo "${line[5]}:${line[6]} ${line[7]}" >> $profDir/ZJ_${ptsqmin}_tune/mc/${tuneId}/used_params        
            echo "${line[8]}:${line[9]} ${line[10]}" >> $profDir/ZJ_${ptsqmin}_tune/mc/${tuneId}/used_params

        done

    fi

done < "$tuneDef"

cd $currentDir
