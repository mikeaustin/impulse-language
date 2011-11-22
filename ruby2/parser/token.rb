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

class NumberToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/\d/)

    return self.read_helper(stream, /\d/, :to_f)
  end
  
end

class StringToken < Token

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

class NewlineToken < Token

end

