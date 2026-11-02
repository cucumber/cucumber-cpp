source 'https://rubygems.org'

# Select the Cucumber-Ruby version with CUCUMBER_VERSION, for example
# `export CUCUMBER_VERSION=10` before `bundle install`. Keep it set for `bundle exec`,
# otherwise Bundler switches back to the default. Cucumber-Ruby 10 needs Ruby 3.1 or later.
cucumber_versions = {
  '7.1' => { cucumber: '7.1.0', wire: '6.2.1' },
  '10' => { cucumber: '10.2.0', wire: '8.0.0' },
}
cucumber_version = ENV.fetch('CUCUMBER_VERSION', '7.1')
versions = cucumber_versions.fetch(cucumber_version) do
  raise "Unsupported CUCUMBER_VERSION '#{cucumber_version}', use one of: #{cucumber_versions.keys.join(', ')}"
end

group :test do
  gem 'cucumber', "=#{versions[:cucumber]}"
  gem 'cucumber-wire', "=#{versions[:wire]}"
end
