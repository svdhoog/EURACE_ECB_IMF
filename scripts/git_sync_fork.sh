#!/bin/bash

#Script to synchronize this fork with the upstream repository
#Ref: https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/syncing-a-fork


#Fetch the branches and their respective commits from the upstream repository. 
#Commits to master will be stored in a local branch, upstream/master.
git fetch upstream

#Check out your fork's local master branch.
git checkout master

#Merge the changes from upstream/master into your local master branch.
#This brings your fork's master branch into sync with the upstream repository, without losing your local changes.
git merge upstream/master
