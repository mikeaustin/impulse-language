#
# core/protos/object.rb
#

require './core/protos/nil.rb'


class ObjectProto < Frame

  def self.instance()
    return @instance ||= ObjectProto.new(NilProto.instance)
  end

  def init_slots()
    self.add_method(:"id",      FunctionProto(self.method(:id)))
    self.add_method(:"type",    FunctionProto(self.method(:type)))
    self.add_method(:"is-a:",   FunctionProto(self.method(:is_a)))
    self.add_method(:"assign",  FunctionProto(self.method(:assign_)))
    self.add_method2(:"methods", [])     { |receiver, args| self._methods(receiver) }
    self.add_method2(:"send:",       []) { |receiver, args| self.send_(receiver, args) }
    self.add_method2(:"add-module:", [SymbolProto.instance, BlockProto.instance], ["symbol, function"], "Adds a module definition the the current scope") {
                                      |receiver, args| self.add_module(receiver, args[0].float, args[1]) }
    self.add_method2(:"add-object:", [SymbolProto.instance, BlockProto.instance]) {
                                      |receiver, args| self.add_object(receiver, args[0].float, args[1]) }
    self.add_method2(:"add-method:", [SymbolProto.instance, BlockProto.instance]) \
                                         { |receiver, args| receiver.add_method(args[0].float, args[1]) }
    self.add_method2(:"add-field:", [])  { |receiver, args| receiver.set_local(args[0].float, args[1]) }
    self.add_method2(:"tee:",  [])       { |receiver, args| tee(receiver, args[0]) }
    self.add_method2(:"with:",   [])       { |receiver, args| do_(receiver, args[0]) }
  end

  def frame_inspect(value)
    if value.frame == ObjectProto.instance.frame
      return "<object>"
    end
    
    return super()
  end

  def id(receiver, args)
    return Value(receiver.frame.object_id)
  end

  def type(receiver, args)
    return receiver.proto
  end

  def is_a(receiver, args)
    return Value(receiver.frame_is_a(args[0]))
  end

  def assign_(receiver, args)
    return receiver.set_slot(args[0].float, args[1])
  end

  def _methods(receiver)
  	puts receiver.inspect if !receiver.frame_methods.empty?
  	
  	receiver.frame_methods.each do |selector, function|
  	  arg_names = function.frame.arg_names.join(", ")
  	  
  	  puts "  #{selector} #{arg_names}\r\t\t\t\t#{function.frame.summary_doc}"
  	end
  	
  	self._methods(receiver.proto) if receiver.proto
  	
  	return nil
  end

  def tee(receiver, block)
    block.frame._call(block, [receiver])

    return receiver
  end

  def do_(receiver, block)
    return block.frame._call(block, [receiver])
  end

  def send_(receiver, args)
    return receiver.send_(args[0].float, args[1] ? args[1].frame.array : [], $lobby)
  end

  def add_module(receiver, symbol, block)
    object = Frame.new(ObjectProto.instance)
    block.frame._call(block, [object])
    
    receiver.add_module(symbol, object)
    
    return nil
  end

  def add_object(receiver, symbol, block)
    object = Frame.new(ObjectProto.instance)
    block.frame._call(block, [object])
    
    receiver.set_local(symbol, object)
    
    return nil
  end

end

