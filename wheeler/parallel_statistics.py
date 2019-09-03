import os, subprocess, argparse

parser = argparse.ArgumentParser(description='NeatFA Evolving script')
parser.add_argument('--PBS_O_WORKDIR', dest='workdir')

args = parser.parse_args()

temp_dir = '{}/temp'.format(args.workdir)
experiments_dir = '{}/experiments'.format(args.workdir)

evaluate_fitness_command = [
'parallel',
'singularity',
'run',
'-B', '{}:/root/temp/'.format(temp_dir),
'-B', '{}:/neatfa/argos/experiments/'.format(experiments_dir),
'$PBS_O_WORKDIR/neatfa_test3.simg',
'--DisablePheromone', '{}',
':::', '1', '0'
]

subprocess.call(evaluate_fitness_command, env=os.environ.copy())

for epoch in range(10):
    print "epoch {}".format(epoch)
    # Evolve population
    # Evaluate population
