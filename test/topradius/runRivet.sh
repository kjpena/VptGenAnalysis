#!/bin/bash

baseDir=/store/cmst3/user/psilva/CompositeTop
a=(
    Rt__5.00E_03_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
    Rt__5.00E_03_kappat__5.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.root
)
for i in ${a[@]}; do
    tag=${i%.*}
    echo "Analyzing ${tag}"
    echo "cmsRun test/runRivetModule_cfg.py input=${baseDir}/${i} output=${tag}.yoda"
    cmsRun test/runRivetModule_cfg.py input=${baseDir}/${i} output=${tag}.yoda &
done

