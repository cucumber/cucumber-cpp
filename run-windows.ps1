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
$cmake_params += "-DCUKE_ENABLE_GTEST=OFF"
$cmake_params += "-DCUKE_ENABLE_QT_6=OFF"
$cmake_params += "-DCUKE_ENABLE_EXAMPLES=OFF"
$cmake_params += "-DCUKE_TESTS_UNIT=OFF"
$cmake_params += "-DCUKE_CODE_COVERAGE=OFF"

$cmake_params += "-Dnlohmann_json_DIR=${nlohmann_json_DIR}"
$cmake_params += "-DAsio_ROOT=${Asio_ROOT}"
$cmake_params += "-DTCLAP_ROOT=${TCLAP_ROOT}"

$cmake_params += ".."


Invoke-CMake "$cmake_params"
Invoke-CMake "--build","build" #,"--parallel"
