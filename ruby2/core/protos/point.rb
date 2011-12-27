#
# core/protos/point.rb
#


class PointProto < Frame

  def self.instance()
    @instance ||= PointProto.new(ObjectProto.instance)

    @instance.add_method2(:"slice", [NumberProto.instance, NumberProto.instance]) {
                                               |receiver, args| receiver.frame._clone(args[0], args[1]) }
    @instance.add_method2(:"+", [@instance]) { |receiver, args| receiver.frame.add(receiver, args[0]) }
    @instance.add_method2(:"-", [@instance]) { |receiver, args| receiver.frame.sub(receiver, args[0]) }
 
    return @instance
  end

  def _clone(x, y)
    value = Value(PointValue.instance.frame)
    
    value.x = x
    value.y = y
    
    return value
  end

  def frame_to_s(value)
    return "#{value.x}, #{value.y}"
  end
  
  def frame_inspect(value)
    if value.frame == PointProto.instance.frame
      return "<point>"
    end
    
    return "<point> [#{value.x.inspect}, #{value.y.inspect}]"
  end

  #
  # <point> methods
  #
  
  def add(point, other)
    return PointProto.instance.frame._clone(point.x.float + other.x.float, point.y.float + other.y.float)
  end

  def sub(point, other)
    return PointProto.instance.frame._clone(point.x.float - other.x.float, point.y.float - other.y.float)
  end

end

class PointValue < PointProto

  def self.instance()
    return @instance ||= PointValue.new(PointProto.instance)
  end
  
end

