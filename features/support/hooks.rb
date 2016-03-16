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
      Process.kill "KILL", @steps_out.pid #windows accepts only KILL signal - do not change please
      Process.wait @steps_out.pid
    rescue Errno::ESRCH  # exited already
    rescue Errno::ECHILD # killed before wait call
    end
  end
end
