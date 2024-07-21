#!/bin/bash
##===----------------------------------------------------------------------===##
##
## This source file is part of the ScribbleFoundation open source project
##
## Copyright (c) 2024 ScribbleLabApp. and the ScribbleFoundation project authors
## Licensed under Apache License v2.0
##
## See LICENSE.txt for license information
## See CONTRIBUTORS.md for the list of ScribbleFoundation project authors
##
## SPDX-License-Identifier: Apache-2.0
##
##===----------------------------------------------------------------------===##

set -eu
here="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
contributors=$( cd "$here"/.. && git shortlog -es | cut -f2 | sed 's/^/- /' )

cat > "$here/../CONTRIBUTORS.md" <<- EOF
	For the purpose of tracking copyright, this is the list of individuals and
	organizations who have contributed source code to SFFileManagementKit.

	For employees of an organization/company where the copyright of work done
	by employees of that company is held by the company itself, only the company
	needs to be listed here.

	## COPYRIGHT HOLDERS

	- ScribbleLabApp (all contributors with '@scribblelab.com')

	### Contributors

	$contributors

	## Updating this list

	Please do not edit this file manually. It is generated using \`./scripts/generate_contributors_list.sh\`.
    If a name is misspelled or appearing multiple times: add an entry in \`./.mailmap\` (see [docs](https://git-scm.com/docs/git-shortlog#_mapping_authors))
EOF
