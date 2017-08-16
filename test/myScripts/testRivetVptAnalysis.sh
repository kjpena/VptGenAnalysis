#!/bin/bash

#ptsqmin=ptsqmin400
ptsqmin=ptsqmin4
#ptsqmin=central

#tune=CUETP8M1
tune=

if [ $tune='' ]
then
    tags=${ptsqmin}
else
    tags=${ptsqmin}_${tune}
fi

yodaDir_Pedro=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/cmst3/user/psilva/Wmass/ntuples
#yodaDir=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_central/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_central/crab_ZJ_central/170511_192318/merged 
yodaDir=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_ptsqmin4/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_ptsqmin4/crab_ZJ_ptsqmin4/170511_192556/merged
#yodaDir/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_central_CUETP8M1/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_central/crab_ZJ_central_CUETP8M1/170513_200242/merged
#yodaDir/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/group/phys_smp/Wmass/kjpena/ZJ_ptsqmin4_CUETP8M1/ZJ_ZToMuMu_powheg_minlo_8TeV_NNPDF30_ptsqmin4/crab_ZJ_ptsqmin4_CUETP8M1/170513_200254/merged

#outputDir=/afs/cern.ch/user/k/kjpena/www/plots/ZJ_${ptsqmin}/all
#outputDir=/afs/cern.ch/user/k/kjpena/www/plots_new/ZJ_${tags}/all
outputDir=/afs/cern.ch/user/k/kjpena/www/plots_new/ZJ_${tags}/all_withPreviousPlots

values=("1" "1.5" "2" "3" "4" "8" "0.125" "0.25" "0.333" "0.5" "0.667") 

# Functions to get the scale values for the legends in the plots
get_muR () 
{
    local weight=${1}

    local j=${weight}/11
    echo ${values[$j]}
} 


get_muF () 
{
    local weight=${1}

    local k=0
    until [ $k -eq $((${weight}%11)) ]
    do
	((k++))
    done

    echo ${values[$k]} 
} 

get_legend ()
{
    local weight=${1}

    legend="MC ($\mu_R="
    legend+=$(get_muR ${weight})
    legend+="$, $\mu_F="
    legend+=$(get_muF ${weight})
    legend+="$)"

    echo $legend #returns 'MC ($\mu_R=1$, $\mu_F=1$)'
}

# Plots

legend_0=$(get_legend 0)
echo ${legend_0}

rivet-mkhtml -s --times \
    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
    -o ${outputDir}/w0 \
    ${yodaDir}/ZJ_${tags}.w0.yoda:"${legend_0}" \
    ${yodaDir_Pedro}/ZJ_${tags}.w0.yoda:"${legend_0}":"LineStyle=dashed" 


for i in {1..60}
do 

    #legend_a=$(get_legend $((4*i-3)) )
    #legend_b=$(get_legend $((4*i-2)) )
    legend_c=$(get_legend $((2*i-1)) )
    legend_d=$(get_legend $((2*i)) )
    #echo ${legend_a}
    #echo ${legend_b}
    echo ${legend_c}
    echo ${legend_d}

    rivet-mkhtml -s --times \
       ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'data' \
       --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
       -o ${outputDir}/w$((2*i-1))-w$((2*i)) \
       ${yodaDir}/ZJ_${tags}.w$((2*i-1)).yoda:"${legend_c}" \
       ${yodaDir}/ZJ_${tags}.w$((2*i)).yoda:"${legend_d}" \
       ${yodaDir_Pedro}/ZJ_${tags}.w$((2*i-1)).yoda:"${legend_c}":"LineStyle=dashed" \
       ${yodaDir_Pedro}/ZJ_${tags}.w$((2*i)).yoda:"${legend_d}":"LineStyle=dashed" 
       #${yodaDir}/ZJ_${tags}.w$((4*i-3)).yoda:"${legend_a}" \
       #${yodaDir}/ZJ_${tags}.w$((4*i-2)).yoda:"${legend_b}" \
       #${yodaDir}/ZJ_${tags}.w$((4*i-1)).yoda:"${legend_c}" \
       #${yodaDir}/ZJ_${tags}.w$((4*i)).yoda:"${legend_d}" \
       #${yodaDir_Pedro}/ZJ_${tags}.w$((4*i-3)).yoda:"${legend_a}":"LineStyle=dashed" \
       #${yodaDir_Pedro}/ZJ_${tags}.w$((4*i-2)).yoda:"${legend_b}":"LineStyle=dashed" \
       #${yodaDir_Pedro}/ZJ_${tags}.w$((4*i-1)).yoda:"${legend_c}":"LineStyle=dashed" \
       #${yodaDir_Pedro}/ZJ_${tags}.w$((4*i)).yoda:"${legend_d}":"LineStyle=dashed"
done
