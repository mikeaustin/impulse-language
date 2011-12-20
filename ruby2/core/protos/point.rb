#
# core/protos/point.rb
#

class PointProto < Frame

  def self.instance()
    @instance ||= PointProto.new(ObjectProto.instance)

    @instance.add_method(:create, FunctionProto(@instance.frame.method(:create_)))

    return @instance
  end

  def create_(receiver, args)
    value = Value(PointValue.instance.frame)

    value.x = args[0]
    value.y = args[1]
    
    return value
  end

  def frame_to_s(value)
    return "#{value.x}, #{value.y}"
  end
  
  def frame_inspect(value)
    if value.frame == PointProto.instance.frame
      return Value("<point>")
    end
    
    return "#{value.x}, #{value.y}"
  end

end

class PointValue < Frame

  def self.instance()
    return @instance ||= PointValue.new(PointProto.instance)
  end
  
  def frame_to_s(value)
    return "#{value.x}, #{value.y}"
  end
  
  def frame_inspect(value)
    return "<point> [#{value.x}, #{value.y}]"
  end
  
end

$lobby.set_local(:point, PointProto.instance)

