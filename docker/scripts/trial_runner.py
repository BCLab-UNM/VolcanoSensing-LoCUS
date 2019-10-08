import os, argparse, datetime, random, time, subprocess, tempfile

def run_trial(args):
    with open('/faulttolerance/argos/experiments/Fault-Tolerance_template.xml', "rt") as fin:
        with tempfile.NamedTemporaryFile(dir="/faulttolerance/argos/experiments/",  suffix=".xml", delete=True) as configuration_file:

            for line in fin:
                updated = line\
                    .replace('{failures}', "{}".format(args.failures))\
                    .replace('{rmin}', "{}".format(args.rmin))\
                    .replace('{rmax}', "{}".format(args.rmax))\
                    .replace('{coverage_radius}', "{}".format(args.coverage_radius))\
                    .replace('{failtimestep}', "{}".format(args.failtimestep))\
                    .replace('{swarmsize}', "{}".format(args.swarmsize))\
                    .replace('{seed}', "{}".format(args.seed))
                configuration_file.write(updated)
            configuration_file.flush()
            # Exectute Argos Experiment and write output to temp file
            print subprocess.check_output(["argos3", "-c", configuration_file.name])
