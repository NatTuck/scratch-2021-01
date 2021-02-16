defmodule HangmanWeb.GameChannel do
  use HangmanWeb, :channel

  alias Hangman.Game
  alias Hangman.BackupAgent

  @impl true
  def join("game:" <> name, payload, socket) do
    if authorized?(payload) do
      game = BackupAgent.get(name) || Game.new
      socket = socket
      |> assign(:name, name)
      |> assign(:game, game)
      BackupAgent.put(name, game)
      view = Game.view(game)
      {:ok, view, socket}
    else
      {:error, %{reason: "unauthorized"}}
    end
  end

  @impl true
  def handle_in("guess", %{"letter" => ll}, socket0) do
    game0 = socket0.assigns[:game]
    game1 = Game.guess(game0, ll)
    socket1 = assign(socket0, :game, game1)
    BackupAgent.put(socket0.assigns[:name], game1)
    view = Game.view(game1)
    {:reply, {:ok, view}, socket1}
  end

  @impl true
  def handle_in("reset", _, socket) do
    game = Game.new
    socket = assign(socket, :game, game)
    view = Game.view(game)
    {:reply, {:ok, view}, socket}
  end

  # Add authorization logic here as required.
  defp authorized?(_payload) do
    true
  end
end
