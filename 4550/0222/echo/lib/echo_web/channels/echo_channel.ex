defmodule EchoWeb.EchoChannel do
  use EchoWeb, :channel

  @impl true
  def join("echo:echo", payload, socket) do
    if authorized?(payload) do
      {:ok, socket}
    else
      {:error, %{reason: "unauthorized"}}
    end
  end

  # Channels can be used in a request/response fashion
  # by sending replies to requests from the client
  @impl true
  def handle_in("echo", %{"text" => text}, socket) do
    {:reply, {:ok, %{text: "server says: #{text}"}}, socket}
  end

  # Add authorization logic here as required.
  defp authorized?(_payload) do
    true
  end
end
