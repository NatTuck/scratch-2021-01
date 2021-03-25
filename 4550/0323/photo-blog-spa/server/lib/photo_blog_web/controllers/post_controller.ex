defmodule PhotoBlogWeb.PostController do
  use PhotoBlogWeb, :controller

  alias PhotoBlog.Posts
  alias PhotoBlog.Posts.Post

  alias PhotoBlogWeb.Plugs
  plug Plugs.RequireAuth when action
    in [:create]

  action_fallback PhotoBlogWeb.FallbackController

  def index(conn, _params) do
    posts = Posts.list_posts()
    render(conn, "index.json", posts: posts)
  end

  def create(conn, %{"post" => post_params}) do
    {:ok, photo_hash} = PhotoBlog.Photos.save_photo(
      post_params["photo"].filename,
      post_params["photo"].path
    )
    user = conn.assigns[:current_user]
    post_params = post_params
    |> Map.put("user_id", user.id)
    |> Map.put("photo_hash", photo_hash)

    IO.inspect({:post, post_params})

    with {:ok, %Post{} = post} <- Posts.create_post(post_params) do
      conn
      |> put_status(:created)
      |> put_resp_header("location", Routes.post_path(conn, :show, post))
      |> render("show.json", post: post)
    end
  end

  def show(conn, %{"id" => id}) do
    post = Posts.get_post!(id)
    render(conn, "show.json", post: post)
  end

  def update(conn, %{"id" => id, "post" => post_params}) do
    post = Posts.get_post!(id)

    with {:ok, %Post{} = post} <- Posts.update_post(post, post_params) do
      render(conn, "show.json", post: post)
    end
  end

  def delete(conn, %{"id" => id}) do
    post = Posts.get_post!(id)

    with {:ok, %Post{}} <- Posts.delete_post(post) do
      send_resp(conn, :no_content, "")
    end
  end
end
