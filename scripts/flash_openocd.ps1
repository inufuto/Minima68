param(
    [Parameter(Mandatory = $true)]
    [string]$ElfPath,
    [string]$TargetConfig = "target/rp2040.cfg",
    [string]$InterfaceConfig = "interface/cmsis-dap.cfg",
    [int]$AdapterSpeed = 5000,
    [int]$MaxRetries = 5,
    [int]$RetryDelaySeconds = 1
)

$openocd = Join-Path $env:USERPROFILE ".pico-sdk/openocd/0.12.0+dev/openocd.exe"
$scriptsDir = Join-Path $env:USERPROFILE ".pico-sdk/openocd/0.12.0+dev/scripts"

if (-not (Test-Path $openocd)) {
    Write-Error "OpenOCD was not found at: $openocd"
    exit 1
}

# Release probe handle if a stale debug server is still running.
Get-Process -Name openocd -ErrorAction SilentlyContinue | Stop-Process -Force

for ($attempt = 1; $attempt -le $MaxRetries; $attempt++) {
    Write-Host "OpenOCD flash attempt $attempt/$MaxRetries"

    & $openocd `
        -s $scriptsDir `
        -f $InterfaceConfig `
        -f $TargetConfig `
        -c "adapter speed $AdapterSpeed" `
        -c "program $ElfPath verify" `
        -c "reset" `
        -c "exit"

    if ($LASTEXITCODE -eq 0) {
        Write-Host "Flash completed successfully."
        exit 0
    }

    if ($attempt -lt $MaxRetries) {
        Write-Warning "Flash failed (exit code $LASTEXITCODE). Retrying in $RetryDelaySeconds second(s)..."
        Start-Sleep -Seconds $RetryDelaySeconds
    }
}

Write-Error "Flash failed after $MaxRetries attempts. Check USB cable, probe power, and CMSIS-DAP driver status."
exit 1
