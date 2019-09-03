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

print "\nfailures"

for rmin in range(1, 4):
  print "rmin = {}".format(rmin)
  print "failures = 0"
  print "trial = 1"
  subprocess.call(['argos3', '-c', buildArgosParameters('failures', rmin, 4, 73, 100)])
  for failures in range(5, 70, 5):
    print "failures = {}".format(failures)
    for t in range(1, 10):
      print "trial = {}".format(t)
      subprocess.call(['argos3', '-c', buildArgosParameters('failures', rmin, 4, 73, 100, failures)])

  print "failures = 72"
  for t in range(1, 10):
    print "trial = {}".format(t)
    subprocess.call(['argos3', '-c', buildArgosParameters('failures', rmin, 4, 73, 100, 72)])