#
# core/protos/nil.rb
#

class NilProto < Frame

  def self.instance()
    return @instance ||= NilProto.new()
  end

  def send_(selector, receiver, args)
    return Value(self)
  end

  def frame_inspect(value)
    return "nil"
  end

end

