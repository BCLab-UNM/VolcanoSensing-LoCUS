import subprocess
import os

def buildArgosParameters(type, rmin, rmax, swarmsize, coverage_radius, failures = 0, failtimestep = 1000):
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
          .replace('{swarmsize}', "{}".format(swarmsize))
        fout.write(updated)

  return filename

print "Arena Size"
for radius in range(10, 100, 10):
  print "arena size = {}".format(radius)
  subprocess.call(['argos3', '-c', buildArgosParameters('arena_size', 1, 2, 32, radius)])