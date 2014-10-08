#!/usr/bin/env python

import os
import sys
import optparse

def main():
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-c', '--configfile'  ,    dest='configfile'       , help='Name of the configfile.'                  , default=None)
    parser.add_option('-o', '--outputdir' ,      dest='outputdir'        , help='Name of the local output directory.'      , default='rivet_batch')
    parser.add_option('-q', '--queue' ,          dest='queue'        ,     help='Batch quque.'      ,                        default='1nh')


    (opt, args) = parser.parse_args()

    if opt.configfile is None:
        parser.error('No input configfile defined!')

    configfile = opt.configfile
    outputdir  = opt.outputdir
    queue = opt.queue


    pwd = os.getcwd()
    path = pwd+'/'+outputdir
    if not os.path.exists(path):
        print 'creating output directory '+path
        os.makedirs(path)
    else:
        print 'directory '+path+' already exists'



    ## create the job submission files
    jobs = []

    name = 'submit_'+configfile.split('/')[-1].replace('_cfg.py','.sh')
    jobs.append(name)
    filename = path+'/'+name
    print 'creating '+filename
    sub = open(filename,'w')
    sub.write('#!/bin/bash\n')
    sub.write('cd '+pwd+'\n')
    sub.write('eval `scramv1 ru -sh`\n')
    cmd = 'cmsRun '+configfile
    sub.write(cmd+'\n')
    os.system('chmod +x '+filename)

    os.chdir(path)
    for job in jobs:
        print 'bsub -q '+queue+' '+job
        os.system('bsub -q '+queue+' '+job)


if __name__ == '__main__':
    main()
    print 'done'
