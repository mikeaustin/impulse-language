
module String_

  def self.foo(string)
    string + "-foo"
  end

end

class Object

  def method_missing(symbol, *args, &block)
    Kernel.const_get("#{self.class}_").send(symbol, *([self] + args), &block)
  end

end

puts "kung".foo()

