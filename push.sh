#!/bin/bash
hikaru clean
hikaru build --debug
git add ./docs/
git commit -m "Updated Site."
git push origin master
git push gitlab master
