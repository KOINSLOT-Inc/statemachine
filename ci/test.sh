# SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

#!/bin/bash

set -euo pipefail

echo "Installing Python dependencies..."
pip install pytest pytest_embedded pytest_embedded_serial_esp pytest_embedded_idf

echo "Building test applications..."
idf.py --project-dir test_app build
SDKCONFIG_DEFAULTS="../../test_app/sdkconfig.defaults" idf.py --project-dir examples/minimal build

echo "Running tests..."
pytest --junitxml=test_app/test_report.xml
