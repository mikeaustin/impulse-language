factorial = lambda { |n| n == 0 ? 1 : n * factorial [n - 1] }

facts = (1..10000).map { |n| factorial [170] }

puts facts[5]

