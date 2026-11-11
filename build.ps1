# Builds PipelineApp.exe with the MSVC compiler (Visual Studio).
# Usage (from the project folder): .\build.ps1
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vsPath) { throw 'Visual Studio with C++ tools not found.' }

$env:PATH = "${env:ProgramFiles(x86)}Microsoft Visual StudioInstaller;$env:PATH"
# Load the MSVC environment (cl.exe, include and lib paths) into this PowerShell session.
Import-Module "$vsPath\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath $vsPath -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64' | Out-Null

New-Item -ItemType Directory -Force build | Out-Null
cl /nologo /std:c++17 /EHsc /W4 /Zi /utf-8 /Fo:build\ /Fd:build\ /Fe:PipelineApp.exe *.cpp /link /INCREMENTAL:NO
if ($LASTEXITCODE -ne 0) { throw 'Build failed.' }
