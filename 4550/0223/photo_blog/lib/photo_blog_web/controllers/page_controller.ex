defmodule PhotoBlogWeb.PageController do
  use PhotoBlogWeb, :controller

  alias PhotoBlog.Posts

  def index(conn, _params) do
    posts = Posts.list_posts()
    render(conn, "index.html", posts: posts)
  end
end
