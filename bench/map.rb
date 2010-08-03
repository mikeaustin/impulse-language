a = (1..100000000).map { |n| n * n if n % 2 == 0 }.compact

puts a.size

puts a[0]
puts a[1]
puts a[2]

