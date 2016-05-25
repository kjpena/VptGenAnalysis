#!/bin/bash

baseDir=/store/cmst3/user/psilva/CompositeTop
a=( Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_01_kappat_1.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_01_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_01_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_02_kappat_1.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_02_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_02_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_03_kappat_1.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_03_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.root
    #Rt_1.00E_03_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.root
)
for i in ${a[@]}; do
    tag=${i%.*}
    echo "Analyzing ${tag}"
    cmsRun test/runRivetModule_cfg.py input=${baseDir}/${i} output=${tag}.yoda;
    python ttGenLevel.py ${baseDir}/${i};
done

