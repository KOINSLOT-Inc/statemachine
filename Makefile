# SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: lint
lint:
	docker run --rm --volume $$PWD:/data fsfe/reuse lint

.PHONY: test
test:
	docker run --rm -v $$PWD:/statemachine -w /statemachine espressif/idf:release-v5.1 bash ci/test.sh
