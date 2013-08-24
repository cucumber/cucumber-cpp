Before do
  require 'fileutils'
  featuresTmpSpace = ENV["TMP_DIR"]
  FileUtils.rm_rf(featuresTmpSpace) if Dir.exists?(featuresTmpSpace) 
  FileUtils.mkdir(featuresTmpSpace)
  FileUtils.touch("#{featuresTmpSpace}/cycle.log")
end

