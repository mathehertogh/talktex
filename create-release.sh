#!/bin/bash

PACKAGE_FILENAME_BASE="talktex"
PACKAGE_DIR_PATH="release"
RELEASE_PATHS_PATH="release-paths.txt"

####################################################################################################

set -e # Quit on error
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null

####################################################################################################

working_dir=$(mktemp -d -t talktex-release-XXXXXXXXXX)
package_dir="$working_dir/$PACKAGE_FILENAME_BASE"
mkdir "$package_dir"

while IFS="" read -r release_path || [ -n "$release_path" ]
do
	for path in $(find "$release_path" -type f -o -type l); do
		if ! $(git check-ignore -q $path); then
			mkdir -p "$package_dir/$(dirname "$path")"
			cp --no-dereference --no-target-directory "$path" "$package_dir/$path"
		fi
	done
done < "$RELEASE_PATHS_PATH"

mkdir -p "$PACKAGE_DIR_PATH"
package_path="$PACKAGE_DIR_PATH/$PACKAGE_FILENAME_BASE.tar.gz"
tar -czf "$package_path" --directory="$working_dir" "$PACKAGE_FILENAME_BASE"

rm -rf "$working_dir"

echo "$(tput bold; tput setaf 2)Succesfully created release package at $(tput smul)${package_path}$(tput sgr0)"
