# TaskMonitor
A Task Manager clone for Linux

# How Do I Compile TaskMonitor?
The dependencies are `gtk4` and `pthread`

Install Dependencies (Arch):
```
sudo pacman -Sy gtk4
```
Download and Compile:
```
git clone https://github.com/gamingdoom/TaskMonitor.git
cd TaskMonitor
make
```
Run:
```
build/taskmonitor
```
Install:
```
sudo ln -s `pwd`/build/taskmonitor /usr/local/bin/taskmonitor
```
