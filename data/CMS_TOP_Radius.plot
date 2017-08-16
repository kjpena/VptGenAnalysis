# BEGIN PLOT /CMS_TOP_Radius/*
Title=Madgraph$+$Pythia 8 ($\sqrt{s}=$13 TeV)
RatioPlotYMin=0.2
RatioPlotYMax=1.8
RatioPlotErrorBandColor=lightgray
LogY=0
# END PLOT

# BEGIN PLOT /CMS_TOP_Radius/xsec
YLabel=$\sigma(pp\rightarrow t\bar{t}+X)$ [pb]
LegendXPos=0.1
LegendYPos=0.4
# END PLOT

# BEGIN PLOT /CMS_TOP_Radius/inistate
XLabel=Initial state (2=gg,1=qg,0=$q\bar{q}$)
YLabel=Events [pb]
LegendXPos=0.05
LegendYPos=0.9
# END PLOT

# BEGIN PLOT /CMS_TOP_Radius/dphill
XLabel=$\Delta\phi(\ell,\ell')$ [rad]
YLabel=Events [pb]
LegendXPos=0.1
LegendYPos=0.9
Rebin=2
# END PLOT

# BEGIN PLOT /CMS_TOP_Radius/dphibb
XLabel=$\Delta\phi(b,b')$ [rad]
YLabel=Events [pb]
LegendXPos=0.1
LegendYPos=0.9
Rebin=2
# END PLOT

# BEGIN PLOT /CMS_TOP_Radius/mttbar
XLabel=$M(t\bar{t})$ [GeV]
YLabel=Events [pb]
Rebin=4
# END PLOT
