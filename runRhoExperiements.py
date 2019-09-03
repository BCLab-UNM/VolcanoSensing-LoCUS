import subprocess

def buildArgosParameters(type, rmin, rmax, swarmsize, coverage_radius, failures = 0, failtimestep = 1000):
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

print "rho"
for rmax in [2, 4, 8, 16]:
  print "rmax = {}".format(rmax)
  for rmin in range(1, rmax):
    print "rmin = {}".format(rmin)
    subprocess.call(['argos3', '-c', buildArgosParameters('rho', rmin, rmax, 50, 100)])