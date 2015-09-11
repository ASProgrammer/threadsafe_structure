#!/bin/bash

if [ $1 == "" ]
then
	echo "Use: ./commit.sh <commit-name>"
	exit 0
fi

git add --all
git commit -m "$1"
git push origin master
