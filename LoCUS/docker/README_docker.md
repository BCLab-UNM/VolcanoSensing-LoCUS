# docker-neatfa-combined
This repo has the source code for the neat sim embedded into it, to allow for easier debugging of the c++ code while running in the docker image.

When you change any c++ code, or make changes to the xml, rebuilding the image will automatically detect changes in the c++ source files and recompile. 
## clone this repo and run from directory root:
```
docker build -t <your-image-name>:<your-image-tag> -f docker/Dockerfile .
```
## to run the built image, run:
```
docker run <your-image-name>:<your-image-tag> [--optional-args]
```
## to automatically remove container after exectution (recommended), run: 
```
docker run --rm <your-image-name>:<your-image-tag> [--optional-args]
```
## each time you made a change to any source code, you need to rebuild the image.

params are listed in argos_neatfa_runner.py and can be optionally passed in as command args to the docker run command

to remove accumulating containers as you test and run, run this docker command to remove ALL containers:
```
docker rm $(docker ps -aq)
````

if you want to totally wipe your built image and re-build fresh, run:
```
docker rmi <your-image-name>:<your-image-tag>
```
