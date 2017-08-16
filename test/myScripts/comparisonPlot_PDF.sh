#!/bin/bash

yodaDir=/eos/cms/store/cmst3/group/wmass/kjpena/ntuples/merged
outputDir=/afs/cern.ch/user/k/kjpena/www/comparison_plots_new

rivet-mkhtml -s --times \
    ./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.yoda:'Data' \
    --config=./GeneratorInterface/RivetInterface/data/ATLAS_2015_I1408516_MU.plot \
    -o ${outputDir}/PDF/central_w0 \
    ${yodaDir}/ZJ_central_tune0_PDFpSet13/ZJ_central_tune0_PDFpSet13.w0.yoda:'CUETP8M1 NNPDF2.3' \
    ${yodaDir}/ZJ_central_CUETP8M1/ZJ_central_CUETP8M1.w0.yoda:'CUETP8M1 NNPDF3.0' \
    ${yodaDir}/ZJ_central_tune1_PDFpSet13/ZJ_central_tune1_PDFpSet13.w0.yoda:'CUETP8M2T4 NNPDF2.3' \
    ${yodaDir}/ZJ_central/ZJ_central.w0.yoda:'CUETP8M2T4 NNPDF3.0'
