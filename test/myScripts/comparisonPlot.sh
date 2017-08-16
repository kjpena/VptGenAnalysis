#!/bin/bash

#Options
reducedMuSpace=true

yodaDir=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/cmst3/user/psilva/Wmass/ntuples
outputDir=/afs/cern.ch/user/k/kjpena/www/comparison_plots

if [ "$reducedMuSpace" = true ]; then
    rivet-mkhtml -s --times \
        ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
        --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
	-o ${outputDir}/ReducedMuSpace \
        ${yodaDir}/ZJ_central.w46.yoda:'MC ($p_{T,min}^2=0.8$, $\mu_R=4$, $\mu_F=2$)' \
        ${yodaDir}/ZJ_ptsqmin4.w47.yoda:'MC ($p_{T,min}^2=4.0$, $\mu_R=4$, $\mu_F=3$)'\


else
    rivet-mkhtml -s --times \
        ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
        --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
        -o ${outputDir}/FullMuSpace \
        ${yodaDir}/ZJ_central.w59.yoda:'MC ($p_{T,min}^2=0.8$, $\mu_R=8$, $\mu_F=4$)' \
        ${yodaDir}/ZJ_ptsqmin4.w60.yoda:'MC ($p_{T,min}^2=4.0$, $\mu_R=8$, $\mu_F=8$)'
fi
