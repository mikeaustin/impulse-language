#
# core/protos/nil.rb
#

require './core/frame.rb'


class NilProto < Frame

  def self.instance()
    return @instance ||= NilProto.new()
  end

  def send_(selector, receiver, args)
    return NilProto.instance
  end

  def frame_to_s(value)
    return "nil"
  end

  def frame_inspect(value)
    return "nil"
  end

end


class NothingProto < NilProto

  def self.instance()
    return @instance ||= NothingProto.new(NilProto.instance)
  end

  def frame_to_s(value)
    return "()"
  end

  def frame_inspect(value)
    return "()"
  end

end

