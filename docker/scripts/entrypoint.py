from __future__ import print_function
import argparse
import trial_runner

def test(args):
    trial_runner.run_trial(args)

def get_args():
    parser = argparse.ArgumentParser(description='ARGoS Fault Tolerant Drone Simulator')

    parser.add_argument(
        '--rmin',
        type=int,
        default=1)
    parser.add_argument(
        '--rmax',
        type=int,
        default=1)
    parser.add_argument(
        '--coverage_radius',
        type=int,
        default=100)
    parser.add_argument(
        '--failtimestep',
        type=int,
        default=1000)
    parser.add_argument(
        '--swarmsize',
        type=int,
        default=1)
    parser.add_argument(
        '--failures',
        type=int,
        default=0)
    parser.add_argument(
        '--seed',
        type=int,
        default=0)
    args = parser.parse_args()
    return args

def main():
    args = get_args()
    test(args)


if __name__ == '__main__':
    main()
