param(
    [Parameter(Mandatory=$false)]
    [string]$Target
)

function Show-Help {
    Write-Host "Usage: .\run-build-win.ps1 [-Target <FunctionName>]"
    Write-Host ""
    Write-Host "  -Target <FunctionName> : A build function defined in build.ps1."
    Write-Host "                           e.g., vc-x64-v4, vc-pgo-x64, clang-x64, etc."
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  1) .\run-build-win.ps1 --help"
    Write-Host "     -> Displays this help and exits."
    Write-Host "  2) .\run-build-win.ps1 -Target vc-x64"
    Write-Host "     -> Creates 'bin' directory if not present, navigates to 'src',"
    Write-Host "        imports build.ps1, and executes the vc-x64 function."
    Write-Host ""
    Write-Host "Available build functions (based on the sample makefile):"
    Write-Host "  vc-x64-v4 : MSVC (x64) with SSE2/AVX/SSE4.2/popcount/AVX2/AVX512 support"
    Write-Host "  vc-x64-v3 : MSVC (x64) with SSE2/AVX/SSE4.2/popcount/AVX2 support"
    Write-Host "  vc-x64-v2 : MSVC (x64) with SSE2/AVX/SSE4.2/popcount support"
    Write-Host "  vc-x64    : MSVC (x64) with SSE2 support"
    Write-Host "  vc-a64    : MSVC (ARM64)"
    Write-Host ""
    Write-Host "  clang-x64-v4 : clang-cl (x64) with SSE2/AVX/SSE4.2/popcount/AVX2/AVX512 support"
    Write-Host "  clang-x64-v3 : clang-cl (x64) with SSE2/AVX/SSE4.2/popcount/AVX2 support"
    Write-Host "  clang-x64-v2 : clang-cl (x64) with SSE2/AVX/SSE4.2/popcount support"
    Write-Host "  clang-x64    : clang-cl (x64) with SSE2 support"
    Write-Host "  clang-a64    : clang-cl (ARM64)"
    Write-Host ""
    Write-Host "  vc-pgo-x64-v3: MSVC (x64) PGO with SSE2/AVX/SSE4.2/popcount/AVX2 support"
    Write-Host "  vc-pgo-x64   : MSVC (x64) PGO with SSE2/AVX/SSE4.2/popcount support"
    Write-Host ""
    Write-Host "Notes:"
    Write-Host "  - build.ps1 is located in the 'src' directory and defines the build functions above."
    Write-Host "  - If the specified target function does not exist in build.ps1, an error is shown."
}

if ($Target -eq "--help" -or $Target -eq "-help" -or $Target -eq "help") {
    Show-Help
    exit 0
}

if (-not $Target) {
    Write-Host "No target specified."
    Show-Help
    exit 1
}

if (-Not (Test-Path -Path "./bin")) {
    Write-Host "Creating 'bin' directory..."
    New-Item -ItemType Directory -Path "./bin" | Out-Null
} else {
    Write-Host "'bin' directory already exists."
}

if (-Not (Test-Path -Path "./src")) {
    Write-Error "Error: 'src' directory not found."
    exit 1
} else {
    Write-Host "Navigating to 'src' directory..."
    Set-Location "./src"
}

if (-Not (Test-Path -Path "./build.ps1")) {
    Write-Error "Error: 'build.ps1' not found in 'src'."
    exit 1
}

Write-Host "Importing 'build.ps1'..."
. .\build.ps1

if (Get-Command $Target -ErrorAction SilentlyContinue) {
    Write-Host "Executing function '$Target'..."
    & $Target
} else {
    Write-Error "Error: Function '$Target' not found in 'build.ps1'."
    exit 1
}

Write-Host "Returning to root directory..."
Set-Location ".."

Write-Host "Build process completed."

