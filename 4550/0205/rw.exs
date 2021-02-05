defmodule RW do
  # Loop forever:
  #  - Read a line of text from stdin
  #  - Reverse the characters in each word
  #  - Print it
  def loop do
    case IO.gets "input: " do
      :eof ->
        IO.puts "All done"
      {:error, reason} ->
        IO.puts "Error: #{reason}"
      line ->
        IO.puts reverse_words(line)
        loop()
    end
  end

  def reverse_words(line) do
    #words = String.split(line)
    #rwords = Enum.map(word, &reverse_chars/1)
    #Enum.join(rwords, " ")

    line
    |> String.split()    # This is String.split/1
    |> Enum.map(&reverse_chars/1)
    |> Enum.join(" ")
  end

  def reverse_chars(word) do
    word
    |> String.to_charlist
    |> Enum.reverse
    |> to_string

    # This is backwards, pipe lets us write our
    # code forwards:
    # to_string(Enum.reverse(String.to_charlist(word)))

    # Erlang has two distinct data types:
    #  - List of X
    #  - Binary
    #
    # In Erlang, String is List of Char
    #
    # In Elixir, String is a Binary
  end
end

RW.loop()
