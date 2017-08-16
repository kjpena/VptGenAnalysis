rivet-mkhtml -s -o ~/public/html/TopRadius/RtScan --times --config=data/CMS_TOP_Radius.plot \
    Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$SM t\bar{t}$+0,1j' \
    Rt_1.00E_01_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.1~~~\kappa=0.01$' \
    Rt_1.00E_02_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.01~~\kappa=0.01$' \
    Rt_1.00E_02_kappat_1.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.01~~\kappa=1.0$' \

rivet-mkhtml -s -o ~/public/html/TopRadius/Rt1e_3_kappaScan --times --config=data/CMS_TOP_Radius.plot \
    Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$SM t\bar{t}$+0,1j' \
    Rt_1.00E_03_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.001~\kappa=0.01$' \
    Rt_1.00E_03_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.001~\kappa=0.1$' \
    Rt_1.00E_03_kappat_1.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.001~\kappa=1.0$' \

rivet-mkhtml -s -o ~/public/html/TopRadius/Rt1e_2_kappaScan --times --config=data/CMS_TOP_Radius.plot \
    Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$SM t\bar{t}$+0,1j' \
    Rt_1.00E_02_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.01~\kappa=0.01$' \
    Rt_1.00E_02_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.01~\kappa=0.1$' \
    Rt_1.00E_02_kappat_1.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.01~\kappa=1.0$' \

rivet-mkhtml -s -o ~/public/html/TopRadius/Rt1e_1_kappaScan --times --config=data/CMS_TOP_Radius.plot \
    Rt_0.00E+00_kappat_0.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$SM t\bar{t}$+0,1j' \
    Rt_1.00E_01_kappat_1.00E_02_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.1~\kappa=0.01$' \
    Rt_1.00E_01_kappat_1.00E_01_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.1~\kappa=0.1$' \
    Rt_1.00E_01_kappat_1.00E+00_xqcut_30.0_njetmax_1_qcut_60.0.yoda:'$R_{\rm t}=0.1~\kappa=1.0$' \


