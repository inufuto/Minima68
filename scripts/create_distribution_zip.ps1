param(
    [string]$OutputDir = "$(Join-Path $PSScriptRoot '..\dist')",
    [string]$ZipName = "Minima68-distribution-$(Get-Date -Format 'yyyyMMdd-HHmmss').zip",
    [switch]$SkipBuild
)

$ErrorActionPreference = 'Stop'

function Invoke-CMake {
    param(
        [Parameter(Mandatory = $true)][string[]]$Arguments,
        [Parameter(Mandatory = $true)][string]$Label
    )

    Write-Host "==> $Label"
    & cmake @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "cmake failed: $Label"
    }
}

function Find-NewestFile {
    param(
        [Parameter(Mandatory = $true)][string]$Root,
        [Parameter(Mandatory = $true)][string]$Filter,
        [Parameter(Mandatory = $true)][string]$Description
    )

    if (-not (Test-Path -LiteralPath $Root)) {
        throw "Directory not found for ${Description}: $Root"
    }

    $files = Get-ChildItem -Path $Root -Recurse -File -Filter $Filter
    if (-not $files) {
        throw "No file found for $Description ($Filter) under $Root"
    }

    return $files | Sort-Object LastWriteTimeUtc -Descending | Select-Object -First 1
}

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
Set-Location $repoRoot

if (-not $SkipBuild) {
    Invoke-CMake -Arguments @('--preset', 'pico-release') -Label 'Configure pico-release'
    Invoke-CMake -Arguments @('--build', '--preset', 'pico-release') -Label 'Build pico-release'

    Invoke-CMake -Arguments @('--preset', 'pico2-release') -Label 'Configure pico2-release'
    Invoke-CMake -Arguments @('--build', '--preset', 'pico2-release') -Label 'Build pico2-release'

    Invoke-CMake -Arguments @('--preset', 'windows-x64-release') -Label 'Configure windows-x64-release'
    Invoke-CMake -Arguments @('--build', '--preset', 'windows-x64-release') -Label 'Build windows-x64-release'
}

$picoUf2 = Find-NewestFile -Root (Join-Path $repoRoot 'build/pico-release') -Filter '*.uf2' -Description 'Pico UF2'
$pico2Uf2 = Find-NewestFile -Root (Join-Path $repoRoot 'build/pico2-release') -Filter '*.uf2' -Description 'Pico2 UF2'
$windowsExe = Find-NewestFile -Root (Join-Path $repoRoot 'out/build/windows-x64-release') -Filter 'Minima68.exe' -Description 'Windows EXE'

$stagingDir = Join-Path $env:TEMP ("minima68-dist-" + [Guid]::NewGuid().ToString('N'))

try {
    New-Item -ItemType Directory -Path $stagingDir | Out-Null
    New-Item -ItemType Directory -Path (Join-Path $stagingDir 'pico') | Out-Null
    New-Item -ItemType Directory -Path (Join-Path $stagingDir 'pico2') | Out-Null
    New-Item -ItemType Directory -Path (Join-Path $stagingDir 'windows') | Out-Null

    Copy-Item -LiteralPath $picoUf2.FullName -Destination (Join-Path $stagingDir 'pico')
    Copy-Item -LiteralPath $pico2Uf2.FullName -Destination (Join-Path $stagingDir 'pico2')
    Copy-Item -LiteralPath $windowsExe.FullName -Destination (Join-Path $stagingDir 'windows')

    New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

    $zipPath = Join-Path $OutputDir $ZipName
    if (Test-Path -LiteralPath $zipPath) {
        Remove-Item -LiteralPath $zipPath -Force
    }

    Compress-Archive -Path (Join-Path $stagingDir '*') -DestinationPath $zipPath -CompressionLevel Optimal

    Write-Host ''
    Write-Host 'Distribution ZIP created:'
    Write-Host $zipPath
    Write-Host ''
    Write-Host 'Included files:'
    Write-Host ("- pico:    " + (Split-Path -Leaf $picoUf2.FullName))
    Write-Host ("- pico2:   " + (Split-Path -Leaf $pico2Uf2.FullName))
    Write-Host ("- windows: " + (Split-Path -Leaf $windowsExe.FullName))
}
finally {
    if (Test-Path -LiteralPath $stagingDir) {
        Remove-Item -LiteralPath $stagingDir -Recurse -Force
    }
}
