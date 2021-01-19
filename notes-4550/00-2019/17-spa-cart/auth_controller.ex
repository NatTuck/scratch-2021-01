defmodule HuskyShopWeb.AuthController do
  use HuskyShopWeb, :controller

  alias HuskyShop.Users
  alias HuskyShop.Users.User

  action_fallback HuskyShopWeb.FallbackController

  def authenticate(conn, %{"email" => email, "password" => password}) do
    with {:ok, %User{} = user} <- Users.authenticate_user(email, password) do
      resp = %{
        data: %{
          token: Phoenix.Token.sign(HuskyShopWeb.Endpoint, "user_id", user.id),
          user_id: user.id,
        }
      }

      conn
      |> put_resp_header("content-type", "application/json; charset=UTF-8")
      |> send_resp(:created, Jason.encode!(resp))
    end
  end
end
