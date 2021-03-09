defmodule Hangman.GameServer do
  use GenServer

  def reg(name) do
    {:via, Registry, {Hangman.GameReg, name}}
  end

  def start(name) do
    spec = %{
      id: __MODULE__,
      start: {__MODULE__, :start_link, [name]},
      restart: :permanent,
      type: :worker,
    }
    Hangman.GameSup.start_child(spec)
  end

  def start_link(name) do
    game = Hangman.BackupAgent.get(name) || Hangman.Game.new()
    GenServer.start_link(__MODULE__, game, name: reg(name))
  end

  def guess(name, letter) do
    GenServer.call(reg(name), {:guess, name, letter})
  end

  def peek(name) do
    GenServer.call(reg(name), {:peek, name})
  end

  def init(game) do
    {:ok, game}
  end

  def handle_call({:guess, name, letter}, _from, game) do
    game = Hangman.Game.guess(game, letter)
    Hangman.BackupAgent.put(name, game)
    {:reply, game, game}
  end

  def handle_call({:peek, _name}, _from, game) do
    {:reply, game, game}
  end
end
