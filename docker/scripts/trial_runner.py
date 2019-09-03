import os, argparse, datetime, random, time, subprocess, tempfile
import xml.etree.ElementTree as ET

def run_trial(args):
    # Copy example XML file to temp 
    # os.system('cp /neatfa/argos/experiments/1-C.xml /neatfa/argos/experiments/iAnt_temp.xml')

    # Read and parse XML, extract needed experiment constants
    tree = ET.parse('/neatfa/argos/experiments/dockerized.xml')
    root = tree.getroot()
    sim_params = root.find('loop_functions').find('simulation')

    # Set args in XML foe and rewrite
    for arg in vars(args):
        sim_params.set(arg, str(getattr(args, arg)))
    with tempfile.NamedTemporaryFile(dir="/neatfa/argos/experiments/",  suffix=".xml", delete=False) as configuration_file:

        tree.write(configuration_file)
        configuration_file.flush()
        # Exectute Argos Experiment and write output to temp file
        output = subprocess.check_output(["argos3", "-c", configuration_file.name]).splitlines() 
        score = 0
        for line in output:  
            if str('Fitness') in line:
                score = float(line.split(':')[1].strip())
                break

        print(score)
    return(score)
