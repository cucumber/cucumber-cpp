require 'json'
require 'os'

module CucumberCppMappings

  def features_dir
    FEATURES_DIR
  end

  def write_passing_mapping(step_name)
    append_step_definition(step_name, '// no-op, pass gently')
  end

  def write_failing_mapping(step_name)
    write_failing_mapping_with_message(step_name, "rotten cucumbers")
  end

  def write_failing_mapping_with_message(step_name, message)
    append_step_definition(step_name, "throw \"#{message}\";")
  end

  def write_pending_mapping(step_name)
    append_step_definition(step_name, "pending();")
  end

  def write_calculator_code
    include_file "../support/RpnCalculator.cpp"
  end

  def write_mappings_for_calculator
    include_file "RpnCalculatorSteps.cpp"
  end

  def write_mapping_receiving_data_table_as_headless_row_array(step_name)
    not_implemented
  end

  def write_mapping_receiving_data_table_as_raw(step_name)
    not_implemented
  end

  def write_mapping_receiving_data_table_as_hashes(step_name)
    append_step_definition step_name, <<-EOF
TABLE_PARAM(table);
logTableAsHashes(table);
EOF
  end

  def write_passing_hook options = {}
    log_string = options[:log_cycle_event_as]
    if options[:type]
      hook_type  = options[:type]
      log_string ||= hook_type
    else
      hook_type  = "before"
      log_string ||= "hook"
    end
    tags        = options[:tags] || []
    define_hook = hook_type.upcase
    params      = tags.any? ? '"'+tags.join('", "')+'"' : ""

    if hook_type == "around"
      # around scenario hooks not implemented
      not_implemented
    else
      append_support_code <<-EOF
#{define_hook}(#{params}) {
  logCycleEvent("#{log_string}");
}
EOF
    end
  end

  def write_scenario options = {}
    tags = options[:with_tags] || []

    @next_step_count ||= 0
    step_name = nth_step_name @next_step_count += 1
    tags_definition = tags.any? ? "\n  #{tags.join(' ')}" : ""
    append_logging_step_definition(step_name)
    append_to_feature <<-EOF
#{tags_definition}
  Scenario: scenario #{"tagged with " + tags.join(', ') if tags.any?}
    Given #{step_name}
EOF
  end

  def write_world_variable_with_numeric_value(value)
    append_support_code <<-EOF
struct World {
  int variable;
  World() : variable(#{value}) {}
};
EOF
  end

  def write_world_function
    append_support_code <<-EOF
struct World {
    void someFunction() {
        writeToFile("#{WORLD_FUNCTION_LOG_FILE}", "");
    }
};
EOF
  end

  def write_custom_world_constructor
    # it does not make any sense! the scenario should be changed
  end

  def write_mapping_incrementing_world_variable_by_value(step_name, increment_value)
    append_step_definition step_name, <<-EOF
ScenarioScope<World> world;

world->variable += #{increment_value};
EOF
  end

  def write_mapping_logging_world_variable_value(step_name, time = "1")
    append_step_definition step_name, <<-EOF
ScenarioScope<World> world;

writeToFile("#{WORLD_VARIABLE_LOG_FILE}.#{time}", world->variable);
EOF
  end

  def write_mapping_calling_world_function(step_name)
    append_step_definition step_name, <<-EOF
ScenarioScope<World> world;

world->someFunction();
EOF
  end

  def assert_returned_success
    expect(@return).to be == true
  end

  def assert_passing_feature
    assert_no_partial_output(failed_output, @output)
    expect(@return).to be == true
  end

  def assert_failing_feature(message)
    assert_partial_output(message, @output)
    expect(@return).to be == false 
  end

  def assert_passing_scenario
    #puts @output
    assert_partial_output("1 scenario (1 passed)", @output)
    expect(@return).to be == true
  end

  def assert_failing_scenario
    assert_partial_output("1 scenario (1 failed)", @output)
    expect(@return).to be == false 
  end

  def assert_pending_scenario
    assert_partial_output("1 scenario (1 pending)", @output)
    expect(@return).to be == true
  end

  def assert_undefined_scenario
    assert_partial_output("1 scenario (1 undefined)", @output)
    expect(@return).to be == true
  end

  def assert_scenario_reported_as_failing(scenario_name)
    assert_partial_output("# Scenario: #{scenario_name}", @output)
    expect(@return).to be == false
  end

  def assert_scenario_not_reported_as_failing(scenario_name)
    assert_no_partial_output("# Scenario: #{scenario_name}", @output)
  end

  def assert_suggested_step_definition_snippet(stepdef_keyword, stepdef_pattern, parameter_count = 0, doc_string = false, data_table = false)
    not_implemented
  end

  def assert_executed_scenarios *scenario_offsets
    sequence = scenario_offsets.inject([]) do |sequence, scenario_offset|
      sequence << nth_step_name(scenario_offset)
    end
    assert_complete_cycle_sequence *sequence
  end

  def assert_world_variable_held_value_at_time(value, time)
    expect(File.read("#{WORLD_VARIABLE_LOG_FILE}.#{time}")).to match value 
  end

  def assert_world_function_called
    expect(File.file?(WORLD_FUNCTION_LOG_FILE)).to be true
  end

  def assert_cycle_sequence *args
    expected_string = args.join CYCLE_SEQUENCE_SEPARATOR
    expect(File.read(CYCLE_LOG_FILE)).to match expected_string
  end

  def assert_cycle_sequence_excluding *args
    args.each do |unexpected_string|
      expect(File.read(CYCLE_LOG_FILE)).not_to match unexpected_string
    end
  end

  def assert_complete_cycle_sequence *args
    expected_string = "#{CYCLE_SEQUENCE_SEPARATOR}#{args.join(CYCLE_SEQUENCE_SEPARATOR)}"
    expect(File.read(CYCLE_LOG_FILE)).to match expected_string
  end

  def assert_data_table_equals_json(json)
    log_file_contents = File.read(DATA_TABLE_LOG_FILE)
    actual_array      = JSON.parse(log_file_contents)
    expected_array    = JSON.parse(json)
    expect(actual_array).to be == expected_array
  end

  def run_feature
    run_feature_with_params("")
  end

  def run_feature_with_tags *tag_groups
    params = ""
    tag_groups.each do |tag_group|
      params += " --tags #{tag_group}"
    end
    run_feature_with_params(params)
  end

  def failed_output
    "failed"
  end

# cpp steps

  def append_step_definition(step_name, code) # todo params parameter?
    append_support_code <<-EOF
CUKE_STEP_("^#{step_name}$") {
#{indent_code code}
}
EOF
  end

  def append_support_code(code)
    helper_functions = get_absolute_path('../support/HelperFunctions.hpp');
    @support_code ||= <<-EOF
#include <cucumber-cpp/defs.hpp>
#include "#{helper_functions}"

using cucumber::ScenarioScope;

void logCycleEvent(const char *name) {
    logCycleEvent(name, "#{CYCLE_LOG_FILE}", "#{CYCLE_SEQUENCE_SEPARATOR}");
}

void logTableAsHashes(const cucumber::internal::Table & table) {
    logTableAsHashes(table, "#{DATA_TABLE_LOG_FILE}");
}
EOF
    @support_code += code
  end

  CUCUMBER_EXE                 = ENV["CUKE_EXE"]
  TMP_DIR                      = ENV["TMP_DIR"]
  FEATURES_DIR                 = ENV["TEST_FEATURES_DIR"]
  STEP_DEFINITIONS_SRC         = ENV["DYNAMIC_CPP_STEPS_SRC"]
  STEP_DEFINITIONS_EXE         = ENV["DYNAMIC_CPP_STEPS_EXE"]
  COMPILE_STEP_DEFINITIONS_CMD = ENV["COMPILE_DYNAMIC_CPP_STEPS"]

  WORLD_VARIABLE_LOG_FILE      = "#{TMP_DIR}/world_variable.log"
  WORLD_FUNCTION_LOG_FILE      = "#{TMP_DIR}/world_function.log"
  DATA_TABLE_LOG_FILE          = "#{TMP_DIR}/data_table.log"
  CYCLE_LOG_FILE               = "#{TMP_DIR}/cycle.log"
  CYCLE_SEQUENCE_SEPARATOR     = " -> "

private

  def not_implemented
    pending "Not yet implemented!"
  end

  def append_logging_step_definition(step_name)
    append_step_definition step_name, "logCycleEvent(\"#{step_name}\");"
  end

  def include_file(relative_file_path)
    absolute_file_path = get_absolute_path(relative_file_path)
    append_support_code <<-EOF
#include "#{absolute_file_path}"
EOF
  end

  def get_absolute_path(relative_path)
    File.expand_path(relative_path, File.dirname(__FILE__))
  end

  def nth_step_name n
    "step #{n}"
  end

  def run_feature_with_params(params)
    write_main_step_definitions_file
    compile_step_definitions
    create_wire_file
    run_cucumber_cpp
    run_cucumber_test_feature params
    begin
      Process.kill(9, @steps_out.pid)
      Process.wait @steps_out.pid
    rescue Errno::ESRCH
#jescze jedno rescue
    end
#    if(OS.windows?)
#      %x[taskkill /pid #{@steps_out.pid} /f /t]
#      %x[del /f #{STEP_DEFINITIONS_EXE}]
#    end
  end

  def write_main_step_definitions_file
    write_file(STEP_DEFINITIONS_SRC, @support_code);
  end

  def compile_step_definitions
    compiler_output = %x[ #{COMPILE_STEP_DEFINITIONS_CMD} ]
    expect($?.success?).to be == true, "Compilation failed!\n#{compiler_output}"
  end

  def create_wire_file
    write_file "#{FEATURES_DIR}/step_definitions/cucumber-cpp.wire", <<-EOF
host: localhost
port: 3902
EOF
  end

  def run_cucumber_cpp
   # puts "running: #{STEP_DEFINITIONS_EXE}"
    @steps_out = IO.popen STEP_DEFINITIONS_EXE
  end

  def run_cucumber_test_feature(params)
    cmd="cucumber #{params} #{FEATURES_DIR}"
   # puts "running #{cmd}"
    @output = %x[ #{cmd} ]
    @return = ($?.success?)
  end
end

World(CucumberCppMappings)
