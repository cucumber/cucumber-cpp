If ($env:ruby_version -match "200") {
    $path = "C:\Ruby200\lib\ruby\gems\2.0.0\gems\cucumber-2.0.0\lib\cucumber\formatter\ansicolor.rb"
} Else {
    $path = "C:\Ruby193\lib\ruby\gems\1.9.1\gems\cucumber-2.0.0\lib\cucumber\formatter\ansicolor.rb"
}

(Get-Content $path) -replace '\ST\DERR.puts %', '#nope' | Set-Content $path 
(Get-Content $path) -replace '    Cucumber::', '#nope' | Set-Content $path
(Get-Content $path) -replace '^Cucumber::', '#nope' | Set-Content $path
