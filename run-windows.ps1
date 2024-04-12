# --- Function definitions ---
function Invoke-CMake {
    param (
        $Arguments
    )
    $process = Start-Process "cmake" -ArgumentList "$Arguments" -NoNewWindow -Wait -PassThru
    If ($process.ExitCode -ne 0) {
        Write-Host "Abort script execution due to CMake error."
        Exit(42)
    }
}

function Get-VariableOrEnv {
    param (
        [string]$Name
    )

    $envValue = [Environment]::GetEnvironmentVariable($Name)
    if ($envValue -ne $null) {
        return $envValue
    }

    if (Get-Variable -Name $Name -ErrorAction SilentlyContinue) {
        return (Get-Variable -Name $Name).Value
    }

    return $null
}

# --- Variable definitions ---
$cpp_standard = Get-VariableOrEnv -Name "cpp_standard"

$nlohmann_json_DIR = Get-VariableOrEnv -Name "nlohmann_json_DIR"
$Asio_ROOT = Get-VariableOrEnv -Name "Asio_ROOT"
$TCLAP_ROOT = Get-VariableOrEnv -Name "TCLAP_ROOT"
$Env:BOOST_ROOT = "boost_${Env:BOOST_VERSION}".Replace(".","_")

$Env:CTEST_OUTPUT_ON_FAILURE="ON"


# --- Script ---

Invoke-CMake "-E","make_directory","build"

$cmake_params = "-E chdir build cmake",
    "-DBUILD_SHARED_LIBS=`"${BUILD_SHARED_LIBS}`"",
    "-DCMAKE_INSTALL_PREFIX=${HOME}/.local"

$cmake_params += "-DCMAKE_CXX_STANDARD=${cpp_standard}"

$cmake_params += "-DCUKE_ENABLE_BOOST_TEST=OFF"
$cmake_params += "-DCUKE_ENABLE_GTEST=ON"
$cmake_params += "-DCUKE_ENABLE_QT_6=OFF"
$cmake_params += "-DCUKE_ENABLE_QT_5=ON"
$cmake_params += "-DCUKE_ENABLE_EXAMPLES=ON"
$cmake_params += "-DCUKE_TESTS_UNIT=ON"
$cmake_params += "-DCUKE_CODE_COVERAGE=OFF"

$cmake_params += "-Dnlohmann_json_DIR=${nlohmann_json_DIR}"
$cmake_params += "-DAsio_ROOT=${Asio_ROOT}"
$cmake_params += "-DTCLAP_ROOT=${TCLAP_ROOT}"

$cmake_params += ".."

Invoke-CMake "$cmake_params"
Invoke-CMake "--build","build","--config","Release" #,"--parallel"
Invoke-CMake "--build","build","--config","Release","--target","RUN_TESTS"

#
# Execute Calc examples
#

$CalcTests = @("build\examples\Calc\Release\GTestCalculatorSteps.exe",
"build\examples\Calc\Release\BoostCalculatorSteps.exe",
"build\examples\Calc\Release\FuncArgsCalculatorSteps.exe")

If ((Get-Command "qmake.exe" -ErrorAction SilentlyContinue) -ne $null) 
{ 
   $CalcTests += "build\examples\Calc\Release\QtTestCalculatorSteps.exe"
}

For ($i=0; $i -lt $CalcTests.Length; $i++) {
    If (Test-Path -path $CalcTests[$i] -PathType Leaf) {
        Write-Host "Start Test $($CalcTests[$i])"  -f Green
        Start-Process -NoNewWindow $CalcTests[$i]
        Start-Sleep -Seconds 1.0
        Set-Location -Path 'examples/Calc'
        Start-Process cucumber -NoNewWindow -Wait
        set-Location -Path $PSScriptRoot
    } Else {
        Write-Host "Skipping $($CalcTests[$i]): file not exisiting"  -f Yellow
    }    
}
#
# Execute QtCalc examples
# 

If ((Get-Command "qmake.exe" -ErrorAction SilentlyContinue) -eq $null) 
{ 
   Write-Host "Qt not found in PATH, skipping QtCalc Tests" -f Yellow
}
Else
{
    $QtCalcTests = @("build\examples\CalcQt\Release\GTestCalculatorQtSteps.exe",
    "build\examples\CalcQt\Release\QtTestCalculatorQtSteps.exe",
    "build\examples\CalcQt\Release\BoostCalculatorQtSteps.exe")

    For ($i=0; $i -lt $QtCalcTests.Length; $i++) {
        If (Test-Path -path $QtCalcTests[$i] -PathType Leaf) {
            Write-Host "Start Test $($QtCalcTests[$i])"  -f Green
            Start-Process -NoNewWindow $QtCalcTests[$i]
            Start-Sleep -Seconds 1.0
            Set-Location -Path 'examples/CalcQt'
            Start-Process cucumber -NoNewWindow -Wait
            set-Location -Path $PSScriptRoot
        } Else {
            Write-Host "Skipping $($QtCalcTests[$i]): file not exisiting"  -f Yellow
        }    
    }
}

Invoke-CMake "--build","build","--config","Release","--target","INSTALL"


