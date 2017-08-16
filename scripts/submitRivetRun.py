#!/usr/bin/env python

import os
import sys
import optparse

def main():

    # configure
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-j', '--njobs',         dest='njobs',      help='number of jobs to submit',            default=1, type=int)
    parser.add_option('-i', '--input',         dest='input',      help='input lhe file',                      default=None, type='string')  
    parser.add_option('-n', '--nevts',         dest='nevts',      help='number of events per job',            default=100, type=int)
    parser.add_option('-c', '--cfiFile',       dest='cfiFile',    help='Name of the cfi.'        ,            default='UserCode/RivetAnalysis/python/TT_UEP11_8TeV_pythia6_tauola_cfi')
    parser.add_option('-o', '--outputdir',     dest='outputdir' , help='Name of the local output directory.', default='results')
    parser.add_option('-r', '--rivetCust',     dest='rivetCust' , help='Rivet customisation file',            default='UserCode/RivetAnalysis/rivet_customise.customiseTOP13007')
    parser.add_option('-q', '--queue',         dest='queue'     , help='Batch queue (default=local).'       , default='local')
    (opt, args) = parser.parse_args()
    njobs      = opt.njobs
    nevts      = opt.nevts
    cfiFile    = opt.cfiFile
    rivetCust  = opt.rivetCust
    outputdir  = opt.outputdir
    queue      = opt.queue
    extraOpts  = '' if opt.input is None else '--filein %s' % opt.input

    # prepare output directory
    jobName=os.path.basename(cfiFile)
    jobName=jobName.replace('_cfi.py','')
    pwd = os.getcwd()
    workDir = pwd+'/'+outputdir+'/'+jobName
    if not os.path.exists(workDir):
        print 'creating output directory '+workDir
        os.makedirs(workDir)
    else:
        print 'directory '+workDir+' already exists'

    # create the job submission files
    for i in xrange(0,njobs) :

        myCustomisation  = '\"'
        myCustomisation += 'process.rivetAnalyzer.OutputFile = cms.string(\'out_%d.yoda\')\\n'%(i+1)
        myCustomisation += 'process.RandomNumberGeneratorService.generator.initialSeed=cms.untracked.uint32(%d)\\n'%(i+1)
        myCustomisation += 'process.source.skipEvents=cms.untracked.uint32(%d)\\n'%(i*nevts)
        myCustomisation += 'process.MessageLogger.cerr.FwkReport.reportEvery = 5000\\n'
        myCustomisation += '\"'
        
        #build the script
        scriptName = '%s/rivetJob_%d.sh'%(workDir,i+1)
        script = open(scriptName,'w')
        script.write('#!/bin/bash\n')
        script.write('cd '+pwd+'\n')
        script.write('eval `scramv1 ru -sh`\n')
        script.write('cd '+workDir+'\n')
        cfgFile='rivet_cfg_%d.py'%(i+1)
        script.write('cmsDriver.py %s -s GEN --datatier=GEN-SIM-RAW --conditions auto:mc --eventcontent RAWSIM --no_exec -n %d --python_filename=%s --customise=%s --customise_commands=%s %s\n'%(cfiFile,nevts,cfgFile,rivetCust,myCustomisation,extraOpts))
        script.write('cmsRun %s\n'%cfgFile)
        script.write('cd -')
        script.close()
        os.system('chmod +x '+scriptName)
        
        #run or submit
        if queue=='local':
            print 'Running locally'
            os.system('sh %s'%scriptName)
        else:
            print 'Submitting to queue %s'%queue
            os.system('bsub -q %s %s'%(queue,scriptName))


if __name__ == '__main__':
    main()
