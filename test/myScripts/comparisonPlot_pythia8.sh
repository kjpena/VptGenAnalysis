#!/bin/bash

yodaDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged
outputDir=/afs/cern.ch/user/k/kjpena/www/comparison_plots_new

rivet-mkhtml -s --times \
    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
    -o ${outputDir}/pythia8 \
    ${yodaDir}/pythia8_tuneAZ/pythia8_tuneAZ.yoda:'Pythia8 AZ, $\mu_R=1$, $\mu_F=1$' \
    ${yodaDir}/ZJ_ptsqmin4/ZJ_ptsqmin4.w0.yoda:'Powheg+MiNLO $p_{T,min}^2=4.0$, $\mu_R=1$, $\mu_F=1$' \
    ${yodaDir}/ZJ_ptsqmin4/ZJ_ptsqmin4.w48.yoda:'Powheg+MiNLO $p_{T,min}^2=4.0$, $\mu_R=4$, $\mu_F=4$'
