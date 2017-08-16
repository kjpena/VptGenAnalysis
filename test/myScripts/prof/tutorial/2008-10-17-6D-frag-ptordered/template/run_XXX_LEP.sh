#!/bin/bash 
##PBS -q dg_long
##PBS -m n

### Set some paths for Rivet
export PATH=$PATH:/griddata/gridsoft/local/hendrik/installation/local/bin:/griddata/gridsoft/local/hendrik/installation/sherpa/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/griddata/gridsoft/local/hendrik/installation/local/lib
export AGILE_GEN_PATH=/griddata/gridsoft/local/hendrik/installation

### Jobnumber and directory for the output files
export JOBNUMBER=XXX
export OUTDIR=/griddata/gridsoft/local/hendrik/rundir/2008-10-17-6D-frag-ptordered/mc/$JOBNUMBER

### Energise!
rivetgun -g Pythia6:418 \
    -a ALEPH_1996_S3486095 \
    -a DELPHI_1995_S3137023 \
    -a DELPHI_1996_S3430090 \
    -a DELPHI_2002_069_CONF_603 \
    -a OPAL_1998_S3780481 \
    -a PDG_HADRON_MULTIPLICITIES \
    -a PDG_HADRON_MULTIPLICITIES_RATIOS \
    -P lep1.params \
    -P fpythia-hadronic.params \
    -P $OUTDIR/../../flavour.params \
    -P $OUTDIR/used_params \
    -H $OUTDIR/out \
    -n 1000000 &> $OUTDIR/out.log

exit 0
