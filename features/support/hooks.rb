Before do
  require 'fileutils'
  featuresTmpSpace = ENV["TMP_DIR"]
  FileUtils.rm_rf(featuresTmpSpace) if Dir.exists?(featuresTmpSpace)
  FileUtils.mkdir(featuresTmpSpace)
  FileUtils.touch("#{featuresTmpSpace}/cycle.log")
end

After do
  # for when there are no scenarios
  if @steps_out
    begin
      Process.kill :SIGKILL, @steps_out.pid # :SIGTERM not supported on Windows
      Process.wait @steps_out.pid
    rescue Errno::ESRCH  # exited already
    rescue Errno::ECHILD # killed before wait call
    end
  end
end
