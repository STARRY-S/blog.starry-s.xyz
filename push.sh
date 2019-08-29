#!/bin/bash
hikaru clean
hikaru build
git add .
git commit -m "Update."
git push origin master
