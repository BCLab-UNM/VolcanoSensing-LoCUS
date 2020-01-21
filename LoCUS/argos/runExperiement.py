import subprocess
import os

def buildArgosParameters(type, rmin, rmax, swarmsize, pertubePlume, radius = 100, failureProbability = 0, plumeFailureProbability = 0, failures = 0, failtimestep = 1000, seed = 0, disableHeal = False):
  if not os.path.exists("experiments/coverage/{}".format(type)):
    os.makedirs("experiments/coverage/{}".format(type))
  filename = "experiments/coverage/{}/Fault-Tolerance_{}_{}_{}_{}_{}_{}.xml".format(type, rmin, rmax, swarmsize, pertubePlume, failures, failtimestep)
  with open("experiments/Fault-Tolerance_template.xml", "rt") as fin:
    with open(filename, "wt") as fout:
      for line in fin:
        updated = line.replace('{failures}', "{}".format(failures)) \
          .replace('{rmin}', "{}".format(rmin)) \
          .replace('{rmax}', "{}".format(rmax)) \
          .replace('{radius}', "{}".format(radius)) \
          .replace('{arenaRadius}', "{}".format(radius * 3)) \
          .replace('{perturbPlume}', "{}".format(pertubePlume).lower()  ) \
          .replace('{disableHeal}', "{}".format(disableHeal).lower()  ) \
          .replace('{failureProbability}', "{}".format(failureProbability)) \
          .replace('{plumeFailureProbability}', "{}".format(plumeFailureProbability)) \
          .replace('{failtimestep}', "{}".format(failtimestep)) \
          .replace('{swarmsize}', "{}".format(swarmsize)) \
          .replace('{seed}', "{}".format(seed))
        fout.write(updated)

  return filename
# result_30_3_3_true_0.008_0_24.txt

subprocess.call(['argos3', '-c', buildArgosParameters('swarm_size', 3, 3, 16, True, failureProbability=0, plumeFailureProbability=0, seed=0)])
