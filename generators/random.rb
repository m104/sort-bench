#!/usr/bin/env ruby
#
# random.rb:
#   generate random integer sequences
#
# usage:
#   ./random.rb | head -n 500   # 500 numbers
#

min_value = (ARGV[0] || 1).to_i
max_value = (ARGV[1] || 1000).to_i

# ensure proper range order
if (min_value > max_value)
  (min_value, max_value) = [max_value, min_value]
end

range = max_value - min_value + 1

begin
  loop do
    puts rand(range) + min_value
  end
rescue
  # nothing
end


