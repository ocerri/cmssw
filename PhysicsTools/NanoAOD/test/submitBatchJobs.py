#!/usr/bin/env python
import os, sys, subprocess
import argparse
import commands
import time
#____________________________________________________________________________________________________________
### processing the external os commands
def processCmd(cmd, quite = 0):
    status, output = commands.getstatusoutput(cmd)
    if (status !=0 and not quite):
        print 'Error in processing command:\n   ['+cmd+']'
        print 'Output:\n   ['+output+'] \n'
    return output


#_____________________________________________________________________________________________________________
def main():
    parser = argparse.ArgumentParser()

    parser.add_argument ('--nev', help='number of events per job', default=100)
    parser.add_argument ('--njobs', help='number of jobs', default=10)
    parser.add_argument ('--st_seed', help='starting seed', default=1, type=int)
    parser.add_argument ('--outdir', help='output directory ', default='/afs/cern.ch/user/o/ocerri/cernbox/BPhysics/data/cmsMC_private/HardQCD_bbar_Bu_D0munu_KPimunu_NoPU_10-2-3_v0/jobs_out')
    parser.add_argument ('--force_production', action='store_true', default=False, help='Proceed even if the directory is already existing')
    parser.add_argument ('--queue', help='lsf queue', default='8nh')
    parser.add_argument ('--memory', help='min virtual memory', default='8000')
    parser.add_argument ('--disk', help='min disk space', default='8000')

    args = parser.parse_args()

    nev       = args.nev
    njobs     = int(args.njobs)
    st_seed   = int(args.st_seed)

    outdir    = args.outdir

    queue     = args.queue
    mem       = args.memory
    disk      = args.disk

    if not os.path.exists(outdir):
        os.makedirs(outdir)
        os.makedirs(outdir+'/std/')
        os.makedirs(outdir+'/cfg/')
    elif not args.force_production:
        sys.exit('Output dir: "'+outdir+'" exists.')

    os.system('chmod +x job1023_gen_NoPU_v0.sh')
    print '[Submitting jobs]'
    jobCount=0
    for job in xrange(njobs):

        print 'Submitting job '+str(job+1)+' / '+str(njobs)
        job += st_seed
        seed=str(job)

        basename = 'HardQCD-bbar_{}'.format(seed)

        cmd = 'bsub -o '+outdir+'/std/'+basename +'.out -e '+outdir+'/std/'+basename +'.err -q '+queue
        cmd += ' -R "rusage[mem={}:pool={}]"'.format(mem,disk)
        cmd += ' -J {}'.format(basename)
        cmd += ' /afs/cern.ch/user/o/ocerri/cernbox/BPhysics/MCGeneration/BPH_CMSMCGen/job1023_gen_NoPU_v0.sh'
        cmd += ' '+str(nev)+' '+str(seed)

        print cmd

        # submitting jobs
        output = processCmd(cmd)
        kk = 0
        while (kk<5 and ('error' in output)):
            kk += 1
            time.sleep(1.0);
            output = processCmd(cmd)
            if ('error' not in output):
                print 'Submitted after retry - job '+str(jobCount+1)
            else:
                print output

        jobCount += 1
        time.sleep(0.01);

#_______________________________________________________________________________________
if __name__ == "__main__":
    main()
