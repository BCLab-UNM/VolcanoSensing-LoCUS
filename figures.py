import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
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

def timeFormatter(value, tick_number):
    seconds = int(value / 16)
    m, s = divmod(seconds, 60)
    return '{:02d}:{:02d}'.format(m, s)
    

# plt.rcParams['font.size'] = 14

# FT Failures
# parallel $CMD_STR ::: 30 ::: 3 ::: 3 ::: false ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 ::: 0 ::: $(seq 100)
# parallel $CMD_STR ::: 30 ::: 3 ::: 3 ::: false ::: 0 ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 ::: $(seq 100)

timeFormatter(1000, 1)

swarmsizes = range(2, 21)
#failures = ['0.1', '0.01', '0.001', '0.0001', '0.00001', '0.000001']
failures = ['0.1', '0.09', '0.08', '0.07', '0.06', '0.05', '0.04', '0.03', '0.03', '0.01', '0.009', '0.008', '0.007', '0.006', '0.005', '0.004', '0.003', '0.002', '0.001', '0.0009', '0.0008', '0.0007', '0.0006', '0.0005', '0.0004', '0.0003', '0.0002', '0.0001', '0.00009', '0.00008', '0.00007', '0.00006', '0.00005', '0.00004', '0.00003', '0.00002', '0.00001', '0.000009', '0.000008', '0.000007', '0.000006', '0.000005', '0.000004', '0.000003', '0.000002', '0.000001']
samples = range(1, 101)
failureTime = {}
plumeFailureTime = {}
intolerantFailures = {}
intolerantPlumeFailures = {}

for failureCount in failures:
    failureTime.update({failureCount : []})
    plumeFailureTime.update({failureCount : []})
    intolerantFailures.update({failureCount : []})
    intolerantPlumeFailures.update({failureCount : []})
    for s in samples:
        failureTime.get(failureCount).append(processFile(getFTResults('ft_failures', 20, 3, 3, True, failureCount, 0, s)))
        plumeFailureTime.get(failureCount).append(processFile(getFTResults('ft_failures', 20, 3, 3, True, 0, failureCount, s)))
        intolerantFailures.get(failureCount).append(processFile(getFTResults('unt_failures', 20, 3, 3, True, failureCount, 0, s)))
        intolerantPlumeFailures.get(failureCount).append(processFile(getFTResults('unt_failures', 20, 3, 3, True, 0, failureCount, s)))


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
# parallel $CMD_STR ::: 20 ::: false ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 0.00000001 ::: 0 ::: $(seq 100)
# parallel $CMD_STR ::: 20 ::: false ::: 0 ::: 1 0.1 0.01 0.001 0.0001 0.00001 0.000001 0.0000001 0.00000001 0.00000001 ::: $(seq 100)
baseline_swarmsize_failureTime = {}
baseline_swarmsize_plumeFailureTime = {}
    
for failureCount in failures:
    baseline_swarmsize_failureTime.update({failureCount : []})
    baseline_swarmsize_plumeFailureTime.update({failureCount : []})
    for s in samples:
        baseline_swarmsize_failureTime.get(failureCount).append(processFile(getBaselineResults('unc_failures', 20, True, failureCount, 0, s)))
        baseline_swarmsize_plumeFailureTime.get(failureCount).append(processFile(getBaselineResults('unc_failures', 20, True, 0, failureCount, s)))

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

plt.errorbar(swarmsizes, x, err, fmt='-o', label="LoCUS", color="#f1a340")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="MoBS", color="#998ec3", linestyle="--")
#['#f1a340','#f7f7f7','#998ec3']

pp = PdfPages('figs/swarmSize.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Max Flux by Swarm Size')
plt.legend()
pp.savefig()
pp.close()

print "ave differences:"
print([a - b for a, b in zip(x, x2)])

print "stddevs"
print err
print err2

# Swarm size with max
plt.clf()

plt.errorbar(swarmsizes, x, err, fmt='-o', label="LoCUS")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="MoBS")
plt.plot(swarmsizes, maxx, label="Max LoCUS")
plt.plot(swarmsizes, maxx2, label="Max MoBS")

pp = PdfPages('figs/swarmSizeWithMax.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Max Flux by Swarm Size')
plt.legend()
pp.savefig()
pp.close()


# Swarm Size Box plots
plt.clf()

ft_data = []


swarmsizesToShow = [5, 10, 20]
labels = []


for i in swarmsizesToShow :

    #ft_data.append([value['time'] for value in ftper_failureTime[5]])
    #ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[5]])
    ft_data.append([value['time'] for value in ft_swarmsize_failureTime[i]])
    ft_data.append([value['time'] for value in baseline_swarmsize[i]])
    ft_data.append([value['time'] for value in baseline_swarmsize[i]])
    ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[i]])

    labels.append("LoCUS {}".format(i))
    labels.append("LoCUS {}".format(i))
    labels.append("MoBS {}".format(i))
    labels.append("MoBS {}".format(i))

plt.boxplot(ft_data, notch=1, labels=labels)

pp = PdfPages('figs/swarmSizeBoxPlots.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Max Flux by Swarm Size')
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

plt.errorbar(swarmsizes, x, err, fmt='-o', label="LoCUS", color="#f1a340")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="MoBS", color="#998ec3", linestyle="--")

pp = PdfPages('figs/perturbed.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Perturbed Max Flux by Swarm Size')
plt.legend()
pp.savefig()
pp.close()

# Perturbed with max
plt.clf()

plt.errorbar(swarmsizes, x, err, fmt='-o', label="LoCUS")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="MoBS")
plt.plot(swarmsizes, maxx, label="Max LoCUS")
plt.plot(swarmsizes, maxx2, label="Max MoBS")

pp = PdfPages('figs/perturbedWithMax.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Perturbed Max Flux by Swarm Size')
plt.legend()
pp.savefig()
pp.close()

# Perturbed Box plots
plt.clf()

swarmsizesToShow = [5, 10, 20]
labels = []
ft_data = []

for i in swarmsizesToShow :

    #ft_data.append([value['time'] for value in ftper_failureTime[5]])
    #ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[5]])
    ft_data.append([value['time'] for value in ft_swarmsize_failureTime[i]])
    ft_data.append([value['time'] for value in ftper_failureTime[i]])
    ft_data.append([value['time'] for value in baseline_swarmsize[i]])
    ft_data.append([value['time'] for value in baseline_per_swarmsize_failureTime[i]])

    labels.append("LoCUS {}".format(i))
    labels.append("LoCUS {}".format(i))
    labels.append("MoBS {}".format(i))
    labels.append("MoBS {}".format(i))

plt.boxplot(ft_data, notch=1, labels=labels)

pp = PdfPages('figs/perturbedBoxPlots.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Perturbed Max Flux by Swarm Size')
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

x3 = []
err3 = []

for key in failures:
    completionTime = [value['time'] for value in intolerantFailures[key] if 'time' in value and not value['failed']]
    x3.append(np.mean(completionTime))
    err3.append(np.std(completionTime))

plt.errorbar(failures, x, err, fmt='-o', label="LoCUS")
plt.errorbar(failures, x3, err3, fmt='-o', label="LoCUS without Healing")
plt.errorbar(failures, x2, err2, fmt='-o', label="MoBS")

pp = PdfPages('figs/failures.pdf')
plt.xlabel('Failure Probability')
plt.xscale('log')       
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Max Flux With Failures')
plt.legend()
pp.savefig()
pp.close()

# Failure Success

plt.clf()

success = []
successWithoutHeal = []
baselinesuccess = []
plumesuccess = []
plumesuccessWithoutHeal = []
plumebaselinesuccess = []

for key in failures:
    completionTime = [value['time'] for value in failureTime[key] if 'time' in value and not value['failed']]
    success.append(len(completionTime))

for key in failures:
    completionTime = [value['time'] for value in baseline_swarmsize_failureTime[key] if 'time' in value and not value['failed']]
    baselinesuccess.append(len(completionTime))

for key in failures:
    completionTime = [value['time'] for value in intolerantFailures[key] if 'time' in value and not value['failed']]
    successWithoutHeal.append(len(completionTime))


for key in failures:
    completionTime = [value['time'] for value in plumeFailureTime[key] if 'time' in value and not value['failed']]
    plumesuccess.append(len(completionTime))

for key in failures:
    completionTime = [value['time'] for value in baseline_swarmsize_plumeFailureTime[key] if 'time' in value and not value['failed']]
    plumebaselinesuccess.append(len(completionTime))

for key in failures:
    completionTime = [value['time'] for value in intolerantPlumeFailures[key] if 'time' in value and not value['failed']]
    plumesuccessWithoutHeal.append(len(completionTime))


g1, = plt.plot(failures, success, label="LoCUS", color="#f1a340", linewidth=2.4)
g2, = plt.plot(failures, successWithoutHeal, label="LoCUS 20 w/o healing", color="#f1a340")
g3, = plt.plot(failures, baselinesuccess, label="MoBS", color="#998ec3", linestyle='--', linewidth=2.4)
genericLegend = plt.legend([g1, g2, g3], ['LoCUS 20', 'LoCUS 20 w/o healing', 'MoBS 20'], title='Generic Failures', loc=(0.02, 0.3))

p1, = plt.plot(failures, plumesuccess, label="LoCUS 20", color="#abdda4", linewidth=2.4)
p2, = plt.plot(failures, plumesuccessWithoutHeal, label="LoCUS 20 w/o healing", color="#abdda4")
p3, = plt.plot(failures, plumebaselinesuccess, label="MoBS 20", color="#f14040", linestyle=':', linewidth=2.4)
plt.legend([p1, p2, p3], ['LoCUS 20', 'LoCUS 20 w/o healing', 'MoBS 20'], title='In-Plume Failures', loc=(0.02, 0.02))

pp = PdfPages('figs/failureSuccess.pdf')
plt.xlabel('Failure Probability ($p_f$)')
plt.xscale('log')       
plt.ylabel('Successfully Found Max Flux')
plt.title('Count of Swarms to Find Max Flux over Failure Probability')
plt.gca().add_artist(genericLegend)

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

x3 = []
err3 = []

for key in failures:
    completionTime = [value['time'] for value in intolerantPlumeFailures[key] if 'time' in value and not value['failed']]
    x3.append(np.mean(completionTime))
    err3.append(np.std(completionTime))

plt.errorbar(failures, x, err, fmt='-o', label="LoCUS")
plt.errorbar(failures, x3, err3, fmt='-o', label="LoCUS without heal")
plt.errorbar(failures, x2, err2, fmt='-o', label="MoBS")

pp = PdfPages('figs/plumeFailures.pdf')
plt.xlabel('Failure Probability')
plt.xscale('log')       
plt.ylabel('Simulation Timesteps')
plt.title('Time to Find Max Flux with Plume Failures')
plt.legend()
pp.savefig()
pp.close()

# Plume Failure Success

plt.clf()

success = []
successWithoutHeal = []
baselinesuccess = []

for key in failures:
    completionTime = [value['time'] for value in plumeFailureTime[key] if 'time' in value and not value['failed']]
    success.append(len(completionTime))

for key in failures:
    completionTime = [value['time'] for value in baseline_swarmsize_plumeFailureTime[key] if 'time' in value and not value['failed']]
    baselinesuccess.append(len(completionTime))

for key in failures:
    completionTime = [value['time'] for value in intolerantPlumeFailures[key] if 'time' in value and not value['failed']]
    successWithoutHeal.append(len(completionTime))

plt.plot(failures, success, label="LoCUS", color="#f1a340")
plt.plot(failures, successWithoutHeal, label="LoCUS without healing", color="#abdda4")
plt.plot(failures, baselinesuccess, label="MoBS", color="#998ec3", linestyle='--')

pp = PdfPages('figs/plumeFailureSuccess.pdf')
plt.xlabel('Plume Failure Probability')
plt.xscale('log')       
plt.ylabel('Successfully Found Max Flux')
plt.title('Count of Swarms to Find Max Flux with Plume Failures')
plt.legend(loc='lower left')
pp.savefig()
pp.close()

# Time to initial plume contact
plt.clf()

swarmsizesToShow = [5, 10, 20]

contactTimes = []
contactTimeMedians = []
totalTimes = []
totalTimeStd = []
labels = []
encounteredColors = []
totalTimeColors = []
encounteredHatch = []
totalTimeHatch = []
for i in swarmsizesToShow :
    contactTimes.append(np.mean([value['plumeEncountered'] for value in ft_swarmsize_failureTime[i] if 'plumeEncountered' in value]))
    contactTimes.append(np.mean([value['plumeEncountered'] for value in ftper_failureTime[i] if 'plumeEncountered' in value]))
    contactTimes.append(np.mean([value['plumeEncountered'] for value in baseline_swarmsize[i] if 'plumeEncountered' in value]))
    contactTimes.append(np.mean([value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[i] if 'plumeEncountered' in value]))

    contactTimeMedians.append(np.median([value['time'] for value in ft_swarmsize_failureTime[i] if 'time' in value]))
    contactTimeMedians.append(np.median([value['time'] for value in ftper_failureTime[i] if 'time' in value]))
    contactTimeMedians.append(np.median([value['time'] for value in baseline_swarmsize[i] if 'time' in value]))
    contactTimeMedians.append(np.median([value['time'] for value in baseline_per_swarmsize_failureTime[i] if 'time' in value]))

    totalTimeStd.append(np.std([value['time'] for value in ft_swarmsize_failureTime[i] if 'time' in value]))
    totalTimeStd.append(np.std([value['time'] for value in ftper_failureTime[i] if 'time' in value]))
    totalTimeStd.append(np.std([value['time'] for value in baseline_swarmsize[i] if 'time' in value]))
    totalTimeStd.append(np.std([value['time'] for value in baseline_per_swarmsize_failureTime[i] if 'time' in value]))

    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in ft_swarmsize_failureTime[i] if 'plumeEncountered' in value]))
    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in ftper_failureTime[i] if 'plumeEncountered' in value]))
    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in baseline_swarmsize[i] if 'plumeEncountered' in value]))
    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[i] if 'plumeEncountered' in value]))

    labels.append("LoCUS {}".format(i))
    #labels.append("LoCUS {}".format(i))
    labels.append("MoBS {}".format(i))
    #labels.append("MoBS {}".format(i))
    encounteredColors.append('#f1a340')
    encounteredColors.append('#f1a340')
    encounteredHatch.append('')
    encounteredHatch.append('\\\\\\')
    encounteredHatch.append('')
    encounteredHatch.append('\\\\\\')
    totalTimeColors.append('#998ec3')
    totalTimeColors.append('#998ec3')
    totalTimeHatch.append('')
    totalTimeHatch.append('\\\\\\')
    totalTimeHatch.append('')
    totalTimeHatch.append('\\\\\\')

f, (ax, ax2) = plt.subplots(2, 1, sharex=True, gridspec_kw={'height_ratios':[1, 10]})
plt.subplots_adjust(hspace=0.04)
ax.yaxis.set_major_formatter(ticker.FuncFormatter(timeFormatter))
ax2.yaxis.set_major_formatter(ticker.FuncFormatter(timeFormatter))
ax.set_ylim(9.5 * 15.5 * 30 * 16, 9.5 * 18 * 30 * 16)
ax2.set_ylim(0, 9.5 * 9 * 30 * 16)
ax.spines['bottom'].set_visible(False)
ax.xaxis.tick_top()
ax.tick_params(labeltop='off')
ax2.xaxis.tick_bottom()
ax2.spines['top'].set_visible(False)

d = .01  # how big to make the diagonal lines in axes coordinates
mob5break = 0.307
# arguments to pass to plot, just so we don't keep repeating them
kwargs = dict(transform=ax.transAxes, color='k', clip_on=False)
ax.plot((-d, +d), (-10 * d, 10 * d), **kwargs)        # top-left diagonal
ax.plot((1 - d, 1 + d), (-10 * d, 10 * d), **kwargs)  # top-right diagonal
kwargs = dict(transform=ax.transAxes, color='b', clip_on=False)
ax.plot((mob5break - d, mob5break + d), (-10 * d, 10 * d), **kwargs)  # top-right diagonal

kwargs.update(transform=ax2.transAxes, color='k')  # switch to the bottom axes
ax2.plot((-d, +d), (1 - d, 1 + d), **kwargs)  # bottom-left diagonal
ax2.plot((1 - d, 1 + d), (1 - d, 1 + d), **kwargs)  # bottom-right diagonal
kwargs = dict(transform=ax2.transAxes, color='b', clip_on=False)
ax2.plot((mob5break - d, mob5break + d), (1 - d, 1 + d), **kwargs)  # bottom-right diagonal

ind = range(1, 1 + (4 * len(swarmsizesToShow)))
p1 = ax2.bar(ind, contactTimes, 0.8, align='center', color=encounteredColors)
p2 = ax2.bar(ind, totalTimes, 0.8, yerr=totalTimeStd, align='center', bottom=contactTimes, color=totalTimeColors)
p3 = ax2.plot([ i + 0.2 for i in ind], contactTimeMedians, 'ro', color='#ff0000', marker='*')
# plt.boxplot(ft_data, notch=1, labels=labels, showfliers=False)

ax.bar(ind, contactTimes, 0.8, align='center', color=encounteredColors)
ax.bar(ind, totalTimes, 0.8, yerr=totalTimeStd, align='center', bottom=contactTimes, color=totalTimeColors)

# Hatch marks
for bar, pattern in zip(p1, encounteredHatch):
    bar.set_hatch(pattern)

for bar, pattern in zip(p2, totalTimeHatch):
    bar.set_hatch(pattern)

plt.xticks([tic + 0.5 for tic in ind[::2]], labels)
#plt.subplots_adjust(bottom=0.15)

pp = PdfPages('figs/plumeEncountered.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Time (mm:ss)')
ax2.set_yticks([i * 10 * 30 * 16 for i in range(0, 9)])
ax.set_yticks([i * 10 * 30 * 16 for i in range(16, 17)])
plt.xlim(0, 1 + (4 * len(swarmsizesToShow)))

ax.set_title('Initial Plume Contact and Max Flux', size=16)
#plt.legend((p1[0], p2[0], p1[1], p2[1]), ("Initial Plume Contact", "Locate Max Flux", "Pert", "Pert2"))

unperturbedLegend = plt.legend([p1[0], p2[0]], ['Plume Contact', 'Max Flux'], title="Unperturbed", loc=(.63, .88))
medianLegend = plt.legend([p3[0]], ['Max Flux Median'], loc=(.63, .55), numpoints=1, frameon=False, handletextpad=0)
plt.legend([p1[1], p2[1]], ['Plume Contact', 'Max Flux'], title='Perturbed', loc=(.63, 0.64))


plt.gca().add_artist(unperturbedLegend)
plt.gca().add_artist(medianLegend)

pp.savefig()
pp.close()


# Time to initial perterbed plume contact
plt.clf()

swarmsizesToShow2 = [5, 20]
contactTimes = []
totalTimes = []
labels = []
for i in swarmsizesToShow2 :
    contactTimes.append(np.mean([value['plumeEncountered'] for value in ftper_failureTime[i] if 'plumeEncountered' in value]))
    contactTimes.append(np.mean([value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[i] if 'plumeEncountered' in value]))
    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in ftper_failureTime[i] if 'plumeEncountered' in value and 'time' in value and not value['failed']]))
    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[i] if 'plumeEncountered' in value and 'time' in value and not value['failed']]))
    labels.append("LoCUS {}".format(i))
    labels.append("MoBS {}".format(i))


ind = range(0, 2 * len(swarmsizesToShow2))
p1 = plt.bar(ind, contactTimes, 0.75, align='center', color='#f1a340', hatch = "\\")
p2 = plt.bar(ind, totalTimes, 0.75, align='center', bottom=contactTimes, color='#998ec3', hatch = "/")

plt.xticks(ind, labels)

pp = PdfPages('figs/perturbedPlumeEncountered2.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Average Time to Initial Plume Contact and Max Flux')
plt.legend((p1[0], p2[0]), ("Initial Plume Contact", "Locate Max Flux"))
pp.savefig()
pp.close()

# Time to initial perterbed plume contact
plt.clf()

contactTimes = []
totalTimes = []
labels = []
for i in swarmsizesToShow :
    contactTimes.append(np.mean([value['plumeEncountered'] for value in ftper_failureTime[i] if 'plumeEncountered' in value]))
    contactTimes.append(np.mean([value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[i] if 'plumeEncountered' in value]))
    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in ftper_failureTime[i] if 'plumeEncountered' in value and 'time' in value and not value['failed']]))
    totalTimes.append(np.mean([value['time'] - value['plumeEncountered'] for value in baseline_per_swarmsize_failureTime[i] if 'plumeEncountered' in value and 'time' in value and not value['failed']]))
    labels.append("LoCUS {}".format(i))
    labels.append("MoBS {}".format(i))


ind = range(0, 2 * len(swarmsizesToShow))
p1 = plt.bar(ind, contactTimes, 0.75, align='center', color='#f1a340', hatch = "\\")
p2 = plt.bar(ind, totalTimes, 0.75, align='center', bottom=contactTimes, color='#998ec3', hatch = "/")

plt.xticks(ind, labels)

pp = PdfPages('figs/perturbedPlumeEncountered.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Average Time to Initial Plume Contact and Max Flux')
plt.legend((p1[0], p2[0]), ("Initial Plume Contact", "Locate Max Flux"))
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

plt.errorbar(swarmsizes, x, err, fmt='-o', label="LoCUS")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="MoBS")

pp = PdfPages('figs/maxFlux.pdf')
plt.xlabel('Swarm Size')
plt.ylabel('Simulation Timesteps')
plt.title('Time to Max Flux after Plume Contact')
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

plt.errorbar(swarmsizes, x, err, fmt='-o', label="LoCUS")
plt.errorbar(swarmsizes, x2, err2, fmt='-o', label="MoBS")

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
            csvWriter.writerow([20, 3, 3, False, failureCount, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
        for value in plumeFailureTime.get(failureCount):
            csvWriter.writerow([20, 3, 3, False, 0, failureCount, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
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
            csvWriter.writerow([20, False, failureCount, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
        for value in baseline_swarmsize_plumeFailureTime.get(failureCount):
            csvWriter.writerow([20, False, 0, failureCount, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
    for swarmsize in swarmsizes:
        for value in baseline_per_swarmsize_failureTime.get(swarmsize):
            csvWriter.writerow([swarmsize, True, 0, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])
        for value in baseline_swarmsize.get(swarmsize):
            csvWriter.writerow([swarmsize, False, 0, 0, value.get('time'), value.get('plumeEncountered'), value.get('healingTook'), value.get('failed')])


# for swarmsize in swarmsizes:
#     perturbedBySwarmSize = baseline_per_swarmsize_failureTime.get(swarmsize)
#     smoothBySwarmSize = baseline_swarmsize.get(swarmsize)
#     print "Swarmsize: {}".format(swarmsize)
#     for s in samples:
#         perturbed = perturbedBySwarmSize[s]
#         smooth = smoothBySwarmSize[s]
#
#         print "seed: {} perterbed: {} smooth: {}".format(s, perturbed.get('plumeEncountered'), smooth.get('plumeEncountered'))

    # print "Diff: ", [a - b for a, b in zip([p.get('plumeEncountered') for p in perterbedBySwarmSize], [p.get('plumeEncountered') for p in smoothBySwarmSize]) if a is not None and b is not None][12]
    # print "Diff: ", perterbedBySwarmSize[12].get('plumeEncountered'), smoothBySwarmSize[12].get('plumeEncountered')
