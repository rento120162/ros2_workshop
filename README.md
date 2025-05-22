# ros2_workshop
This is a repository for ros2 workshop
## How to use Docker
At your local terminal 
```bash
sudo apt-get install git
sudo snap install docker
```
check whether docker work correctly
```bash
docker ps
```
If you could see like this, you should add user to docker group to give user permisstion 
```bash
permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock:
```
```bash
sudo chmod 771 /var/run/docker.sock
sudo groupadd docker
sudo usermod -aG docker $USER
```
## How to start ROS2
https://github.com/hrjp/rosenv.git

## How to use remote repository

If you connect local repository to remote one, execute below on your ros2 container.
```bash
cd /home/colcon_ws
touch .gitignore
vi .gitignore
```
on the .gitignore
```bash
build
install
log
```
If there are files(or folder) you want to ignore, put it on .gitignore.
You should do this only once when your first time
```bash
git config --global init.defaultBranch main
git config --global user.name "user_name"
git config --global user.email "user_email"
git init
#You make sure to match "user_name","user_emal" and github account
```
```bash
git remote add origin "URL"
#make sure to contain your repository's URL to "URL" 
git pull origin main
#you are required your name and access tokenkey
```
```bash
git branch
#then you could see your branch "main"
```
```bash
git add .
#to commit all files, if not you can choose your own file
git status
#you could check staging area (you can check files to be commited)
git commit -m"comment"
#put description of chenge to "comment" 
```
```bash
git push origin main
#push to main branch if you want to change target branch, chenge it
#you are required your name and access tokenkey
```


## How to use micro-ROS-PlatformIo
In your ROS2 container 
```bash
sudo apt update && sudo apt upgrade
sudo apt install python3-venv
```
https://github.com/micro-ROS/micro_ros_platformio.git


## How to use Ultralytics-ros
Check whether connect USBcamera correctly at you local terminal
```bash
sudo apt update && sudo apt upgrade
sudo apt install v4l-utils
v4l2-ctl --list-devices
```
If you want to use original inference model, add your model to ROS container.
note: before build, you could not find this directory
```bash
/home/colcon_ws/install/ultralytics-ros/share/ultralytics-ros/models/
```
