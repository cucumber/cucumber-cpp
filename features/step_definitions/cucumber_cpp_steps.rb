Given /^a step definition file with:$/ do |code|
    append_support_code code
end

Then /^the step output should contain:$/ do |output|
    expect(@steps_out.gets).to eq(output)
end

