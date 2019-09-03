import subprocess

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

print "Swarm Size"
for swarmsize in [1, 11, 12, 35, 36, 72, 73, 122, 123, 184, 185]:
  print "swarm size = {}".format(swarmsize)
  for rmin in range(1, swarmsize):
    print "rmin = {}".format(rmin)
    subprocess.call(['argos3', '-c', buildArgosParameters('swarm_size', 1, 2, swarmsize, 100)])