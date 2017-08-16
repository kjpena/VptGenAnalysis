#!/bin/bash


# Functions to get the scale values for the legends in the plots
values=("1" "1.5" "2" "3" "4" "8" "0.125" "0.25" "0.333" "0.5" "0.667")

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
for ptsqmin in central #ptsqmin4
do

    for tuneId in {0..0}
    do
        tune=tune${tuneId}

        # Get tags
        if [[ -z $tune ]]
        then
            tags=${ptsqmin}
        else
            tags=${ptsqmin}_${tune}_PDFpSet13_CMSAnalysis
        fi

        echo $tags

        # Create plot configuration file
        plotTempl=$CMSSW_BASE/src/GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.templ

        if [[ $ptsqmin == "ptsqmin4" ]]
        then
            ptsqminValue=4.0 
        else
            ptsqminValue=0.8        
        fi

        echo "ptsqmin="$ptsqminValue

        if [[ $tuneId == 0 ]]
        then
            tuneId="CUETP8M1" 
        fi

        sedstr="s%_PTSQMIN_%${ptsqminValue}%;s%_ID_%${tuneId}%;"
        cat ${plotTempl} | sed "${sedstr}" > $CMSSW_BASE/src/GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU_temp.plot

        # Get input and output directories
        yodaDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged/ZJ_${tags}
        #yodaDir=/afs/cern.ch/work/k/kjpena/CMSSW_8_0_8_patch1/src/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged/ZJ_${tags}
        outputDir=/afs/cern.ch/user/k/kjpena/www/plots_new/ZJ_${tags}/all

        ## Default scale values
        legend_0=$(get_legend 0)
        echo ${legend_0}

        rivet-mkhtml -s --times \
            $CMSSW_BASE/src/myScripts/patch/CMS_2017_I1471281_Z.yoda:'Data' \
            --config=$CMSSW_BASE/src/myScripts/patch/CMS_2017_I1471281.plot \
            -o ${outputDir}/w0 \
            ${yodaDir}/ZJ_${tags}.w0.yoda:"${legend_0}" \

        # Scale variations
        for i in {1..30}
        do 

            legend_a=$(get_legend $((4*i-3)) )
            legend_b=$(get_legend $((4*i-2)) )
            legend_c=$(get_legend $((4*i-1)) )
            legend_d=$(get_legend $((4*i)) )
            echo ${legend_a}
            echo ${legend_b}
            echo ${legend_c}
            echo ${legend_d}

        #    rivet-mkhtml -s --times \
        #        $CMSSW_BASE/src/GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'data' \
        #        --config=$CMSSW_BASE/src/GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU_temp.plot \
        #        -o ${outputDir}/w$((4*i-3))-w$((4*i)) \
        #        ${yodaDir}/ZJ_${tags}.w$((4*i-3)).yoda:"${legend_a}" \
        #        ${yodaDir}/ZJ_${tags}.w$((4*i-2)).yoda:"${legend_b}" \
        #        ${yodaDir}/ZJ_${tags}.w$((4*i-1)).yoda:"${legend_c}" \
        #        ${yodaDir}/ZJ_${tags}.w$((4*i)).yoda:"${legend_d}"

            rivet-mkhtml -s --times \
                $CMSSW_BASE/src/myScripts/patch/CMS_2017_I1471281_Z.yoda:'Data' \
                --config=$CMSSW_BASE/src/myScripts/patch/CMS_2017_I1471281.plot \
                -o ${outputDir}/w$((4*i-3))-w$((4*i)) \
                ${yodaDir}/ZJ_${tags}.w$((4*i-3)).yoda:"${legend_a}" \
                ${yodaDir}/ZJ_${tags}.w$((4*i-2)).yoda:"${legend_b}" \
                ${yodaDir}/ZJ_${tags}.w$((4*i-1)).yoda:"${legend_c}" \
                ${yodaDir}/ZJ_${tags}.w$((4*i)).yoda:"${legend_d}"
        done

        # Remove plot configuration file
        rm $CMSSW_BASE/src/GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU_temp.plot

    done

done
