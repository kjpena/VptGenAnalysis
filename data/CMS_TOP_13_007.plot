# BEGIN PLOT /CMS_TOP_13_007/
Title=CMS preliminary, $\sqrt{s}$ = 8 TeV, L = 19.7 fb$^{-1}$
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r0-j0
CustomLegend=Inclusive, =0 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r0-j1
CustomLegend=Inclusive, =1 extra jet
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r0-j2
CustomLegend=Inclusive, $\geq$2 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r1-j0
CustomLegend=Toward, =0 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r1-j1
CustomLegend=Toward, =1 extra jet
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r1-j2
CustomLegend=Toward, $\geq$2 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r2-j0
CustomLegend=Transverse, =0 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r2-j1
CustomLegend=Transverse, =1 extra jet
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r2-j2
CustomLegend=Transverse, $\geq$2 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r3-j0
CustomLegend=Away, =0 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r3-j1
CustomLegend=Away, =1 extra jet
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/.*-r3-j2
CustomLegend=Away, $\geq$2 extra jets
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/chMult
XLabel=Charged particle multiplicity
YLabel=$1/\sigma~d\sigma / d N_{ch}$
YMin=0.00005
YMax=2
LogY=1
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/chSumPt
XLabel=$\sum p_T$ [GeV]
YLabel=$1/\sigma~d\sigma / d\sum p_T$ [1/GeV]
YMin=0.00005
YMax=2
LogY=1
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/chAvgPt
XLabel=$\bar{p}_T$ [GeV]
YLabel=$1/\sigma~d\sigma / d\bar{p}_T$ [1/GeV]
YMin=0.0005
YMax=2
LogY=1
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/ptttbar
XLabel=$p_{T}(t\bar{t})$ [GeV]
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/dphi2ttbar
XLabel=$\Delta\phi(p,p_{t\bar{t}})$ [deg]
# BEGIN SPECIAL
\psline[linewidth=0.8pt,linecolor=red,linestyle=dashed]\physicscoor( 60,0)\physicscoor( 60,5)
\psline[linewidth=0.8pt,linecolor=red,linestyle=dashed]\physicscoor( 120,0)\physicscoor( 120,5)
\endpsclip
# END SPECIAL
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/ptttbar_chMult
YLabel=$<N_{ch}>$
LogY=0
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/dphi2ttbar_chMult
YLabel=$<N_{ch}>$
LogY=0
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/ptttbar_chSumPt
YLabel=$<\sum p_{T}>$ [GeV] 
LogY=0
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/dphi2ttbar_chSumPt
YLabel=$<\sum p_{T}>$ [GeV] 
LogY=0
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/ptttbar_chAvgPt
YLabel=$<\bar{p}_T>$ [GeV]
LogY=0
# END PLOT

# BEGIN PLOT /CMS_TOP_13_007/dphi2ttbar_chAvgPt
YLabel=$<\bar{p}_T>$ [GeV]
LogY=0
# END PLOT

