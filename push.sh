#!/bin/bash
hikaru clean
hikaru build --debug
git add .
git commit -m "Update."
git push origin master
git push gitlab master
