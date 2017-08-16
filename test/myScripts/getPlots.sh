#!/bin/bash

for ptsqmin in central ptsqmin4; do
    for tuneId in {21..23}; do

        tune=tune${tuneId}
        tags=${ptsqmin}_${tune}
        echo $tags

        plotsDir=/afs/cern.ch/user/k/kjpena/www/plots_new/ZJ_${tags} 

        for plotName in d15-x01-y04 d27-x01-y04; do
            mkdir ${plotsDir}/${plotName}
            cp /afs/cern.ch/user/k/kjpena/public/index.php ${plotsDir}/${plotName}/

            cp ${plotsDir}/all/w0/ATLAS_2015_I1408516_MU/${plotName}.png \
                ${plotsDir}/${plotName}/w0.png

            cp ${plotsDir}/all/w0/ATLAS_2015_I1408516_MU/${plotName}.pdf \
                ${plotsDir}/${plotName}/w0.pdf

            for i in {1..30}; do
                cp ${plotsDir}/all/w$((4*i-3))-w$((4*i))/ATLAS_2015_I1408516_MU/${plotName}.png \
                    ${plotsDir}/${plotName}/w$((4*i-3))-w$((4*i)).png

                cp ${plotsDir}/all/w$((4*i-3))-w$((4*i))/ATLAS_2015_I1408516_MU/${plotName}.pdf \
              	    ${plotsDir}/${plotName}/w$((4*i-3))-w$((4*i)).pdf
            done

        done

    done
done

