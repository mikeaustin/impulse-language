#
# core/protos/set.rb
#

require './core/protos/object.rb'
require 'set'


class SetProto < Frame

  attr :set, true

  def self.instance
    @instance ||= SetProto.new(ObjectProto.instance)

    @instance.add_method2(:"slice", []) { |receiver, args| receiver.frame.slice(args) }
    @instance.add_method2(:"size", [])  { |receiver, args| receiver.frame.size }

    @instance.add_method2(:"|", [])     { |receiver, args| receiver.frame.union(args[0].frame) }
    @instance.add_method2(:"&", [])     { |receiver, args| receiver.frame.intersection(args[0].frame) }
    @instance.add_method2(:"\\", [])    { |receiver, args| receiver.frame.difference(args[0].frame) }
    @instance.add_method2(:"^", [])     { |receiver, args| receiver.frame.sym_diff(args[0].frame) }

    @instance.add_method2(:"<", [])     { |receiver, args| receiver.frame.proper_subset(args[0].frame) }
    @instance.add_method2(:">", [])     { |receiver, args| receiver.frame.proper_superset(args[0].frame) }
    @instance.add_method2(:"<=", [])     { |receiver, args| receiver.frame.subset(args[0].frame) }
    @instance.add_method2(:">=", [])     { |receiver, args| receiver.frame.superset(args[0].frame) }

    return @instance
  end

  def frame_inspect(value)
    if value.frame == SetProto.instance.frame
      return "<set>"
    end
    
    return "<set> [#{value.frame.set.to_a.join(', ')}]"
  end

  def create(set)
    object = SetProto.new(Value(self))

    object.frame.set = set
    
    return object
  end

  #
  # <set> methods
  #

  def slice(args)
    object = self.class.new(SetProto.instance)
    object.frame.set = args.to_set
    
    return object
  end

  def size
    return Value(self.set.size)
  end

  def union(other)
    return self.create(self.set | other.set)
  end

  def intersection(other)
    return self.create(self.set & other.set)
  end

  def difference(other)
    return self.create(self.set - other.set)
  end

  def sym_diff(other)
    return self.create(self.set ^ other.set)
  end

  def proper_subset(other)
    return Value(self.set.proper_subset?(other.set))
  end

  def proper_superset(other)
    return Value(self.set.proper_superset?(other.set))
  end

  def subset(other)
    return Value(self.set.subset?(other.set))
  end

  def superset(other)
    return Value(self.set.superset?(other.set))
  end

end

