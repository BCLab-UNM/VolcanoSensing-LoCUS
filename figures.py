import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import csv
import numpy as np	

def is_not_blank(s):
    return bool(s and s.strip())

def processFile(filename):
    data = {}
    data['failed'] = False
    with open(filename, 'r') as fp:
        for cnt, line in enumerate(fp):
            if line.startswith("Time:"):
                data['time'] = int(line[5:len(line) - 1:])
            if line.startswith("[1;32mencountered plume:"):
                data['plumeEncountered'] = int(line[37:len(line) - 5:])
            if line.startswith("[1;32mHealing took:"):
                data['healingTook'] = int(line[32:len(line) - 5:])
            if line.startswith("[1;32mfailed:true"):
                data['failed'] = True
    return data

def getFTResults(result_name, swarmsize, rmin, rmax, perturbed, failureProbability, plumeFailureProbability, seed):
    return "results/{}/result_{}_{}_{}_{}_{}_{}_{}.txt".format(result_name, swarmsize, rmin, rmax, perturbed, failureProbability, plumeFailureProbability, seed).lower()

def getBaselineResults(result_name, swarmsize, perturbed,  failureProbability, plumeFailureProbability, seed):
    return "results/{}/result_{}_{}_{}_{}_{}.txt".format(result_name, swarmsize, perturbed,  failureProbability, plumeFailureProbability, seed).lower()

# FT Failures
# parallel $CMD_STR ::: 30 ::: 3 ::: 3 ::: false ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 ::: 0 ::: $(seq 100)
# parallel $CMD_STR ::: 30 ::: 3 ::: 3 ::: false ::: 0 ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 ::: $(seq 100)

swarmsizes = range(2, 21)
failures = ['0.1', '0.01', '0.001', '0.0001', '0.00001', '0.000001', '0.0000001', '0.00000001']
samples = range(1, 101)
failureTime = {}
plumeFailureTime = {}

for failureCount in failures:
    failureTime.update({failureCount : []})
    plumeFailureTime.update({failureCount : []})
    for s in samples:
        failureTime.get(failureCount).append(processFile(getFTResults('ft_failures', 10, 3, 3, False, failureCount, 0, s)))
        plumeFailureTime.get(failureCount).append(processFile(getFTResults('ft_failures', 10, 3, 3, False, 0, failureCount, s)))


# FT Perturbed
# parallel $CMD_STR ::: $(seq 100) ::: 3 ::: 3 ::: true ::: 0 ::: 0 ::: $(seq 100)
ftper_failureTime = {}

for swarmsize in swarmsizes:
    ftper_failureTime.update({swarmsize : []})
    for s in samples:
        failTime = processFile(getFTResults('ft_per_swarmsize', swarmsize, 3, 3, True, 0, 0, s))
        if failTime is not None:
            ftper_failureTime.get(swarmsize).append(failTime)

# FT Swarm Size
# parallel $CMD_STR ::: $(seq 100) ::: 3 ::: 3 ::: false ::: 0 ::: 0 ::: $(seq 100)
ft_swarmsize_failureTime = {}

for swarmsize in swarmsizes:
    ft_swarmsize_failureTime.update({swarmsize : []})
    for s in samples:
        ft_swarmsize_failureTime.get(swarmsize).append(processFile(getFTResults('ft_swarmsize', swarmsize, 3, 3, False, 0, 0, s)))

# Baseline Failures
# parallel $CMD_STR ::: 30 ::: false ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 0.00000001 ::: 0 ::: $(seq 100)
# parallel $CMD_STR ::: 30 ::: false ::: 0 ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 0.00000001 ::: $(seq 100)
baseline_swarmsize_failureTime = {}
baseline_swarmsize_plumeFailureTime = {}
    
for failureCount in failures:
    baseline_swarmsize_failureTime.update({failureCount : []})
    baseline_swarmsize_plumeFailureTime.update({failureCount : []})
    for s in samples:
        baseline_swarmsize_failureTime.get(failureCount).append(processFile(getBaselineResults('unc_failures', 10, False, failureCount, 0, s)))
        baseline_swarmsize_plumeFailureTime.get(failureCount).append(processFile(getBaselineResults('unc_failures', 10, False, 0, failureCount, s)))

# Baseline Perturbed
# parallel $CMD_STR ::: $(seq 100) ::: true ::: 0 ::: 0 ::: $(seq 100)
baseline_per_swarmsize_failureTime = {}

for swarmsize in swarmsizes:
    baseline_per_swarmsize_failureTime.update({swarmsize : []})
    for s in samples:
        baseline_per_swarmsize_failureTime.get(swarmsize).append(processFile(getBaselineResults('unc_per_swarmsize', swarmsize, True, 0, 0, s)))

# Baseline Swarm Size
# parallel $CMD_STR ::: $(seq 100) ::: false ::: 0 ::: 0 ::: $(seq 100)
baseline_swarmsize = {}

for swarmsize in swarmsizes:
    baseline_swarmsize.update({swarmsize : []})
    for s in samples:
        baseline_swarmsize.get(swarmsize).append(processFile(getBaselineResults('unc_swarmsize', swarmsize, False, 0, 0, s)))

# Swarm size
plt.clf()

maxx = []
x = []
err = []
ft_data = []

for key in swarmsizes:
    maxx.append(max([value['time'] for value in ft_swarmsize_failureTime[key]]))
    x.append(np.mean([value['time'] for value in ft_swarmsize_failureTime[key]]))
    err.append(np.std([value['time'] for value in ft_swarmsize_failureTime[key]]))

maxx2 = []
x2 = []
err2 = []

for key in swarmsizes:
    maxx2.append(max([value['time'] for value in baseline_swarmsize[key]]))
    x2.append(np.mean([value['time'] for value in baseline_swarmsize[key]]))
    err2.append(np.std([value['time'] for value in baseline_swarmsize[key]]))

plt.errorbar(swarmsizes, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="Uncoordinated Swarm")
plt.plot(swarmsizes, maxx, label="Max Fault Tolerant Swarm")
plt.plot(swarmsizes, maxx2, label="Max Uncoordinated Swarm")

pp = PdfPages('figs/swarmSize.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Plume Source by Swarm Size')
plt.legend()
pp.savefig()
pp.close()


# Swarm Size Box plots
plt.clf()


#ft_data.append([value['time'] for value in ftper_failureTime[5]])
#ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[5]])
ft_data.append([value['time'] for value in ft_swarmsize_failureTime[10]])
ft_data.append([value['time'] for value in baseline_swarmsize[10]])
ft_data.append([value['time'] for value in ft_swarmsize_failureTime[15]])
ft_data.append([value['time'] for value in baseline_swarmsize[15]])
ft_data.append([value['time'] for value in ft_swarmsize_failureTime[20]])
ft_data.append([value['time'] for value in baseline_swarmsize[20]])

plt.boxplot(ft_data, notch=1, labels=['FT 10', 'UNC 10', 'FT 15', 'UNC 15', 'FT 20', 'UNC 20'])

pp = PdfPages('figs/swarmSizeBoxPlots.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Perturbed Plume Source by Swarm Size')
plt.legend()
pp.savefig()
pp.close()

# Perturbed
plt.clf()

maxx = []
x = []
err = []
    

for key in swarmsizes:
    maxx.append(max([value['time'] for value in ftper_failureTime[key]]))
    x.append(np.mean([value['time'] for value in ftper_failureTime[key]]))
    err.append(np.std([value['time'] for value in ftper_failureTime[key]]))

maxx2 = []
x2 = []
err2 = []

for key in swarmsizes:
    maxx2.append(max([value['time'] for value in baseline_per_swarmsize_failureTime[key]]))
    x2.append(np.mean([value['time'] for value in baseline_per_swarmsize_failureTime[key]]))
    err2.append(np.std([value['time'] for value in baseline_per_swarmsize_failureTime[key]]))

plt.errorbar(swarmsizes, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="Uncoordinated Swarm")
plt.plot(swarmsizes, maxx, label="Max Fault Tolerant Swarm")
plt.plot(swarmsizes, maxx2, label="Max Uncoordinated Swarm")

pp = PdfPages('figs/perturbed.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Perturbed Plume Source by Swarm Size')
plt.legend()
pp.savefig()
pp.close()

# Perturbed Box plots
plt.clf()


#ft_data.append([value['time'] for value in ftper_failureTime[5]])
#ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[5]])
ft_data.append([value['time'] for value in ftper_failureTime[10]])
ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[10]])
ft_data.append([value['time'] for value in ftper_failureTime[15]])
ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[15]])
ft_data.append([value['time'] for value in ftper_failureTime[20]])
ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[20]])

plt.boxplot(ft_data, notch=1, labels=['FT 10', 'UNC 10', 'FT 15', 'UNC 15', 'FT 20', 'UNC 20'])

pp = PdfPages('figs/perturbedBoxPlots.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Perturbed Plume Source by Swarm Size')
plt.legend()
pp.savefig()
pp.close()

# Failures
plt.clf()

x = []
err = []

for key in failures:
    completionTime = [value['time'] for value in failureTime[key] if 'time' in value and not value['failed']]
    x.append(np.mean(completionTime))
    err.append(np.std(completionTime))

x2 = []
err2 = []

for key in failures:
    completionTime = [value['time'] for value in baseline_swarmsize_failureTime[key] if 'time' in value and not value['failed']]
    x2.append(np.mean(completionTime))
    err2.append(np.std(completionTime))

plt.errorbar(failures, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(failures, x2, err2, fmt='-o', label="Uncoordinated Swarm")

pp = PdfPages('figs/failures.pdf')
plt.xlabel('Failure Probability')
plt.xscale('log')       
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Plume Source With Failures')
plt.legend()
pp.savefig()
pp.close()

# Failure Success

plt.clf()

success = []
baselinesuccess = []

for key in failures:
    completionTime = [value['time'] for value in failureTime[key] if 'time' in value and not value['failed']]
    success.append(len(completionTime))

x2 = []
err2 = []

for key in failures:
    completionTime = [value['time'] for value in baseline_swarmsize_failureTime[key] if 'time' in value and not value['failed']]
    baselinesuccess.append(len(completionTime))

plt.plot(failures, success, label="Fault Tolerant Swarm Successes")
plt.plot(failures, baselinesuccess, label="Uncoordinated Swarm Successes")

pp = PdfPages('figs/failureSuccess.pdf')
plt.xlabel('Failure Probability')
plt.xscale('log')       
plt.ylabel('Successfully Found Max Flux')
plt.title('Count of Swarms to Find Plume Source with General Failures')
plt.legend()
pp.savefig()
pp.close()

# Plume Failures
plt.clf()

x = []
err = []

for key in failures:
    completionTime = [value['time'] for value in plumeFailureTime[key] if 'time' in value and not value['failed']]
    x.append(np.mean(completionTime))
    err.append(np.std(completionTime))

x2 = []
err2 = []

for key in failures:
    completionTime = [value['time'] for value in baseline_swarmsize_plumeFailureTime[key] if 'time' in value and not value['failed']]
    x2.append(np.mean(completionTime))
    err2.append(np.std(completionTime))

plt.errorbar(failures, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(failures, x2, err2, fmt='-o', label="Uncoordinated Swarm")

pp = PdfPages('figs/plumeFailures.pdf')
plt.xlabel('Failure Probability')
plt.xscale('log')       
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Plume Source with Plume Failures')
plt.legend()
pp.savefig()
pp.close()

# Plume Failure Success

plt.clf()

success = []
baselinesuccess = []

for key in failures:
    completionTime = [value['time'] for value in plumeFailureTime[key] if 'time' in value and not value['failed']]
    success.append(len(completionTime))

x2 = []
err2 = []

for key in failures:
    completionTime = [value['time'] for value in baseline_swarmsize_plumeFailureTime[key] if 'time' in value and not value['failed']]
    baselinesuccess.append(len(completionTime))

plt.plot(failures, success, label="Fault Tolerant Swarm Successes")
plt.plot(failures, baselinesuccess, label="Uncoordinated Swarm Successes")

pp = PdfPages('figs/plumeFailureSuccess.pdf')
plt.xlabel('Plume Failure Probability')
plt.xscale('log')       
plt.ylabel('Successfully Found Max Flux')
plt.title('Count of Swarms to Find Plume Source with Plume Failures')
plt.legend()
pp.savefig()
pp.close()

# Time to initial plume contact
plt.clf()

x = []
err = []

for key in swarmsizes:
    contactTime = [value['plumeEncountered'] for value in ft_swarmsize_failureTime[key] if 'plumeEncountered' in value]
    x.append(np.mean(contactTime))
    err.append(np.std(contactTime))

x2 = []
err2 = []

for key in swarmsizes:
    contactTime = [value['plumeEncountered'] for value in baseline_swarmsize[key] if 'plumeEncountered' in value]
    x2.append(np.mean(contactTime))
    err2.append(np.std(contactTime))

plt.errorbar(swarmsizes, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="Uncoordinated Swarm")

pp = PdfPages('figs/plumeEncountered.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Initial Plume Contact')
plt.legend()
pp.savefig()
pp.close()

# Once plume is encountered, how long to max flux?
plt.clf()

x = []
err = []

for key in swarmsizes:
    contactTime = [value['time'] - value['plumeEncountered'] for value in ft_swarmsize_failureTime[key] if 'plumeEncountered' in value and 'time' in value and not value['failed']]
    x.append(np.mean(contactTime))
    err.append(np.std(contactTime))

x2 = []
err2 = []

for key in swarmsizes:
    contactTime = [value['time'] - value['plumeEncountered'] for value in baseline_swarmsize[key] if 'plumeEncountered' in value and 'time' in value and not value['failed']]
    x2.append(np.mean(contactTime))
    err2.append(np.std(contactTime))

plt.errorbar(swarmsizes, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="Uncoordinated Swarm")

pp = PdfPages('figs/maxFlux.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Max Flux after Plume Contact')
plt.legend()
pp.savefig()
pp.close()

# Time to initial perturbed plume contact
plt.clf()

x = []
err = []

for key in swarmsizes:
    contactTime = [value['plumeEncountered'] for value in ftper_failureTime[key] if 'plumeEncountered' in value]
    x.append(np.mean(contactTime))
    err.append(np.std(contactTime))

x2 = []
err2 = []

for key in swarmsizes:
    contactTime = [value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[key] if 'plumeEncountered' in value]
    x2.append(np.mean(contactTime))
    err2.append(np.std(contactTime))

plt.errorbar(swarmsizes, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="Uncoordinated Swarm")

pp = PdfPages('figs/perturbedPlumeEncountered.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Initial Perturbed Plume Contact')
plt.legend()
pp.savefig()
pp.close()

# Once plume is encountered, how long to max flux?
plt.clf()

x = []
err = []

for key in swarmsizes:
    contactTime = [value['time'] - value['plumeEncountered'] for value in ftper_failureTime[key] if 'plumeEncountered' in value and 'time' in value and not value['failed']]
    x.append(np.mean(contactTime))
    err.append(np.std(contactTime))

x2 = []
err2 = []

for key in swarmsizes:
    contactTime = [value['time'] - value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[key] if 'plumeEncountered' in value and 'time' in value and not value['failed']]
    x2.append(np.mean(contactTime))
    err2.append(np.std(contactTime))

plt.errorbar(swarmsizes, x, err, fmt='-o', label="Fault Tolerant Swarm")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="Uncoordinated Swarm")

pp = PdfPages('figs/perturbedMaxFlux.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Max Flux after Perturbed Plume Contact')
plt.legend()
pp.savefig()
pp.close()

# As failures go how many reach max flux?


# Write all data to csvs

with open('ft_swarm.csv', mode='w') as file:
    csvWriter = csv.writer(file)

    csvWriter.writerow(['Swarm Size', 'RMin', 'RMax', 'Perturbe Plume', 'Failure', 'Plume Failure', 'Time to Max Flux', 'Plume Encountered Time', 'Healing Time', 'Swarm Failed'])

    for failureCount in failures:
        for value in failureTime.get(failureCount):
            csvWriter.writerow([30, 3, 3, False, failureCount, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
        for value in plumeFailureTime.get(failureCount):
            csvWriter.writerow([30, 3, 3, False, 0, failureCount, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
    for swarmsize in swarmsizes:
        for value in ftper_failureTime.get(swarmsize):
            csvWriter.writerow([swarmsize, 3, 3, True, 0, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
        for value in ft_swarmsize_failureTime.get(swarmsize):
            csvWriter.writerow([swarmsize, 3, 3, False, 0, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])

with open('unc_swarm.csv', mode='w') as file:
    csvWriter = csv.writer(file)

    csvWriter.writerow(['Swarm Size', 'Perturbe Plume', 'Failure', 'Plume Failure', 'Time to Max Flux', 'Plume Encountered Time', 'Healing Time', 'Swarm Failed'])

    for failureCount in failures:
        for value in baseline_swarmsize_failureTime.get(failureCount):
            csvWriter.writerow([30, False, failureCount, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
        for value in baseline_swarmsize_plumeFailureTime.get(failureCount):
            csvWriter.writerow([30, False, 0, failureCount, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
    for swarmsize in swarmsizes:
        for value in baseline_per_swarmsize_failureTime.get(swarmsize):
            csvWriter.writerow([swarmsize, True, 0, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
        for value in baseline_swarmsize.get(swarmsize):
            csvWriter.writerow([swarmsize, False, 0, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])


