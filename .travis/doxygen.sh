#!/bin/bash

set -e
set -x

REPO_PATH=git@github.com:mgerhardy/simpleai.git
HTML_PATH=simpleai-build/html
GH_PAGES_PATH=simpleai-build/gh-pages
COMMIT_USER="Documentation Builder"
COMMIT_EMAIL="martin.gerhardy@gmail.com"
CHANGESET=$(git rev-parse --verify HEAD)

if [ "$BUILD_TYPE" == "doxygen" ]; then
	mkdir -p simpleai-build
	cd simpleai-build
	cmake ..
	make doc
	cd -
	rm -rf ${GH_PAGES_PATH}
	git clone -b gh-pages "${REPO_PATH}" --single-branch ${GH_PAGES_PATH}
	cd ${GH_PAGES_PATH}
	git rm -rf *
	cd -
	cp ${HTML_PATH}/* ${GH_PAGES_PATH}
	cd ${GH_PAGES_PATH}
	git add .
	git config user.name "${COMMIT_USER}"
	git config user.email "${COMMIT_EMAIL}"
	git commit -m "Automated documentation build for changeset ${CHANGESET}."
	git push origin gh-pages
	cd -
fi
