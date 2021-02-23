defmodule EchoWeb.PageController do
  use EchoWeb, :controller

  def index(conn, _params) do
    render(conn, "index.html")
  end
end
