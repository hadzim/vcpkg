# TBS VCPKG

This is a fork of [Microsoft/vcpkg](https://github.com/microsoft/vcpkg) with some additional changes for our internal use.

# Version

Based on vcpkg 2023.04.15 release (updated 25.04.2023).

# Manifest

Used libraries and their versions are defined in `vcpkg.json` file.

# How to Update this Custom Repository

With locally cloned repository you can update the local vcpkg fork to upstream with `git` as follows:
```sh
# Clone locally
git clone git@github.com:hadzim/vcpkg.git
cd vcpkg
git remote -v
# Specify a remote upstream repo to sync with your fork
git remote add upstream https://github.com/microsoft/vcpkg.git
# Verify
git remote -v
# Fetch branches and commits from the upstream repo.
# You'll be storing the commits to master in a local branch upstream/master
git fetch upstream
# Checkout your fork's local master, then merge changes from upstream/master into it
git checkout master

# ideally merge to some release tag (see https://github.com/microsoft/vcpkg/releases):
git merge 2023.04.15
# or to the current upstream master
git merge upstream/master

# Push changes
git push
```

poco-cipher needs to be updated as well
- copy in above clone poco to poco-cipher folder
- open portfile.cmake and copy REF hash (9d1c428c861f2e5ccf09149bbe8d2149720c5896)
- clone poco repository with that hash into dedicated directory and checkout specific REF HASH
```sh
git clone git@github.com:pocoproject/poco.git
cd poco
git checkout 9d1c428c861f2e5ccf09149bbe8d2149720c5896
## change poco clone
git add Data/SQLiteCipher/
git diff --cached > pocosqlitecipher.patch
## copy patch to vcpkg patches and change portfile.cmake
```

then change poco library

# How to Update Local VCPKG Toolchain
```sh
cd vcpkg
git pull --rebase
bootstrap-vcpkg.bat
vcpkg update # see list of packages to update
vckpg upgrade # see list of upgradable packages
vcpkg upgrade --no-dry-run # upgrade packages
copy-external.bat
```
If `vcpkg upgrade --no-dry-run` fails, then use these commands instead to update:
```sh
vcpkg remove --outdated --recurse # delete outdated packages
# Install dependencies (vcpkg.json defines the versions):
vcpkg install --triplet x86-windows
copy-external.bat
```
