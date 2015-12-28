#!/bin/bash
set -e # exit with nonzero exit code if anything fails

# clear and re-create the out directory
rm -rf build || exit 0;
mkdir build;

# run our compile script, discussed above
(cd docs; qdoc quickflux.qdocconf)

REPO=$(git config remote.origin.url)

# go to the out directory and create a *new* Git repo
cd build 
cp -a ../docs/html/* .
git init

# The first and only commit to this new Git repo contains all the
# files present with the commit message "Deploy to GitHub Pages".
git add .
git commit --author="Ben Lau <xbenlau@gmail.com>"  -m "Deploy to GitHub Pages"

# Force push from the current repo's master branch to the remote
# repo's gh-pages branch. (All previous history on the gh-pages branch
# will be lost, since we are overwriting it.) We redirect any output to
# /dev/null to hide any sensitive credential data that might otherwise be exposed.
git push --force $REPO master:gh-pages 
