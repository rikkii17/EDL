param([string]$Compiler = "C:/Program Files/LLVM/bin/clang++.exe")
$ErrorActionPreference = "Stop"
$project = (Resolve-Path (Join-Path $PSScriptRoot "../..")).Path
$output = Join-Path $project ".pio/contract_test.exe"
New-Item -ItemType Directory -Force (Join-Path $project ".pio") | Out-Null
& $Compiler -std=c++17 -Wall -Wextra -Werror `
    -I "$PSScriptRoot/mocks" -I "$project/lib/telemetry/include" -I "$project/lib/helper/include" `
    -I "$project/lib/hardware/include" -I "$project/lib/config" `
    "$PSScriptRoot/contract_test.cpp" "$project/lib/telemetry/src/measurement_json.cpp" `
    "$project/lib/helper/src/conversion_data.cpp" "$PSScriptRoot/sensor_test.cpp" `
    "$project/lib/hardware/src/ens160sensor.cpp" -o $output
if ($LASTEXITCODE -ne 0) { throw "Native contract compilation failed" }
$fixtures = & $output
if ($LASTEXITCODE -ne 0) { throw "Native contract tests failed" }
$fixtures | Set-Content (Join-Path $project ".pio/firmware_fixture.jsonl") -Encoding ascii
Write-Output "Fixtures: $project/.pio/firmware_fixture.jsonl"
