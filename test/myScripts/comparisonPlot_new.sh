#!/bin/bash

#Options
reducedMuSpace=true

yodaDir_Pedro=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/cmst3/user/psilva/Wmass/ntuples
yodaDir_central=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_central/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_central/crab_ZJ_central/170511_192318/merged 
yodaDir_ptsqmin4=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_ptsqmin4/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_ptsqmin4/crab_ZJ_ptsqmin4/170511_192556/merged
yodaDir_central_CUETP8M1=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_central_CUETP8M1/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_central/crab_ZJ_central_CUETP8M1/170513_200242/merged
yodaDir_ptsqmin4_CUETP8M1=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_ptsqmin4_CUETP8M1/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_ptsqmin4/crab_ZJ_ptsqmin4_CUETP8M1/170513_200254/merged
yodaDir_ptsqmin4_tune2=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_ptsqmin4_tune2/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_ptsqmin4/crab_ZJ_ptsqmin4_tune2/170517_195341/merged
outputDir=/afs/cern.ch/user/k/kjpena/www/comparison_plots_new

#Pedro's plots vs. new plots
rivet-mkhtml -s --times --mc-errs \
    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
    -o ${outputDir}/central_previousBest_newBest \
    ${yodaDir_Pedro}/ZJ_central.w46.yoda:'Old MC ($\mu_R=4$, $\mu_F=2$)' \
    ${yodaDir_central}/ZJ_central.w47.yoda:'\begin{tabular}[t]{@{}l@{}} New MC ($\mu_R=4$, $\mu_F=3$) \\ $p_{T,min}^2=0.8$\end{tabular}' 

rivet-mkhtml -s --times --mc-errs \
    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
    -o ${outputDir}/central_previousBest_new \
    ${yodaDir_Pedro}/ZJ_central.w46.yoda:'Old MC ($\mu_R=4$, $\mu_F=2$)' \
    ${yodaDir_central}/ZJ_central.w46.yoda:'\begin{tabular}[t]{@{}l@{}} New MC ($\mu_R=4$, $\mu_F=2$) \\ $p_{T,min}^2=0.8$\end{tabular}'  

rivet-mkhtml -s --times --mc-errs \
    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
    -o ${outputDir}/ptsqmin4_previousBest_newBest \
    ${yodaDir_Pedro}/ZJ_ptsqmin4.w47.yoda:'Old MC ($\mu_R=4$, $\mu_F=3$)' \
    ${yodaDir_ptsqmin4}/ZJ_ptsqmin4.w48.yoda:'\begin{tabular}[t]{@{}l@{}} New MC ($\mu_R=4$, $\mu_F=4$) \\ $p_{T,min}^2=4.0$\end{tabular}' 

rivet-mkhtml -s --times --mc-errs \
    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
    -o ${outputDir}/ptsqmin4_previousBest_new \
    ${yodaDir_Pedro}/ZJ_ptsqmin4.w47.yoda:'Old MC ($\mu_R=4$, $\mu_F=3$)' \
    ${yodaDir_ptsqmin4}/ZJ_ptsqmin4.w47.yoda:'\begin{tabular}[t]{@{}l@{}} New MC ($\mu_R=4$, $\mu_F=3$) \\ $p_{T,min}^2=4.0$\end{tabular}'

#New plots
#rivet-mkhtml -s --times \
#    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
#    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
#    -o ${outputDir}/allNewPlots \
#    ${yodaDir_central}/ZJ_central.w47.yoda:'MC ($p_{T,min}^2=0.8$, CUEP8M2T4, $\mu_R=4$, $\mu_F=3$)' \
#    ${yodaDir_central_CUETP8M1}/ZJ_central_CUETP8M1.w48.yoda:'MC ($p_{T,min}^2=0.8$, CUETP8M1, $\mu_R=4$, $\mu_F=4$)' \
#    ${yodaDir_ptsqmin4}/ZJ_ptsqmin4.w48.yoda:'MC ($p_{T,min}^2=4.0$, CUEP8M2T4 $\mu_R=4$, $\mu_F=4$)' \
#    ${yodaDir_ptsqmin4_CUETP8M1}/ZJ_ptsqmin4_CUETP8M1.w48.yoda:'MC ($p_{T,min}^2=4.0$, CUETP8M1 $\mu_R=4$, $\mu_F=4$)' \
#    ${yodaDir_ptsqmin4_tune2}/ZJ_ptsqmin4_tune2.w48.yoda:'MC ($p_{T,min}^2=4.0$, Tune 2, $\mu_R=4$, $\mu_F=4$)'
