module CucumberCppMappings

  def features_dir
    FEATURES_DIR
  end

  def write_passing_mapping(step_name)
    append_step_definition(step_name, '// no-op, pass gently')
  end

  def write_failing_mapping(step_name)
    append_step_definition(step_name, "throw \"rotten cucumbers\";")
  end

  def write_failing_mapping_with_message(step_name, message)
    pending
  end

  def write_pending_mapping(step_name)
    append_step_definition(step_name, "pending();")
  end

  def write_calculator_code
    pending
  end

  def write_mappings_for_calculator
    pending
  end

  def write_mapping_receiving_data_table_as_headless_row_array(step_name)
    pending
  end

  def write_mapping_receiving_data_table_as_raw(step_name)
    pending
  end

  def write_mapping_receiving_data_table_as_hashes(step_name)
    pending
  end

  def write_passing_hook options = {}
    pending
  end

  def write_scenario options = {}
    pending
  end

  def write_world_variable_with_numeric_value(value)
    pending
  end

  def write_world_function
    pending
  end

  def write_custom_world_constructor
    pending
  end

  def assert_passing_scenario
    assert_partial_output("1 scenario (1 passed)", all_output)
    assert_success true
  end

  def assert_failing_scenario
    assert_partial_output("1 scenario (1 failed)", all_output)
    assert_success false
  end

  def assert_pending_scenario
    assert_partial_output("1 scenario (1 pending)", all_output)
    assert_success true
  end

  def assert_undefined_scenario
    assert_partial_output("1 scenario (1 undefined)", all_output)
    assert_success true
  end

  def assert_scenario_reported_as_failing(scenario_name)
    assert_partial_output("# Scenario: #{scenario_name}", all_output)
    assert_success false
  end

  def assert_scenario_not_reported_as_failing(scenario_name)
    assert_no_partial_output("# Scenario: #{scenario_name}", all_output)
  end

  def assert_suggested_step_definition_snippet(stepdef_keyword, stepdef_pattern, parameter_count = 0, doc_string = false, data_table = false)
    pending
  end

  def run_feature
    write_main_step_definitions_file
    compile_step_definitions
    create_wire_file
    run_cucumber_cpp
    run_cucumber_test_feature
  end

  def failed_output
    "failed"
  end

# cpp steps

  def append_step_definition(step_name, code) # todo params parameter?
    append_support_code "CUKE_STEP_(\"#{step_name}\") {\n#{code}\n}\n"
  end

  def append_support_code(code)
    @support_code ||= "#include <cucumber-cpp/defs.hpp>\n"
    @support_code += code
  end

private

  FEATURES_DIR = ENV["TEST_FEATURES_DIR"]
  STEP_DEFINITIONS_SRC = ENV["DYNAMIC_CPP_STEPS_SRC"]
  STEP_DEFINITIONS_EXE = ENV["DYNAMIC_CPP_STEPS_EXE"]
  COMPILE_STEP_DEFINITIONS_CMD = ENV["COMPILE_DYNAMIC_CPP_STEPS"]

  def write_main_step_definitions_file
    write_file(STEP_DEFINITIONS_SRC, @support_code);
  end

  def compile_step_definitions
    write_file("/tmp/badger", COMPILE_STEP_DEFINITIONS_CMD)
    compiler_output = %x[ #{COMPILE_STEP_DEFINITIONS_CMD} ]
    expect($?.success?).to eq(true)
  end

  def create_wire_file
    contents = "host: localhost\nport: 3902\n"
    filename = "#{FEATURES_DIR}/step_definitions/cucumber-cpp.wire"
    write_file filename, contents
  end

  def run_cucumber_cpp
    @steps_out = IO.popen STEP_DEFINITIONS_EXE, 'r+'
  end

  def run_cucumber_test_feature
    run_simple "cucumber #{FEATURES_DIR}", false
  end
end

World(CucumberCppMappings)
