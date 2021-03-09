defmodule PhotoBlogWeb.PostView do
  use PhotoBlogWeb, :view

  alias PhotoBlog.Posts.Post

  def render_json(%Post{} = post) do
    %{
      id: post.id,
      score: post.score,
    }
  end
end
