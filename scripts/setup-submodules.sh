#!/usr/bin/env bash
set -Eeuo pipefail

echo "======================================"
echo " Kogence IDE - Submodule Setup"
echo "======================================"

# Ensure we're running inside the repository.
if ! git rev-parse --show-toplevel >/dev/null 2>&1; then
    echo "ERROR: Run this script inside the Kogence IDE Git repository."
    exit 1
fi

cd "$(git rev-parse --show-toplevel)"

# Verify that submodules are registered.
if [[ ! -f .gitmodules ]]; then
    echo "ERROR: .gitmodules does not exist."
    echo "Register your submodules before running this script."
    exit 1
fi

echo
echo "Registered submodules:"
git config -f .gitmodules --get-regexp '^submodule\..*\.path$'

echo
echo "Synchronizing submodule URLs..."
git submodule sync --recursive

echo
echo "Initializing and downloading all submodules..."
git submodule update --init --recursive

echo
echo "Verifying submodule status..."
git submodule status --recursive

# A leading '-' means a submodule is not initialized.
# A leading '+' means its checkout differs from the recorded commit.
if git submodule status --recursive | grep -Eq '^[-+]'; then
    echo
    echo "WARNING: Some submodules may not be at their recorded commits."
    echo "Inspect the status above before building."
    exit 1
fi

echo
echo "SUCCESS: All registered submodules are initialized."

