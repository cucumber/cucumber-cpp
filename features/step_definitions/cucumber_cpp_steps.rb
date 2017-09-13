Given /^a step definition file with:$/ do |code|
    set_code code
end

Given /^a step definition file with support code including:$/ do |code|
    append_support_code code
end

Then /^the step output should contain:$/ do |output|
    expect(@steps_out.gets).to eq(output)
end

Then /^a step definition snippet with (".*") is suggested$/ do |regex_string|
    assert_partial_output("(#{regex_string}) {", all_output)
end

Then /^the scenario fails with message "([^"]*)"$/ do |message|
  assert_partial_output(message, all_output)
  assert_success false
end

