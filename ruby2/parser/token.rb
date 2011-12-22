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

    while stream.peek().chr.match(/\d/)
      buffer << stream.getc()
    end rescue nil

    if stream.peek().chr.match(/\./)
      buffer << stream.getc()
      
      while stream.peek().chr.match(/\d/)
        buffer << stream.getc()
      end rescue nil

      return self.new(buffer.to_f)
    end

    return self.new(buffer.to_i)
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

class LitSymbolToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[\#]/)
    
    stream.getc()
    
    return self.read_helper(stream, /[a-zA-Z\d\+\-\*\/\<\>\=]/, :to_sym)
  end

end

class IdentifierToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[a-zA-Z\<\>]/)

    token = self.read_helper(stream, /[a-zA-Z\d\-\<\>]/, :to_sym)
    
    if stream.peek().chr.match(/[:]/)
      stream.getc();

      return KeywordToken.new(token.float)
    end
    
    return token
  end

end

class KeywordToken < Token

end

class OperatorToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[\+\-\*\/%\<\>\@\=]/)

    char = stream.getc()
    peek = stream.peek()

    stream.ungetc(char)

    if peek.chr.match(/[a-zA-Z\d]/)
      return IdentifierToken.read(stream)
    end

    if char == "=" && !peek.chr.match(/[\+\-\*\/%\<\>\@\=]/)
      return AssignToken.read(stream)
    end
    
    return self.read_helper(stream, /[\+\-\*\/%\<\>\@\=]/, :to_sym)
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

class DollarSignToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[$]/)
    
    return self.new(stream.getc().to_sym)
  end

end

class AssignToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[=]/)
    
    return self.new(stream.getc().to_sym)
  end

end

class OpenParenToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[(]/)
    
    return self.new(stream.getc().to_sym)
  end

end

class CloseParenToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[)]/)
    
    return self.new(stream.getc().to_sym)
  end

end

class OpenBracketToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[\[]/)
    
    return self.new(stream.getc().to_sym)
  end

end

class CloseBracketToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[\]]/)
    
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

