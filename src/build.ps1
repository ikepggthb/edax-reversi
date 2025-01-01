# Powershell script for building and cleaning

# Compilation options
$VC_FLAGS = "/std:c17 /DUNICODE /utf-8 /D_CRT_SECURE_NO_DEPRECATE /I..\include /O2 /fp:fast /GS- /D NDEBUG /MT"

function vc-x64-v4 {
    Write-Host "Building vc-x64-v4..."
    cl $VC_FLAGS /arch:AVX512 /experimental:c11atomics /GL /vlen=256 /D__POPCNT__ /D__CRC32__ all.c ws2_32.lib /Fe..\bin\wEdax-x64-v4.exe /link /VERSION:4.6
}

function vc-x64-v3 {
    Write-Host "Building vc-x64-v3..."
    cl $VC_FLAGS /arch:AVX2 /experimental:c11atomics /GL /D__POPCNT__ /D__CRC32__ all.c ws2_32.lib /Fe..\bin\wEdax-x64-v3.exe /link /VERSION:4.6
}

function vc-x64-v2 {
    Write-Host "Building vc-x64-v2..."
    cl $VC_FLAGS /experimental:c11atomics /GL /D__SSE2__ /D__POPCNT__ /D__CRC32__ all.c ws2_32.lib /Fe..\bin\wEdax-x64-v2.exe /link /VERSION:4.6
}

function vc-x64 {
    Write-Host "Building vc-x64..."
    cl $VC_FLAGS /experimental:c11atomics /GL /D__SSE2__ all.c ws2_32.lib /Fe..\bin\wEdax-x64.exe /link /VERSION:4.6
}

function vc-a64 {
    Write-Host "Building vc-a64..."
    cl $VC_FLAGS /experimental:c11atomics /GL /D__ARM_NEON all.c ws2_32.lib /Fe..\bin\wEdax-a64.exe /link /VERSION:4.6
}

function clang-x64-v4 {
    Write-Host "Building clang-x64-v4..."
    clang-cl -mprefer-vector-width=256 $VC_FLAGS /U__STDC_NO_THREADS__ /arch:AVX512 all.c ws2_32.lib /Fe..\bin\wEdax-x64-v4.exe /link /VERSION:4.6
}

function clang-x64-v3 {
    Write-Host "Building clang-x64-v3..."
    clang-cl $VC_FLAGS /U__STDC_NO_THREADS__ /arch:AVX2 all.c ws2_32.lib /Fe..\bin\wEdax-x64-v3.exe /link /VERSION:4.6
}

function clang-x64-v2 {
    Write-Host "Building clang-x64-v2..."
    clang-cl -march=x86-64-v2 $VC_FLAGS /U__STDC_NO_THREADS__ all.c ws2_32.lib /Fe..\bin\wEdax-x64-v2.exe /link /VERSION:4.6
}

function clang-x64 {
    Write-Host "Building clang-x64..."
    clang-cl $VC_FLAGS /U__STDC_NO_THREADS__ all.c ws2_32.lib /Fe..\bin\wEdax-x64.exe /link /VERSION:4.6
}

function clang-a64 {
    Write-Host "Building clang-a64..."
    clang-cl --target=aarch64-win32-msvc $VC_FLAGS /U__STDC_NO_THREADS__ /D__ARM_NEON all.c ws2_32.lib /Fe..\bin\wEdax-a64.exe /link /VERSION:4.6
}

function vc-pgo-x64-v3 {
    Write-Host "Building vc-pgo-x64-v3 with PGO..."
    $env:VCPROFILE_PATH = "..\src"
    cl $VC_FLAGS /arch:AVX2 /experimental:c11atomics /GL /D__POPCNT__ /D__CRC32__ all.c ws2_32.lib /Fe..\bin\wEdax-x64-v3.exe /link /ltcg:pgi /VERSION:4.6
    Push-Location ..\bin
    .\wEdax-x64-v3.exe -l 60 -solve ..\problem\fforum-20-39.obf
    .\wEdax-x64-v3.exe -l 18 -auto-store on -auto-start on -repeat 2 -auto-quit on -mode 2 -book-file book.pgo
    Remove-Item book.pgo book.pgo.store
    Pop-Location
    link all.obj ws2_32.lib /out:..\bin\wEdax-x64-v3.exe /ltcg:pgo /VERSION:4.6
    Remove-Item *.pgc ..\bin\*.pgd
}

function vc-pgo-x64 {
    Write-Host "Building vc-pgo-x64 with PGO..."
    $env:VCPROFILE_PATH = "..\src"
    cl $VC_FLAGS /experimental:c11atomics /GL /D__POPCNT__ /D__CRC32__ all.c ws2_32.lib /Fe..\bin\wEdax-x64.exe /link /ltcg:pgi /VERSION:4.6
    Push-Location ..\bin
    .\wEdax-x64.exe -l 60 -solve ..\problem\fforum-20-39.obf
    .\wEdax-x64.exe -l 18 -auto-store on -auto-start on -repeat 2 -auto-quit on -mode 2 -book-file book.pgo
    Remove-Item book.pgo book.pgo.store
    Pop-Location
    link all.obj ws2_32.lib /out:..\bin\wEdax-x64.exe /ltcg:pgo /VERSION:4.6
    Remove-Item *.pgc ..\bin\*.pgd
}

function clean {
    Write-Host "Cleaning build files..."
    Remove-Item -Force pgopti* *.dyn all.gc* *~ *.p* *.obj
}

