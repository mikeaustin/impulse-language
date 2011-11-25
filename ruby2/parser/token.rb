#
# parser/token.rb
#

class Token < Value

  def initialize(value)
    super(value)
  end

  def self.read_helper(stream, match, cast)
    buffer = ""
    
    while stream.peek().chr.match(match)
      buffer << stream.getc()
    end rescue nil
    
    return self.new(buffer.send(cast))
  end

end

class LitNumberToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/\d/)

    return self.read_helper(stream, /\d/, :to_f)
  end
  
end

class LitStringToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/["]/)
    
    stream.getc()
    token = self.read_helper(stream, /[^"]/, :to_s)
    stream.getc()
    
    return token
  end

end

class IdentifierToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[a-zA-Z]/)

    return self.read_helper(stream, /[a-zA-Z\d-]/, :to_sym)
  end

end

class XOperatorToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[+-]/)
    
    return self.read_helper(stream, /[+-]/, :to_sym)
  end

end

class CommaToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[,]/)
    
    return self.read_helper(stream, /[,]/, :to_sym)
  end

end

class VerticalBarToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[|]/)
    
    return self.new(stream.getc().to_sym)
  end

end

class NewlineToken < Token

  def self.read(stream)
    return nil if !stream.peek().chr.match(/\n/)
    
    stream.getc()
    
    return NewlineToken.new("\\n".to_sym)
  end

end

