Before do
    FileUtils.rm_rf test_features_dir if File.directory?(test_features_dir)
end

Given /^a feature file with:$/ do |contents|
    write_file sample_feature_name, contents
end

Given /^a step definition file with:$/ do |string|
    contents = <<-FILE
#include <cucumber-cpp/defs.hpp>

#{string}
FILE
    write_file cpp_steps_src, contents
end

When /^I run cucumber\-cpp$/ do
    compile_steps
    create_wire_file
    run_cucumber_cpp
    run_cucumber_test_features
end

Then /^the output should contain:$/ do |string|
    expect(@steps_out.gets).to eq(string)
end


def sample_feature_name
    "#{test_features_dir}/sample.feature"
end

def compile_steps
    compiler_output = %x[ #{compile_cpp_steps_command} ]
    expect($?.success?).to eq(true)
end

def create_wire_file
    contents = <<-WIRE
host: localhost
port: 3902
    WIRE
    filename = "#{test_features_dir}/step_definitions/cucumber-cpp.wire"
    write_file filename, contents
end

def test_features_dir
    ENV["TEST_FEATURES_DIR"]
end

def cpp_steps_src
    ENV["DYNAMIC_CPP_STEPS_SRC"]
end

def cpp_steps_exe
    ENV["DYNAMIC_CPP_STEPS_EXE"]
end

def compile_cpp_steps_command
    ENV["COMPILE_DYNAMIC_CPP_STEPS"]
end

def run_cucumber_cpp
    @steps_out = IO.popen cpp_steps_exe, 'r+'
end

def run_cucumber_test_features
    @cucumber_out = `cucumber #{test_features_dir}`
end

