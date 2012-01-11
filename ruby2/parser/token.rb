#
# parser/token.rb
#

require './core/value.rb'


class Token < Value

  def self.read_buffer(stream, match)
    buffer = ""
    
    while stream.peek().chr.match(match)
      buffer << stream.getc()
    end #rescue nil
    
    return buffer
  end

  def self.read_token(stream, match, cast)
    buffer = self.read_buffer(stream, match)
    
    return self.new(buffer.send(cast))
  end

end

class UnknownToken < Token

end

class LitNumberToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/\d/)

    buffer += self.read_buffer(stream, /\d/)

    if stream.peek().chr.match(/\./)
      char = stream.getc()
      
      if !stream.peek().chr.match(/\d/)
        stream.ungetc(char)
        
        return self.new(buffer.to_i)
      end
      
      buffer << char
      
      buffer += self.read_buffer(stream, /\d/)

      return self.new(buffer.to_f)
    end

    return self.new(buffer.to_i)
  end
  
end

class LitStringToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/["]/)
    
    stream.getc()

    while true
      buffer += self.read_buffer(stream, /[^\\"]/)
      
      if stream.peek().chr == "\\"
        stream.getc()
        
        buffer += stream.getc()
      else
        stream.getc()
        
        return self.new(buffer.to_s)
      end
    end
        
    return nil
  end

end

class LitSymbolToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[\#]/)
    
    stream.getc()
    
    return self.read_token(stream, /[a-zA-Z\d\+\-\*\/\<\>\=\:]/, :to_sym)
  end

end

class IdentifierToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[a-zA-Z\<\>]/)

    buffer += self.read_buffer(stream, /[a-zA-Z\d\-\<\>]/)
    
    if stream.peek().chr.match(/[:]/)
      buffer << stream.getc();

      return KeywordToken.new(buffer.to_sym)
    end
    
    return self.new(buffer.to_sym)
  end

end

class KeywordToken < Token

end

class OperatorToken < Token

  SYMBOLS = /[\+\-\*\/%\<\>\@\=\!\|\&\\\^\.]/

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(SYMBOLS)

    char = stream.getc()

    if stream.peek().chr.match(/[a-zA-Z]/)
      stream.ungetc(char)

      return nil
    end

    stream.ungetc(char)

    return self.read_token(stream, SYMBOLS, :to_sym)
  end

end

class CommentToken < Token

  def self.read(stream, buffer = "")
    return nil if stream.peek().chr != "/"
    
    char = stream.getc()
    
    if stream.peek().chr != "/"
      stream.ungetc(char)
      
      return nil
    end

    stream.getc()

    while stream.peek.chr.match(/[^\n]/)
      buffer << stream.getc()
    end

    return CommentToken.new(buffer.to_s)
  end

end

class BlockCommentToken < CommentToken

  def self.read(stream, buffer = "")
    return nil if stream.peek().chr != "/"

    char = stream.getc()
    
    if stream.peek().chr != "*"
      stream.ungetc(char)
      
      return nil
    end
    
    stream.getc()

    while true
      while stream.peek().chr.match(/[^\/\*]/)
        print "| " if !$file && stream.peek().chr == "\n"
        buffer << stream.getc()
      end

      char = stream.getc()
    
      if char == "/" && stream.peek().chr == "*"
        stream.ungetc(char)
      
        self.read(stream)
      elsif char == "*" && stream.peek().chr == "/"
        stream.getc()
      
        break;
      end
    end
    
    return BlockCommentToken.new(buffer.to_s)
  end

end

class CommaToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[,]/)

    return self.read_token(stream, /[,]/, :to_sym)
  end

end

class DotOperatorToken < OperatorToken

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[\.]/)

    return self.read_token(stream, /[\.]/, :to_sym)
  end

end

class VerticalBarToken < OperatorToken

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

class ColonColonToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[:]/)

    char = stream.getc()
    
    if !stream.peek().chr.match(/[:]/)
      stream.unget(char)
      
      return nil
    end
    
    stream.getc();
    
    return self.new("::")
  end

end

class AssignToken < Token

  def self.read(stream, buffer = "")
    return nil if !stream.peek().chr.match(/[=]/)

    char = stream.getc()
    
    if stream.peek().match(OperatorToken::SYMBOLS)
      stream.ungetc(char)
    
      return nil
    end
    
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

