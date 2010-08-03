fibonacci = lambda { |n| n < 2 ? n : fibonacci [n - 1] + fibonacci [n - 2] }

result = (0..30).map { |n| fibonacci [n] }

puts result [10]

