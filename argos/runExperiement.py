import subprocess
import os

def buildArgosParameters(type, rmin, rmax, swarmsize, coverage_radius, failures = 0, failtimestep = 1000, seed = 0):
  if not os.path.exists("experiments/coverage/{}".format(type)):
    os.makedirs("experiments/coverage/{}".format(type))
  filename = "experiments/coverage/{}/Fault-Tolerance_{}_{}_{}_{}_{}_{}.xml".format(type, rmin, rmax, swarmsize, coverage_radius, failures, failtimestep)
  with open("experiments/Fault-Tolerance_template.xml", "rt") as fin:
    with open(filename, "wt") as fout:
      for line in fin:
        updated = line.replace('{failures}', "{}".format(failures)) \
          .replace('{rmin}', "{}".format(rmin)) \
          .replace('{rmax}', "{}".format(rmax)) \
          .replace('{coverage_radius}', "{}".format(coverage_radius)) \
          .replace('{failtimestep}', "{}".format(failtimestep)) \
          .replace('{swarmsize}', "{}".format(swarmsize)) \
          .replace('{seed}', "{}".format(seed))
        fout.write(updated)

  return filename

subprocess.call(['argos3', '-c', buildArgosParameters('swarm_size', 1, 2, 73, 100, 58, 1000)])
