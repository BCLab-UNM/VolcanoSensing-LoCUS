from __future__ import print_function
import argparse
import trial_runner

def test(args):
    trial_runner.run_trial(args)

def get_args():
    parser = argparse.ArgumentParser(description='ARGoS Fault Tolerant Drone Simulator')
    parser.add_argument(
        '--DisableNestSight',
        type=int,
        default=0)
    parser.add_argument(
        '--DisableCompass',
        type=int,
        default=0)
    parser.add_argument(
        '--DisableHoldingFood',
        type=int,
        default=0)
    parser.add_argument(
        '--DisableNearFood',
        type=int,
        default=0)
    parser.add_argument(
        '--DisableRobotProxmity',
        type=int,
        default=0)
    parser.add_argument(
        '--DisablePheromone',
        type=int,
        default=0)
    parser.add_argument(
        '--DisableNestLight',
        type=int,
        default=0)
    parser.add_argument(
        '--FoodDistribution',
        type=int,
        default=0)
    args = parser.parse_args()
    return args

def main():
    args = get_args()
    test(args)


if __name__ == '__main__':
    main()
