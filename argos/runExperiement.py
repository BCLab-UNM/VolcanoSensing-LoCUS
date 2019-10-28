import subprocess
import os

def buildArgosParameters(type, swarmsize, pertubePlume, failureProbability = 0, plumeFailureProbability = 0, failures = 0, failtimestep = 1000, seed = 0):
  if not os.path.exists("experiments/coverage/{}".format(type)):
    os.makedirs("experiments/coverage/{}".format(type))
  filename = "experiments/coverage/{}/Fault-Tolerance_{}_{}_{}_{}.xml".format(type, swarmsize, pertubePlume, failures, failtimestep)
  with open("experiments/Fault-Tolerance_template.xml", "rt") as fin:
    with open(filename, "wt") as fout:
      for line in fin:
        updated = line.replace('{failures}', "{}".format(failures)) \
          .replace('{perturbPlume}', "{}".format(pertubePlume).lower()  ) \
          .replace('{failureProbability}', "{}".format(failureProbability).lower()  ) \
          .replace('{plumeFailureProbability}', "{}".format(plumeFailureProbability).lower()  ) \
          .replace('{failtimestep}', "{}".format(failtimestep)) \
          .replace('{swarmsize}', "{}".format(swarmsize)) \
          .replace('{seed}', "{}".format(seed))
        fout.write(updated)

  return filename

subprocess.call(['argos3', '-c', buildArgosParameters('swarm_size', 10, True, failureProbability = 0.000001, plumeFailureProbability = 1)])
