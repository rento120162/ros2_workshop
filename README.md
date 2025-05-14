# ROS2_workshop
This is a repository for ROS2 workshop

## How to use github
```bash
#If you connect local repository to remote one, execute below on your ros2 container.
#You should do this only once when your first time
git config --global init.defaultBranch main
git init
git config --global user.name "user_name"
git config --global user.email "user_email"
#You make sure to match "user_name","user_emal" and github account

git remote add origin "URL"
#make sure to contain your repository's URL to "URL" 
git pull origin main
#you are required your name and access tokenkey

git branch
#then you could see your branch "main"

git add .
#to commit all files, if not you can choose your own file
git status
#you could check staging area (you can check files to be commited)

git commit -m"comment"
#put description of chenge to "comment" 

git push origin main
#push to main branch if you want to change target branch, chenge it
#you are required your name and access tokenkey
```
```bash
#If you want to make another branch, you should execute below
git branch "branch name"
#you can put any branch name on "branch name" like "sub1"

git checkout "branch name"

git merge main
#to merge "branch name" to main
#you should execute this after you check the programs work correctly

```
