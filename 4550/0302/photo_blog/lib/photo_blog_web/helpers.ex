defmodule PhotoBlogWeb.Helpers do
  alias PhotoBlog.Users.User

  def current_user_id(conn) do
    user = conn.assigns[:current_user]
    user && user.id
  end

  def have_current_user?(conn) do
    conn.assigns[:current_user] != nil
  end

  def current_user_is?(conn, %User{} = user) do
    current_user_is?(conn, user.id)
  end

  def current_user_is?(conn, user_id) do
    current_user_id(conn) == user_id
  end
end
